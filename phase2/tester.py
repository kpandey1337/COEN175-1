from os import walk
from os import system

f = []
for (dirpath, dirnames, filenames) in walk("/Users/pedro/coen/school/COEN175/phase2/examples"):
	f.extend(filenames)
	break

raw_filenames = []
for filename in f:
	raw_filenames.append(filename.replace(".c",""))


for filename in raw_filenames:
	system_call = "./scc 0 < examples/" + filename + ".c > results/" + filename + ".txt"

	print("====\nTesting: ",filename)
	system(system_call)

	diff_call = "diff results/" + filename + ".txt results/" + filename + ".out > diffs/" + filename + "_diff.txt"

	system(diff_call)

