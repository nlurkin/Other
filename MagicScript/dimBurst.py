#!/bin/env python
# encoding: utf-8
'''
Created on 2 May 2014

@author: ncl
'''

import sys, time
import pydim

class dimBurst(object):
	'''
	classdocs
	'''

	inBurst = False
	serverName = None
	
	EOB = None
	SOB = None
	
	handler = None
	enable_handler = False
	
	def __init__(self,params):
		'''
		Constructor
		'''
		self.serverName = params[0]
		self.handler = params[1]
		
	def eob_callback(self, param):
                if self.FirstEOB:
                        self.FirstEOB = False
                        return
		print "Received new EOB value : " + str(param) 
		self.inBurst = False
		time.sleep(1)
		if self.enable_handler:
			self.enable_handler = False
			self.handler()
	
	def sob_callback(self, param):
                if self.FirstSOB:
                        self.FirstSOB = False
                        return
		print "Received new SOB value : " + str(param)
		self.inBurst = True 

	def activateHandler(self):
		self.enable_handler = True
	
	def fired(self):
		return not self.enable_handler
		
	def start(self):
                self.FirstSOB = True 
                self.FirstEOB = True 
		self.SOB = pydim.dic_info_service(self.serverName + "/SOB", "L:1", self.sob_callback)
		self.EOB = pydim.dic_info_service(self.serverName + "/EOB", "L:1", self.eob_callback)
		
#		while True:
#			time.sleep(5)
		
	def stop(self):
		pydim.dic_release_service(self.SOB)
		pydim.dic_release_service(self.EOB)

def nothing():
        print "Nothing"

if __name__ == "__main__":
	d = dimBurst(["BhamLab/Timing",nothing])
	d.start()
	while True:
		time.sleep(5)
	
