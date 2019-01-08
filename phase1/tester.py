import os

lexan_filename = 'lexan.cpp'

test_filepath = 'examples/'
verified_filepath = 'diffs/'

test_filenames = [
	'fib',
	'hello',
	'list',
	'malloc',
	'real',
	'sum',
	'tricky'
]

test_file_extension = '.c'
verify_file_extension = '.out'

print("Making...")
os.system('Make')

verify_files = [test_filepath + filename + verify_file_extension for filename in test_filenames]


for filename in test_filenames:
	test_filename = test_filepath + filename + '.c'
	result_filename = verified_filepath + filename + '_r.out'

	os.system('./scc < '+test_filename+' > '+result_filename)

	os.system('diff ' + verified_filepath+filename+'_r.out '+verified_filepath+filename+'.out > '+'results/'+filename+'_d.txt')




