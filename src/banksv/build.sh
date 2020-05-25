#!/bin/sh
rm banksv.c
ecpg banksv.pcg
rm banksv
gcc -g -I. -I ../../ubftab  -I /usr/include/postgresql banksv.c -o banksv -latmisrv -latmi -lubf -lnstd -lrt -ldl -lm -pthread /usr/lib/x86_64-linux-gnu/libecpg.a /usr/lib/x86_64-linux-gnu/libpgtypes.a -lpq
cp -f /opt/app1/src/banksv/banksv /opt/app1/bin/banksv