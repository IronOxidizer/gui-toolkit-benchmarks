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
print("Set working directory to", path_project)

# Get all toolkits
toolkits = []
for dockerfile in path_project.glob("toolkits/*/*/*.dockerfile"):
    name = dockerfile.stem
    lang = dockerfile.parents[1].name
    toolkits.append(Toolkit(name, dockerfile, lang))

# Benchmark each toolkit
for toolkit in toolkits:
    pass

#docker build -t bench:latest -f druid.dockerfile .
#xhost +
#docker run --rm -it --network host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix bench:latest
