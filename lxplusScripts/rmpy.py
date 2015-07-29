#!/usr/bin/python2.6
import os
import sys
import curses

def printStat(currFile, fileNumber, total, stdscr):
	progress = (1.0*fileNumber/total)*100
	
	"""progress: 0-100"""
	stdscr.addstr(0, 0, "Deleting file: {0}                                 ".format(currFile))
	stdscr.addstr(1, 5, "Total progress: [{2:101}] {0}/{1}".format(fileNumber,total, "#" * int(progress)))
	stdscr.refresh()

def listdir(path, count, fileList, dirList):
	ls = os.listdir(path)
	
	for f in ls:
		if os.path.isfile("%s/%s" % (path,f)):
			fileList.append("%s/%s" % (path,f))
			count = count+1
		elif os.path.isdir("%s/%s" % (path,f)):
			dirList.append("%s/%s" % (path,f))
			count = count+1
		
	return [count,fileList, dirList]

def delete(fileList, dirList, total, stdscr):
	i=0
	dirList.reverse()
	for f in fileList:
		printStat(f, i, total, stdscr)
		os.remove(f)
		i = i+1
	
	for d in dirList:
		printStat(d, i, total, stdscr)
		os.rmdir(d)
		i = i+1

def execute():
	#get list of arguments
	argsList = sys.argv[1:]
	
	fileList = []
	dirList = []
	scannedDirs = []
	total = 0
	
	for f in argsList:
		#Le path existe-il?
		if not os.path.exists(f):
			print "%s does not exist" % (f)
		
		#File or dir?
		if os.path.isdir(f):
			dirList.append(f.rstrip('/'))
			total = total + 1
		elif os.path.isfile(f):
			fileList.append(f.rstrip('/'))
			total = total + 1
	
	while len(dirList)>0:
		d = dirList.pop(0)
		[total, fileList, dirList] = listdir(d, total, fileList, dirList)
		scannedDirs.append(d)
	
	stdscr = curses.initscr()
	curses.noecho()
	curses.cbreak()
	
	try:
		delete(fileList, scannedDirs, total, stdscr)
	finally:
		curses.echo()
		curses.nocbreak()
		curses.endwin()		

if __name__ == '__main__':
	#Tester les arguments
	if len(sys.argv)<=1:
		print "Missing arguments"
		sys.exit(0)
	
	execute()

