from os import walk
from os import system

f = []
for (dirpath, dirnames, filenames) in walk("/Users/pedro/coen/school/COEN175/phase3/examples"):
	f.extend(filenames)
	break

raw_filenames = []
for filename in f:
<<<<<<< HEAD
	if(filename[-3:] != "err"):
		#print(filename)
		raw_filenames.append(filename.replace(".c",""))
=======
	if filename[-3:] != "err":
		raw_filenames.append(filename.replace(".c",""))

>>>>>>> master

#Make -all
print("Making...")
system("make -C code/")

for filename in raw_filenames:
	system_call = "./scc < examples/" + filename + ".c 2> results/" + filename + ".txt"

	print("====\nTesting: ",filename)
	system(system_call)

	diff_call = "diff results/" + filename + ".txt examples/" + filename + ".err > diffs/" + filename + "_diff.txt"

	print("\n\tDiffing: ",filename)
	system(diff_call)




