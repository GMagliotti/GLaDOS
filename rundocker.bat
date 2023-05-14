docker start dockerarqui
docker exec -it dockerarqui make clean -C/root/Toolchain
docker exec -it dockerarqui make clean -C/root/
docker exec -it dockerarqui make -C/root/Toolchain
docker exec -it dockerarqui make -C/root/
docker stop dockerarqui