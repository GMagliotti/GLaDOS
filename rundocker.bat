docker start SO
docker exec -it SO make clean -C/root/TP2-SO/GLaDOS/Toolchain
docker exec -it SO make clean -C/root/TP2-SO/GLaDOS/
docker exec -it SO make -C/root/TP2-SO/GLaDOS/Toolchain
docker exec -it SO make enable_buddy=1 -C/root/TP2-SO/GLaDOS/
docker stop SO