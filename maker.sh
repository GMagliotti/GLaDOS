#!/bin/bash

docker exec -w /root/Toolchain -d GLaDOS make clean all
docker exec -w /root -d GLaDOS make clean all

