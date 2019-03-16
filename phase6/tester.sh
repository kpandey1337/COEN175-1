#!/bin/bash

 ./scc < examples/double.c > double.s
gcc -o double -m32 double.s
./double < examples/double.in > double.out
diff double.out examples/double.out

./scc < examples/fib.c > fib.s
gcc -o fib -m32 fib.s
./fib < examples/fib.in > fib.out
diff fib.out examples/fib.out

./scc < examples/global.c > global.s
gcc -o global -m32 global.s
./global < examples/global.in > global.out
diff global.out examples/global.out

./scc < examples/hello.c > hello.s
gcc -o hello -m32 hello.s
./hello < examples/hello.in > hello.out
diff hello.out examples/hello.out

./scc < examples/int.c > int.s
gcc -o int -m32 int.s
./int < examples/int.in > int.out
diff int.out examples/int.out

./scc < examples/matrix.c > matrix.s
gcc -o matrix -m32 matrix.s
./matrix < examples/matrix.in > matrix.out
diff matrix.out examples/matrix.out

./scc <  examples/mixed.c > mixed.s
gcc -o mixed -m32 mixed.s
./mixed < examples/mixed.in > mixed.out
diff mixed.out examples/mixed.out

./scc < examples/tree.c > tree.s
gcc -o tree -m32 tree.s
./tree < examples/tree.in > tree.out
diff tree.out examples/tree.out
