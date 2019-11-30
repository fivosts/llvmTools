#!/usr/bin/python2
import sys,os
import argparse, subprocess

LLVM_BUILD_PATH = "/home/fivosts/Desktop/LLVM/build"

if not os.path.isdir(LLVM_BUILD_PATH):
	LLVM_BUILD_PATH = "/mnt/EEE896A2E896691B/LLVM/build"

if __name__ == "__main__":

	parser = argparse.ArgumentParser(description = 'Small framework for importing LLVM passes to code')
	Required_args = parser.add_argument_group('Required Arguments')
	Required_args.add_argument('-i', '--ir', help = "Path for the ll/bc file of the input code", required = True)
	Required_args.add_argument('-p', '--Pass', help = "Name of the pass to be executed", required = True)
	Required_args.add_argument('-a', '--arg', help = "Input argument of the pass", required = True)
	Optional_args = parser.add_argument_group('Optional Arguments')
	Optional_args.add_argument('-t', '--time', help = "Time benchmarking of pass used")
	Optional_args.add_argument('-d', '--debug', help = "Debug pass flag for opt")
	Optional_args.add_argument('-e', '--execute', help = "Flag for compiling and executing optimized source")
	args = parser.parse_args()

	delete_me = []
	
	ir_file = args.ir
	if args.ir.split('.')[-1] == "ll":
		process = subprocess.Popen((LLVM_BUILD_PATH + "/bin/llvm-as " + args.ir).split(), stdout=subprocess.PIPE)
		output, error = process.communicate()
		if error != None:
			print "Error converting the ir file to bitcode"
			sys.exit(1)
		s = list(ir_file)
		s[-2] = "b"
		s[-1] = "c"
		ir_file = "".join(s)
		print ir_file
		delete_me.append(ir_file)
	##elif source code TODO

	out_file = ir_file.split('/')
	out_file[-1] = "opt-" + out_file[-1]
	out_file = "/".join(out_file)

	print 'Executing ...:'
	print LLVM_BUILD_PATH + "/bin/opt -load " + LLVM_BUILD_PATH + "/lib/" + str(args.Pass) + " -" + str(args.arg) + " " + ir_file + " -o " + out_file
	print ''

	process = subprocess.Popen((LLVM_BUILD_PATH + "/bin/opt -load " + LLVM_BUILD_PATH + "/lib/" + str(args.Pass) + " -" + str(args.arg) + " " + ir_file + " -o " + out_file).split(), stdout=subprocess.PIPE)
	output, error = process.communicate()
	print output
	if error != None:
		print "Error loading the pass"
		sys.exit(1)

	process = subprocess.Popen((LLVM_BUILD_PATH + "/bin/llvm-dis " + out_file).split(), stdout=subprocess.PIPE)
	output, error = process.communicate()
	if error != None:
		print "Error converting the optimized bitcode to readable file"
		sys.exit(1)

	delete_me.append(out_file)

	for file in delete_me:
		process = subprocess.Popen(("rm " + file).split(), stdout=subprocess.PIPE)
		output, error = process.communicate()
		if error != None:
			print "Error deleting bitcode file"
			sys.exit(1)