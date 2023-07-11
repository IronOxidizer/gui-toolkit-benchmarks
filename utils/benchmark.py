#!/bin/python3

import docker

import json
import os
import time
import sys
from ctypes import cdll
from dataclasses import dataclass
from pathlib import Path

KB = 1024
RUNS = 5

@dataclass
class Toolkit:
    name: str
    lang: str
    mode: str = ""
    platform_lib: str = ""
    memory: int = -1
    startup: int = -1
    executable_size: int = 0
    dependencies_size: int = -1
    
def print_debug(e):
    print("Build errored:")
    for line in e.build_log:
        if "stream" in line:
            print(line["stream"].strip())
    raise

single = any("single" in arg for arg in sys.argv)

# Set current working directory to the repo root
project_path = Path(os.path.realpath(__file__)).parents[1]
os.chdir(project_path)
print("Working directory set to", project_path)

# Get all toolkits
toolkits = []
for bench in project_path.glob("toolkits/*/*/"):
    name = bench.name
    lang = bench.parent.name
    toolkit = Toolkit(name, lang)
    
    # Load metadata
    metadata_file = bench / "metadata.json"
    if metadata_file.is_file():
        with open(metadata_file, "r") as f:
            metadata = json.load(f)
            if "mode" in metadata: toolkit.mode = metadata["mode"]
            if "platform_lib" in metadata: toolkit.platform_lib = metadata["platform_lib"]
        
    toolkits.append(toolkit)
print(f"Found {len(toolkits)} toolkits")

if (single):
    selected_toolkit = input("\nRunning in single mode\n"
        + "\n".join([f"[{i}] {toolkit.lang}/{toolkit.name}" for i, toolkit in enumerate(toolkits)])
        + "\nSelect a toolkit: ")
    toolkits = [toolkits[int(selected_toolkit)]]

# Setup docker client
docker_client = docker.DockerClient(base_url="unix://var/run/docker.sock")
base_size = docker_client.images.pull("debian:stable-slim").attrs["Size"]

for toolkit in toolkits:
    name = toolkit.name
    bench_path = project_path / "toolkits" / toolkit.lang / name
    build_dockerfile = bench_path / "build.dockerfile"
    if not build_dockerfile.is_file(): continue
    print('Building executable for toolkit "%s"' % name)
    
    # Create build image
    try:
        docker_client.images.build(
            path=str(bench_path),
            dockerfile=build_dockerfile,
            tag="gtb-build/%s" % name,
            rm=False)
    except Exception as e: print_debug(e)
    
    # Create executable
    executable_path = bench_path / "executable"
    executable_path.mkdir(exist_ok=True)
    docker_client.containers.run(
        "gtb-build/%s" % name,
        name=name,
        volumes={str(executable_path): {"bind": "/executable", "mode": "rw"}},
        remove=True)
    toolkit.executable_size = round(sum(f.stat().st_size for f in executable_path.glob("**/*") if f.is_file()) / KB)
    
# Create bench image for each toolkit
for toolkit in toolkits:
    name = toolkit.name
    print('Building bench image for toolkit "%s"' % name)
    bench_path = project_path / "toolkits" / toolkit.lang / name
    dockerfile_path = bench_path / "bench.dockerfile"
    try:
        image = docker_client.images.build(
            #fileobj=open(dockerfile_path, "rb"), # fileobj is required to skip a build context
            path = str(bench_path),
            dockerfile=dockerfile_path,
            tag="gtb/%s" % name,
            rm=False)
        toolkit.dependencies_size = round((image[0].attrs["Size"] - base_size) / KB) - toolkit.executable_size
    except Exception as e: print_debug(e)
print("Cooling down after builds")
time.sleep(10)
os.system("xdotool key Super+d") # Minimize windows, couldn't get this working using vanilla xlib

# Setup pixelpeep
pixelpeep = cdll.LoadLibrary("./utils/pixelpeep.so")
print("Warming up pixelpeep")
pixelpeep.await_stable_image(None, 0, 0)

# Benchmark each toolkit
for toolkit in toolkits:
    name = toolkit.name
    executable_path = project_path / "toolkits" / toolkit.lang / name / "executable"
    print("\nBenchmarking", name)
    try:
        container = docker_client.containers.create(
            "gtb/%s" % name,
            name=name,
            volumes={"/tmp/.X11-unix": {"bind": "/tmp/.X11-unix", "mode": "rw"},
                     "/run/dbus/system_bus_socket": {"bind": "/run/dbus/system_bus_socket", "mode": "rw"},
                     str(executable_path): {"bind": "/executable", "mode": "rw"}
                    },
            network_mode="host",
            environment={"DISPLAY": os.getenv("DISPLAY")},
            detach=True,
            security_opt=["seccomp:unconfined"])
    except Exception as e: print_debug(e)
    
    for i in range(RUNS):
        try:
            container.start()
        except Exception as e: print_debug(e)
        toolkit.startup += pixelpeep.await_stable_image(None, 0, 0)
        toolkit.memory += container.stats(stream=False)["memory_stats"]["usage"] 
        container.stop(timeout=1)
        time.sleep(1)
        
    print("Recording metrics for toolkit and cleaning up container")
    toolkit.startup = round(toolkit.startup / RUNS)
    toolkit.memory = round(toolkit.memory / (KB * RUNS))
    container.remove(force=True)
    print(toolkit.__dict__)
    
os.system("xdotool key Super+d") # Restore windows, this doesn't work for some reason
print("\nSaving results")
# Do NOT record results in single mode
if (not single):
    with open(f"utils/result.json", "w") as f:
        json.dump([tk.__dict__ for tk in toolkits], f)

# Cleanup images after benchmarking
#for image in docker_client.images.list():
#    if len(image.tags) > 0 and image.tags[0].startswith("gtb"):
#        docker_client.images.remove(image.id, force=True)
#docker_client.images.prune()

#docker build -t gtb:latest -f build.dockerfile -v /home/user/gui-toolkit-benchmarks/toolkits/Rust/druid/executable:/executable . 
#docker run --rm -it -v /home/user/gui-toolkit-benchmarks/toolkits/JavaScript/electron/executable:/executable gtb-build/electron:latest
#xhost local:root
# docker build --progress=plain -t gtb/electron:latest -f bench.dockerfile --no-cache .
# docker run --rm -it --network host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v /home/user/gui-toolkit-benchmarks/toolkits/JavaScript/electron/executable:/executable gtb/electron:latest

#docker system prune -a
