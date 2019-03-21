import os

os.system("make clean all -C code")
os.system("cp code/scc manual_testing/")
os.system("./manual_testing/scc < manual_testing/t1.c")
