#!/bin/env python

from multiprocessing import Process, Queue
import os
import sys


def workerList(inputs, output):
	for fd, q in iter(inputs.get, 'STOP'):
		result = listdirAsync(fd, q)
		output.put(result)
		
def listdirAsync(path, queue):
	ls = os.listdir(path)
	
	fileList = []
	dirList = []
	
	for f in ls:
		if os.path.isfile("%s/%s" % (path,f)):
			fileList.append("%s/%s" % (path,f))
			queue.put(1)
		elif os.path.isdir("%s/%s" % (path,f)):
			dirList.append("%s/%s" % (path,f))
			queue.put(1)
		
	return (fileList, dirList)


def run():
	argsList = sys.argv[1:]
	processes = 2
	
	task_queue = Queue()
	result_queue = Queue()
	count_queue = Queue()
	
	dirList = []
	fileList = []
	scannedDirs = []
	total = 0
	finished = False
	startedProc = 0
	finishedProc = 0
	
	#Prepare the arguments
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
			
	print dirList
	##Start the pool
	for _ in range(processes):
		Process(target=workerList, args=(task_queue, result_queue)).start()
		
	while not finished:
		if len(dirList>0):
			d = dirList.pop(0)
			startedProc += 1
			task_queue.put((d, count_queue))
			print "Starting process on %s" % (d)
		
		if not result_queue.empty():
			(finishedDir, fList, dList) = result_queue.get()
			fileList.extend(fList)
			dirList.extend(dList)
			scannedDirs.append(finishedDir)
			finishedProc += 1
			print "finished process on %s" % finishedDir
		
		if startedProc==finishedProc and len(dirList==0):
			finished = True
	
	
	print fileList
	print dirList
	
	
	
if __name__ == '__main__':
	#Tester les arguments
	if len(sys.argv)<=1:
		print "Missing arguments"
		sys.exit(0)
	
	run()

