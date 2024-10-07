#!/bin/bash

IMG_NAME=build_env_img
THIS_FILE_PATH=$(dirname "$0")
DOCKERFILE=${THIS_FILE_PATH}/../docker/env.docker

docker build -t ${IMG_NAME} -f ${DOCKERFILE} .
