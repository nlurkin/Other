#!/bin/env python
import subprocess
import sys

def bash_command(cmd):
	p = subprocess.Popen(cmd, shell=True, executable='/bin/bash', stdout=subprocess.PIPE)
	out = p.communicate()
	return out

def getOldCastor(path):
	cmd = "nsls %s" % path
	print bash_command(cmd)

if __name__ == "__main__":
	path = sys.argv[1]