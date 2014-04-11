#!/bin/env python
# encoding: utf-8
'''
MagicScript -- shortdesc

MagicScript is a description

It defines classes_and_methods

@author:     Nicolas Lurkin
@contact:    nicolas.lurkin@cern.ch
@deffield    updated: Updated
'''

from argparse import ArgumentParser, RawDescriptionHelpFormatter
import os
import socket
import sys
import time
import subprocess
from Cheetah.Template import Template
from os import getenv
import signal



__all__ = []
__version__ = 0.2
__date__ = '2014-04-09'
__updated__ = '2014-04-11'

class CLIError(Exception):
	'''Generic exception to raise and log different fatal errors.'''
	def __init__(self, msg):
		super(CLIError).__init__(type(self))
		self.msg = "E: %s" % msg
	def __str__(self):
		return self.msg
	def __unicode__(self):
		return self.msg

class MagicScriptError(Exception):
	def __init__(self, value):
		self.value = value
	def __str__(self):
		return repr(self.value)

def mapPMs(fileName, datasheet):
	
	try:
		with open(fileName, 'r') as fd:
			pms = list(fd)
	except IOError as e:
		print "Unable to open PM map file ('{0}'): {1}".format(fileName, e.strerror)
		raise(MagicScriptError("Fatal"))
	
	pmMap = dict([[x, {'GeoPos':int(y)}] for x, y in [x.rstrip('\n').split(' ') for x in pms]])
	
	try:
		with open(datasheet, 'r') as fd:
			pms = list(fd)
	except IOError as e:
		print "Unable to open PM datasheet file ('{0}'): {1}".format(datasheet, e.strerror)
		raise(MagicScriptError("Fatal"))				
		
	pms.pop(0)
	pmsData = [[snum, {"CatLum":float(cat_lum), "AnLum":float(an_lum), "DarkCurr":float(d_curr), "BlueSens":float(b_sens), "Gain":float(gain)}] for 
			snum, cat_lum, an_lum, d_curr, b_sens, gain in [x.strip('\n').split(' ') for x in pms]]
	
	for pm in pmsData:
		if pmMap.has_key(pm[0]):
			pmMap[pm[0]].update(pm[1]) 
	
	return pmMap

def parseDaqOutput(daq, args):
	nEvents = 0
	nTriggers = 0
	nBursts = 0
	
	daqResults = []
	currIndex = -1;
	
	lastFileIndex = 0;
	
	for line in daq.stdout:
		line = line.rstrip('\n')
		if line.startswith("Opening"):
			currFile = line[line.find("/"):]
			currIndex = currIndex + 1
			daqResults.append({"File":currFile})
		if line.startswith('Burst'):
			daqResults[currIndex]['BurstNum'] = int(line[line.find("(") + 1:line.find(')')])
			nBursts += 1
		if line.startswith('NEventsPerBurst'):
			daqResults[currIndex]['NTriggers'] = int(line.split('=')[1])
			nTriggers += daqResults[currIndex]['NTriggers']
		if line.startswith('NGoodEvents'):
			daqResults[currIndex]['NEvent'] = int(line.split('=')[1])
			nEvents += daqResults[currIndex]['NEvent']
	
		if nEvents >= int(args.nEvents) and not lastFileIndex == 0:
			lastFileIndex = currIndex
		
		if currIndex > lastFileIndex and currIndex > 2:
			break
	
	return daqResults
	
def rundaq(args):
	if not args.dryRun:
		try:
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			sock.connect(("localhost", 41988))
		except socket.error as e:
			print "Unable to establish communication with tdspy [Error {0}]: {1}".format(e.errno, e.strerror)
			raise(MagicScriptError("Fatal"))
		
	try:
		daq = subprocess.Popen("./na62daq", stdout=subprocess.PIPE)
	except OSError as e:
		print "Unable to run na62daq [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))		
	
	if not args.dryRun:
		raw_input("Press enter to start run (at least 1s before next SOB)")
		sock.sendall("@startrun2.spy")
		sock.recv(1024, 0)
		
	daqResults = parseDaqOutput(daq, args)
	
	if not args.dryRun:
		sock.sendall("@endrun2.spy")
		sock.recv(1024, 0)
	
	daq.kill()
	if not args.dryRun:
		sock.close()
	
	return daqResults

def runReco(daqResults, listFile, recoFile, NA62RecoPath):
	try:
		fd = open(listFile, 'w')
		for f in [x['File'] for x in daqResults[1:-1]]:
			fd.write(f + "\n")
	
		fd.close()
	except IOError as e:
		print "Unable to open run file list ('{0}'): {1}".format(listFile, e.strerror)
		raise(MagicScriptError("Fatal"))

	savedPath = os.getcwd()
	os.chdir(NA62RecoPath)	
	try:
		print ' '.join(["./NA62Reco", "-l", savedPath+"/"+listFile, "-o", savedPath+"/"+recoFile])
		reco = subprocess.Popen(["./NA62Reco", "-l", savedPath+"/"+listFile, "-o", savedPath+"/"+recoFile])
		print "Running reconstruction... please wait"
		reco.wait()
		print "Reconstruction finished"
	except OSError as e:
		print "Unable to run NA62Reco [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))
	finally:
		os.chdir(savedPath)		


def timeout(sig,frm):
	raise(signal.ItimerError("Too Long"))

def runAnalysis(recoFile, anaFile, NA62AnalysisPath):
	try:
		print ' '.join([NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile])
		analysis = subprocess.Popen([NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile, "-g"], stdout=subprocess.PIPE)
		print "Running analysis... please wait"
		answer = None
		signal.signal(signal.SIGALRM, timeout)
		signal.alarm(3)
		for line in analysis.stdout:
			signal.alarm(3)
			if "Analysis complete" in line:
				print "Analysis complete ..."
				answer = raw_input("Are the plots ok? [y/n] ")
				break;
			if "Bye!" in line:
				print "Analysis failed ..."
				raise(MagicScriptError("Fatal"))
	
	except OSError as e:
		print "Unable to run LightBoxTest [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))
	except signal.ItimerError as e:
		print "Analysis timeout ... expecting plots to be displayed now"
		answer = raw_input("Are the plots ok? [y/n] ")
	
	if answer==None:
		print "Analysis failed ..."
		raise(MagicScriptError("Fatal"))
		
	analysis.kill()
	return answer

def GenerateTex(pmMap, texPath):
	pmGeoPos = pmMap[pmMap.keys()[0]]['GeoPos']
	shortPath = texPath[texPath.find("/")+1:]
	searchList= [{'pmSN':pmMap.keys()[0], 'pm':pmMap[pmMap.keys()[0]], 'SlewCorr':shortPath+'/SlewPlot'+str(pmGeoPos)+'.pdf', 'TRes':shortPath+'/TRes'+str(pmGeoPos)+'.pdf'}]
	t = Template(file='PMTemplate.tex', searchList=searchList)
	
	try:
		fd = open(texPath+"/PM"+str(pmGeoPos)+".tex", 'w')
		fd.writelines(str(t))
		fd.close()
	except IOError as e:
		print "Unable to open PM tex file ('{0}'): {1}".format(texPath+"/PM"+str(pmGeoPos)+".tex", e.strerror)
		raise(MagicScriptError("Fatal"))
	
	return "\input{"+shortPath+"/PM"+str(pmGeoPos)+"}\n"
	

def process(args):

	args.LightBox = "LightBox"+str(args.lightBoxNumber)
	
	lightBoxDir = "results/"+args.LightBox+"/"
	listFile = lightBoxDir+args.LightBox+".list"
	recoFile = lightBoxDir+args.LightBox+".root"
	anaFile = lightBoxDir+args.LightBox+"Test.root"
	extractDir = lightBoxDir+"tex"
	texPath = lightBoxDir+"tex"
	texFile = texPath+"/"+args.LightBox+".tex"
	
	
	if getenv("NA62RECOSOURCE") == None:
		print "Missing NA62RECOSOURCE environment variable"
		raise(MagicScriptError("Fatal"))
	if getenv("NA62ANALYSIS") == None:
		print "Missing NA62ANALYSIS environment variable"
		raise(MagicScriptError("Fatal"))
	
	NA62Reco = getenv("NA62RECOSOURCE")
	NA62Analysis = getenv("NA62ANALYSIS")

	if not os.path.exists("results"):
		os.mkdir("results")
	
	if not os.path.exists(lightBoxDir):
		os.mkdir(lightBoxDir)
	
	if not os.path.exists(texPath):
		os.mkdir(texPath)
	
	pmMap = mapPMs(args.mapFile, "datasheet.dat")
	
	daqResults = rundaq(args)
	
	runReco(daqResults, listFile, recoFile, NA62Reco)
	
	answer = runAnalysis(recoFile, anaFile, NA62Analysis)
	
	if answer.lower() == 'n':
		return
	
	
	lightBoxContent = ""
	
	for pm in pmMap:
		try:
			extract = subprocess.Popen(["./ExtractPlots", "-n", str(pmMap[pm]['GeoPos']), "-i", recoFile, "-I", anaFile, "-o", extractDir])
			extract.wait()
		except OSError as e:
			print "Unable to run ExtractPlots [Error {0}]: {1}".format(e.errno, e.strerror)
			raise(MagicScriptError("Fatal"))
		lightBoxContent += GenerateTex(pmMap, texPath)
	
	t = Template(file='LightBoxTemplate.tex', searchList=[{'lightBoxNumber':args.lightBoxNumber, 'pmList':lightBoxContent}])
	try:
		fd = open(texFile, 'w')
		fd.writelines(str(t))
		fd.close()
	except IOError as e:
		print "Unable to open LightBox tex file ('{0}'): {1}".format(texFile, e.strerror)
		raise(MagicScriptError("Fatal"))

def makeLatex(args):
	path = "results"
	lightboxes = os.listdir(path)
	texFiles = []
	print lightboxes
	for l in lightboxes:
		if os.path.isdir(os.path.join(path,l)):
			texFiles += [l+"/tex/"+x for x in os.listdir(os.path.join(path,l+"/tex")) if (x[-4:]==".tex" and "LightBox" in x)]
	
	
	print texFiles
	
	lightBoxList = ""
	for f in texFiles:
		lightBoxList += "\input{"+f+"}\n"
		
	t = Template(file='BookTemplate.tex', searchList=[{'lightBoxList':lightBoxList}])
	try:
		fd = open(path+"/PMBook.tex", 'w')
		fd.writelines(str(t))
		fd.close()
	except IOError as e:
		print "Unable to open Book tex file ('{0}'): {1}".format("PMBook.tex", e.strerror)
		raise(MagicScriptError("Fatal"))

	os.chdir(path)
	try:
		latex = subprocess.Popen(["pdflatex", "PMBook.tex"])
		latex = subprocess.Popen(["pdflatex", "PMBook.tex"])
		latex.wait()
	except OSError as e:
		print "Unable to run pdflatex [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))
	
	
	
	
def main(argv=None):  # IGNORE:C0111
	'''Command line options.'''
	
	if argv is None:
		argv = sys.argv
	else:
		sys.argv.extend(argv)

	program_name = os.path.basename(sys.argv[0])
	program_version = "v%s" % __version__
	program_build_date = str(__updated__)
	program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
	program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
	program_license = '''%s

	Created by Nicolas Lurkin on %s.

USAGE
''' % (program_shortdesc, str(__date__))

	try:
		# Setup argument parser
		parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)
		parser.add_argument('-V', '--version', action='version', version=program_version_message)
		subparsers = parser.add_subparsers()
		parser_run = subparsers.add_parser('run', help='Run the acquisition for a lightbox')
		parser_run.set_defaults(func=process)
		parser_run.add_argument("-m", "--mapfile", dest="mapFile", required=True, help="Path to file containing PM geo position in the box (SN Pos)")
		parser_run.add_argument("-n", "--nevents", dest="nEvents", required=True, help="Number of events required")
		parser_run.add_argument("-l", "--lightbox", dest="lightBoxNumber", type=int, required=True, help="LightBox number currently being tested")
		parser_run.add_argument("-d", "--dryrun", dest="dryRun", action="store_true", help="Don't try to establish socket connection with tel62")
		parser_finalize = subparsers.add_parser('finalize', help='Compile the full pdf book for all tested lightboxes')
		parser_finalize.set_defaults(func=makeLatex)
		
		# Process arguments
		args = parser.parse_args()
		
	except KeyboardInterrupt:
		### handle keyboard interrupt ###
		return 0
	except Exception, e:
		indent = len(program_name) * " "
		sys.stderr.write(program_name + ": " + repr(e) + "\n")
		sys.stderr.write(indent + "  for help use --help")
		return 2
	
	try:
		args.func(args)
	except MagicScriptError as e:
		print "\nScript exiting with error: {0}".format(e.value)


if __name__ == "__main__":
	sys.exit(main())
