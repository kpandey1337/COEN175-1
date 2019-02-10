from os import walk
from os import system

f = []
for (dirpath, dirnames, filenames) in walk("/Users/pedro/coen/school/COEN175/phase3/examples"):
	f.extend(filenames)
	break

raw_filenames = []
for filename in f:
	raw_filenames.append(filename.replace(".c",""))

#Make -all
print("Making...")
system("make -C code/")

for filename in raw_filenames:
	system_call = "./scc < examples/" + filename + ".c > results/" + filename + ".txt"

	print("====\nTesting: ",filename)
	system(system_call)

	diff_call = "diff results/" + filename + ".txt examples/" + filename + ".err > diffs/" + filename + "_diff.txt"

	system(diff_call)

