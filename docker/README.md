# Docker guide

This guide contains instructions for building and running the docker image of this project.

## Table of contents

- [Build image](#build-image)
- [Run image](#run-image)
- [Useful docker commands](#useful-docker-commands)

## Build image

Build docker image using the following commands:

```sh
$ cd <project-directory>
$ cd docker
$ docker build . -t circuitsegmentation
```

## Run image

Run image with creation of a container and mount the project:

```sh
$ # Project path is the absolute path (for example, c:\Projects\circuit-segmentation)
$ docker run -it --name mycontainer -v <project-directory>:/src circuitsegmentation
```

The project can now be compiled and run inside the container, using the commands described [here](../README.md).

## Useful docker commands

Build image:
```sh
$ docker build . -t <image_name>
```

List images:

```sh
$ docker images
```

Remove image:

```sh
$ docker rmi <image_name>
```

Run container:

```sh
$ # Create without naming container
$ docker run -it <image_name>
$ # Create with naming container
$ docker run -it --name <container_name> <image_name>
$ # Create and remove container in the end
$ docker run -it --rm <image_name>
$ # Create and mount
$ docker run -it --name <container_name> -v <source>:<target> <image_name>
```

List containers:

```sh
$ docker ps -a
```

Start container:

```sh
$ docker start -i <container_name>
```

Stop container:

```sh
$ docker stop <container_name>
```

Execute a command inside of container:

```sh
$ docker exec -it <container_name> <command>
```

Remove container:

```sh
$ docker rm <container_name>
```
