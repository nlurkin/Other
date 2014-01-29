#!/usr/bin/env python
import sys
import os

condor_header="""
####################
##
## .condor file 
##
####################

Universe	= vanilla
Executable	= %s

requirements = (NA62 =?= TRUE)||(CMSFARM =?= TRUE)

should_transfer_files   = YES
when_to_transfer_output = ON_EXIT


"""

process_template="""
#
# Process %s
#
Error           = condor/err.%s
Log             = condor/log.%s
Output          = condor/out.%s
Arguments       = "%s '%s' %s %s"
Queue 1
"""

def gen(example, path, exe, opt):
	example = os.path.abspath(example)

	path = path.rstrip("/")
	path = os.path.abspath(path)
	
	dirList=os.listdir(path)
	flist = []
	
	for fname in dirList:
		if fname.endswith(".root"):
			flist.append(fname)
	
	print condor_header % (exe)
	
	j=1
	stringList = ""
	for f in flist:
		stringList = "%s %s/%s" % (stringList, path,f)
		if (j % 10)==0:
			print process_template % (j, j, j, j, example, stringList, "500", opt)
			stringList = ""
		j = j + 1
	
	

if __name__ == '__main__':
	if len(sys.argv)==1:
		print "Send jobs on Ingrid"
		print "Arguments : example dir shellScript"
		sys.exit(0)	

	example = sys.argv[1]
	path = sys.argv[2]
	exe = sys.argv[3]
	if len(sys.argv)==5:
		opt = sys.argv[4]
	else:
		opt = ""

	gen(example, path, exe,opt)
