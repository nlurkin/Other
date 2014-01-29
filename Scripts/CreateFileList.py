#!/bin/env python
import subprocess
import sys
import re
from optparse import OptionParser


expr = re.compile(".*?(?:/castor/cern.ch/|/eos/na62/)(.*)")

def bash_command(cmd):
	p = subprocess.Popen(cmd, shell=True, executable='/bin/bash', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	(out, err) = p.communicate()
	if len(err)>0:
		print err
	return out

def getOldCastor(path):
	cmd = "nsls %s" % path
	
	entries = bash_command(cmd).split()
	return [x for x in entries if x[-5:]==".root"]

def getShortPath(path):
	result = ""
	m = expr.match(path)
	if m:
		result = m.group(1)
	return result
	
if __name__ == "__main__":
	parser = OptionParser()
	parser.add_option("-c", "--castor", action="store_const", dest="format", const="castor")
	parser.add_option("-e", "--eos", action="store_const", dest="format", const="eos")
	parser.add_option("-x", "--xroot", action="store_const", dest="format", const="xroot")
	
	(options, args) = parser.parse_args()
	options = vars(options)
	
	if len(args)!= 1:
		print "You need to provide a search path"
		sys.exit(0)
	path = args[0]
	
	prefix = ""
	if options['format']=="castor":
			prefix = "/castor/cern.ch"
	elif options['format']=="xroot":
			prefix = "xroot://castorpublic.cern.ch//castor/cern.ch"
	elif options['format']=="eos":
			prefix = "eos"
	elif options['format']== None:
		print "Please specify which output format you would like"
		sys.exit(0)

	files = sorted(getOldCastor(path))
	path = getShortPath(path).strip("/")

	for f in files:
		print "%s/%s/%s" % (prefix, path, f)
		
