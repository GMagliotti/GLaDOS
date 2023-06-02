#!/bin/bash

docker start GLaDOS
docker exec -w /root/Toolchain -ti GLaDOS make clean all
docker exec -w /root -ti GLaDOS make clean all enable_buddy=1
docker exec -w /root -ti GLaDOS chmod -R 777 *
