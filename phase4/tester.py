from os import walk
from os import system

f = []
for (dirpath, dirnames, filenames) in walk("/Users/pedro/coen/school/COEN175/phase4/examples"):
	f.extend(filenames)
	break

raw_filenames = []
for filename in f:
	if filename[-3:] != "err" and filename != ".DS_Store":
		raw_filenames.append(filename.replace(".c",""))


#Make -all
print("Making...")
system("make -C code/")

for filename in raw_filenames:
	system_call = "code/./scc < examples/" + filename + ".c 2> results/" + filename + ".txt"

	print("====\nTesting: ",filename)
	#print(system_call)
	system(system_call)

	diff_call = "diff results/" + filename + ".txt examples/" + filename + ".err > diffs/" + filename + "_diff.txt"

	print("\n\tDiffing: ",filename)
	#print(diff_call)
	system(diff_call)




