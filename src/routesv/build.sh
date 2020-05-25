#!/bin/sh
rm routesv.c
ecpg routesv.pcg
rm routesv
gcc -g -I. -I ../../ubftab  -I /usr/include/postgresql routesv.c -o routesv -latmisrv -latmi -lubf -lnstd -lrt -ldl -lm -pthread /usr/lib/x86_64-linux-gnu/libecpg.a /usr/lib/x86_64-linux-gnu/libpgtypes.a -lpq
cp -f /opt/app1/src/routesv/routesv /opt/app1/bin/routesv