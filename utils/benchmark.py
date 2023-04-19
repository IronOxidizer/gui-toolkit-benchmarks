#!/bin/python3

import docker

import json
import os
import time
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
    executable_size: int = -1
    dependencies_size: int = -1

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
    docker_client.images.build(
        path=str(bench_path),
        dockerfile=build_dockerfile,
        tag="gtb-build/%s" % name,
        rm=False)
    
    # Create executable
    executable_path = bench_path / "executable"
    executable_path.mkdir(exist_ok=True)
    docker_client.containers.run(
        "gtb-build/%s" % name,
        name=name,
        volumes={str(executable_path): {"bind": "/executable", "mode": "rw"}},
        remove=True)
    
    
# Create bench image for each toolkit
for toolkit in toolkits:
    name = toolkit.name
    print('Building bench image for toolkit "%s"' % name)
    dockerfile_path = project_path / "toolkits" / toolkit.lang / name / "bench.dockerfile"
    image = docker_client.images.build(
        fileobj=open(dockerfile_path, "rb"), # fileobj is required to skip a build context
        tag="gtb/%s" % name,
        rm=False)
    toolkit.dependencies_size = round((image[0].attrs["Size"] - base_size) / KB)
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
    container = docker_client.containers.create(
        "gtb/%s" % name,
        name=name,
        volumes={"/tmp/.X11-unix": {"bind": "/tmp/.X11-unix", "mode": "rw"},
                 str(executable_path): {"bind": "/executable", "mode": "rw"}},
        network_mode="host",
        environment={"DISPLAY": os.getenv("DISPLAY")},
        detach=True)
    
    for i in range(RUNS):
        container.start()
        toolkit.startup += pixelpeep.await_stable_image(None, 0, 0)
        toolkit.memory += container.stats(stream=False)["memory_stats"]["usage"] 
        container.stop(timeout=1)
        time.sleep(1)
        
    print("Recording metrics for toolkit and cleaning up container")
    toolkit.startup = round(toolkit.startup / RUNS)
    toolkit.memory = round(toolkit.memory / (KB * RUNS))
    toolkit.executable_size = round(sum(f.stat().st_size for f in executable_path.glob("**/*") if f.is_file()) / KB)
    container.remove(force=True)
    print(toolkit.__dict__)
    
os.system("xdotool key Super+d") # Restore windows, couldn't get this working using vanilla xlib
print("\nSaving results")
with open(f"utils/result.json", "w") as f:
    json.dump([tk.__dict__ for tk in toolkits], f)

# Cleanup images after benchmarking
#for image in docker_client.images.list():
#    if len(image.tags) > 0 and image.tags[0].startswith("gtb"):
#        docker_client.images.remove(image.id, force=True)
#docker_client.images.prune()

#docker build -t gtb:latest -f druid.dockerfile .
#xhost +
#docker run --rm -it --network host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix gtb:latest

#docker system prune -a
