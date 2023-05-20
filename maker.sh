#!/bin/bash

docker exec -w /root/Toolchain -ti GLaDOS make clean all
docker exec -w /root -ti GLaDOS make clean all

