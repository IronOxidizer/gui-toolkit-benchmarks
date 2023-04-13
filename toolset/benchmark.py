#!/bin/python3

import docker

import json
import os
import time
from ctypes import cdll
from dataclasses import dataclass
from pathlib import Path

KB = 1024

@dataclass
class Toolkit:
    name: str
    dockerfile: Path
    lang: str

# Set current working directory to the repo root
path_project = Path(os.path.realpath(__file__)).parents[1]
os.chdir(path_project)
print("Working directory set to", path_project)

# Get all toolkits
toolkits = []
results = []
for dockerfile in path_project.glob("toolkits/*/*/*.dockerfile"):
    name = dockerfile.stem
    lang = dockerfile.parents[1].name
    toolkits.append(Toolkit(name, dockerfile, lang))
print(f"Found {len(toolkits)} toolkits")

# Setup docker client
docker_client = docker.DockerClient(base_url="unix://var/run/docker.sock")

# Setup pixelpeep
pixelpeep = cdll.LoadLibrary("./toolset/pixelpeep.so")
print("Warming up pixelpeep")
pixelpeep.await_stable_image(None, 0, 0)

# Build docker image for each toolkit
for toolkit in toolkits:
    name = toolkit.name
    print('\nBuilding docker image for toolkit "%s"' % name)
    docker_client.images.build(
        path=str(toolkit.dockerfile.parent),
        dockerfile=toolkit.dockerfile,
        tag="gtb/%s" % name,
        rm=False)
    
    print("Launching toolkit and waiting for image to stabilize")
    container = docker_client.containers.run(
        "gtb/%s" % name,
        name=name,
        volumes={"/tmp/.X11-unix": {"bind": "/tmp/.X11-unix", "mode": "rw"}},
        network_mode="host",
        environment={"DISPLAY": os.getenv("DISPLAY")},
        detach=True)
    
    startup = pixelpeep.await_stable_image(None, 0, 0)
    print("Recording metrics for toolkit")
    memory = round(container.stats(stream=False)["memory_stats"]["usage"] / KB)
    result = {"name": name, "memory": memory, "startup": startup}
    print(result)
    results.append(result)
    
    print("Stopping toolkit")
    container.remove(force=True)
    
print("\nSaving results")
with open(f"toolset/result.json", "w") as f:
    json.dump(results, f)

# Cleanup images after benchmarking
#for image in docker_client.images.list():
#    if len(image.tags) > 0 and image.tags[0].startswith("gtb"):
#        docker_client.images.remove(image.id, force=True)
#docker_client.images.prune()

#docker build -t gtb:latest -f druid.dockerfile .
#xhost +
#docker run --rm -it --network host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix gtb:latest

#docker system prune -a
