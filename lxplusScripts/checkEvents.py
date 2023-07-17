#!/bin/env python

'''
Created on 4 Jul 2014

@author: ncl
'''

import sys

if __name__ == "__main__":
	
	hasRef = False
	
	if len(sys.argv) == 1:
		print "You need to provide check and/or reference"
		sys.exit(0)
	
	pathCheck = sys.argv[1]
	if len(sys.argv)==3:
		pathRef = sys.argv[2]
		hasRef = True
	
	listCheck = {"id":[], "file":[], "read":[], "total":[], "sel":[]}
	
	with open(pathCheck, "r") as fd:
		for line in fd:
			line = line.rstrip("\n")
			l = line.split(" ")
			listCheck["id"].append(int(l[0]))
			listCheck["file"].append(l[1])
			if len(l)==5:
				listCheck["read"].append(int(l[2]))
				listCheck["total"].append(int(l[3]))
				listCheck["sel"].append(int(l[4]))
			else:
				listCheck["read"].append(0)
				listCheck["total"].append(int(l[2]))
				listCheck["sel"].append(int(l[3]))				
	
	if hasRef:
		listRef = {"id":[], "file":[], "read":[], "total":[], "sel":[]}
		
		with open(pathRef, "r") as fd:
			for line in fd:
				line = line.rstrip("\n")
				l = line.split(" ")
				listRef["id"].append(int(l[0]))
				listRef["file"].append(l[1])
				if len(l)==5:
					listRef["read"].append(int(l[2]))
					listRef["total"].append(int(l[3]))
					listRef["sel"].append(int(l[4]))
				else:
					listRef["read"].append(0)
					listRef["total"].append(int(l[2]))
					listRef["sel"].append(int(l[3]))				
	
	if hasRef:
		#check coherence between ref and check
		if len(listCheck["id"]) != len(listRef["id"]):
			print "Different number of files read"
		else:
			for i in range(0,len(listCheck["id"])):
				error = False
				errorID = ["","","","",""]
				if listCheck["id"][i] != listRef["id"][i]:
					error = True
					errorID[0] = "->"
				if listCheck["file"][i] != listRef["file"][i]:
					error = True
					errorID[1] = "->"
				if listCheck["read"][i] != listRef["read"][i]:
					error = True
					errorID[2] = "->"
				if listCheck["total"][i] != listRef["total"][i]:
					error = True
					errorID[3] = "->"
				if listCheck["sel"][i] != listRef["sel"][i]:
					error = True
					errorID[4] = "->"

				
				if error:
					print "Error for job %i" % (listCheck["id"][i]);
					print "\t\t\tExpected: \t\t\t\t\t Got:"
					print "%s\tid:\t\t%i \t\t\t\t\t\t %i" % (errorID[0], listRef["id"][i], listCheck["id"][i])
					print "%s\tfile:\t\t%s \t\t %s" % (errorID[1], listRef["file"][i].split("/")[-1], listCheck["file"][i].split("/")[-1])
					print "%s\tRead:\t\t%i \t\t\t\t\t %i" % (errorID[2], listRef["read"][i], listCheck["read"][i])
					print "%s\tTotal:\t\t%i \t\t\t\t\t %i" % (errorID[3], listRef["total"][i], listCheck["total"][i])
					print "%s\tSel:\t\t%i \t\t\t\t\t\t %i" % (errorID[4], listRef["sel"][i], listCheck["sel"][i])
					print ""
		
		print "Ref file"
		print "Files read: \t\t%i" % (len(listRef["id"]))
		print "Total events read: \t%i" % (sum(listRef["read"]))
		print "Total events processed: %i" % (sum(listRef["total"]))
		print "Total events selected: \t%i" % (sum(listRef["sel"]))
	
	print ""
	print "Checked file"
	print "Files read: \t\t%i" % (len(listCheck["id"]))
	print "Total events read: \t%i" % (sum(listCheck["read"]))
	print "Total events processed: %i" % (sum(listCheck["total"]))
	print "Total events selected: \t%i" % (sum(listCheck["sel"]))
	
