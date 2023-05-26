docker start elastic_dhawan
docker exec -it elastic_dhawan make clean -C/root/Toolchain
docker exec -it elastic_dhawan make clean -C/root/
docker exec -it elastic_dhawan make -C/root/Toolchain
docker exec -it elastic_dhawan make enable_buddy=1 -C/root/
docker stop elastic_dhawan