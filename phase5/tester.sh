#!/bin/bash

./scc < examples/array.c > array.s
gcc -m32 -S -O0 -c examples/array-lib.c
gcc -o array -m32 array.s array-lib.s
./array > array.out
diff array.out examples/array.out

./scc < examples/global.c > global.s
gcc -m32 -S -O0 -c examples/global-lib.c
gcc -o global -m32 global.s global-lib.s
./global > global.out
diff global.out examples/global.out

./scc < examples/local.c > local.s
gcc -m32 -S -O0 -c examples/local-lib.c
gcc -o local -m32 local.s local-lib.s
./local > local.out
diff local.out examples/local.out

./scc < examples/putchar.c > putchar.s
gcc -o putchar -m32 putchar.s
./putchar > putchar.out
diff putchar.out examples/putchar.out

./scc < examples/towers.c > towers.s
gcc -m32 -S -O0 -c examples/towers-lib.c
gcc -o towers -m32 towers.s towers-lib.s
./towers > towers.out
diff towers.out examples/towers.out
