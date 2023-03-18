#!/bin/python3

import docker
import os
from pathlib import Path
from dataclasses import dataclass

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
for dockerfile in path_project.glob("toolkits/*/*/*.dockerfile"):
    name = dockerfile.stem
    lang = dockerfile.parents[1].name
    toolkits.append(Toolkit(name, dockerfile, lang))
print(len(toolkits), "toolkits found")

# Setup docker client
docker_client = docker.DockerClient(base_url="unix://var/run/docker.sock")

# Build docker image for each toolkit
for toolkit in toolkits:
    print('Building docker image for toolkit "%s"' % toolkit.name)
    docker_client.images.build(
        path=str(toolkit.dockerfile.parent),
        dockerfile=toolkit.dockerfile,
        tag="gtb/%s" % toolkit.name,
        rm=False)

# Cleanup images after benchmarking
#for image in docker_client.images.list():
#    if len(image.tags) > 0 and image.tags[0].startswith("gtb"):
#        docker_client.images.remove(image.id, force=True)
#docker_client.images.prune()


#docker build -t gtb:latest -f druid.dockerfile .
#xhost +
#docker run --rm -it --network host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix bench:latest
