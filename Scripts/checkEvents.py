#!/bin/env python

'''
Created on 4 Jul 2014

@author: ncl
'''

import sys

if __name__ == "__main__":
	
	hasRef = False
	
	if len(sys.argv) == 0:
		print "You need to provide check and/or reference"
		sys.exit(0)
	
	pathCheck = sys.argv[0]
	if len(sys.argv)==2:
		pathRef = sys.argv[1]
		hasRef = True
	
	listCheck = {"id":[], "file":[], "read":[], "total":[], "sel":[]}
	
	with open(pathCheck, "r") as fd:
		line = fd.readline()
		l = line.split(" ")
		listCheck["id"].append(l[0])
		listCheck["file"].append(l[1])
		listCheck["read"].append(l[2])
		listCheck["total"].append(l[3])
		listCheck["sel"].append(l[4])
	
	print listCheck
	
