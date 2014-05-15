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

from Cheetah.Template import Template
from argparse import ArgumentParser, RawDescriptionHelpFormatter
from os import getenv
from dimBurst import dimBurst
import fcntl
import os
import socket
import subprocess
import sys
import time



__all__ = []
__version__ = 0.2
__date__ = '2014-04-09'
__updated__ = '2014-04-11'


channels = [1, 2]

scanVoltages = {1:[x for x in range(0,10)], 
			2:[x for x in range(0,10)]}
defaultVoltages = {1:1, 2:2}

scanThresholds = {1:[x/10. for x in range(0,10)], 
				2:[x/20. for x in range(0,10)]}
defaultThresholds = {1:0.1, 2:0.05}


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

class Answer(object):
	Bool = 'n'
	Data = 0
	def __init__(self):
		pass

tdSpy_Sock = None

def startRun():
	global tdSpy_Sock
	tdSpy_Sock.sendall("@startrun2.spy")
	
dimClient = dimBurst(["BhamLab/Timing", startRun])

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
	while True:
		while True:
			try:
				line = daq.stdout.readline()
			except IOError:
				continue
			else:
				break
		line = line.rstrip('\n')
		print line
		if line.startswith("Opening"):
			currFile = line[line.find("/"):]
			currIndex = currIndex + 1
			daqResults.append({"File":currFile})
		if line.startswith('Burst'):
			daqResults[currIndex]['BurstNum'] = int(line[line.find("(") + 1:line.find(')')])
			nBursts += 1
		if line.startswith('NEventsPerBurst'):
			daqResults[currIndex]['NTriggers'] = int(line.split('=')[1])
			if not currIndex == 0:
				nTriggers += daqResults[currIndex]['NTriggers']
		if line.startswith('NGoodEvents'):
			daqResults[currIndex]['NEvent'] = int(line.split('=')[1])
			if not currIndex == 0:
				nEvents += daqResults[currIndex]['NEvent']
	
		if nTriggers >= int(args.nEvents) and lastFileIndex == 0:
			lastFileIndex = currIndex
		
		if currIndex > lastFileIndex and currIndex > 2 and not lastFileIndex == 0:
			break
	
	return daqResults
	
def setNonBlocking(fd):
	"""
	Set the file description of the given file descriptor to non-blocking.
	"""
	flags = fcntl.fcntl(fd, fcntl.F_GETFL)
	flags = flags | os.O_NONBLOCK
	fcntl.fcntl(fd, fcntl.F_SETFL, flags)
	
def rundaq(args):
	global tdSpy_Sock
	global dimClient
	
	if not args.dryRun:
		try:
			tdSpy_Sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			tdSpy_Sock.connect(("tel62", 41988))
		except socket.error as e:
			print "Unable to establish communication with tdspy [Error {0}]: {1}".format(e.errno, e.strerror)
			raise(MagicScriptError("Fatal"))
		tdSpy_Sock.settimeout(1)
		
	try:
		#daq = subprocess.Popen("./na62daq", stdout=subprocess.PIPE)
		daq = subprocess.Popen(["/usr/bin/stdbuf", "-o", "0", "./na62daq"], stdout=subprocess.PIPE)
	except OSError as e:
		print "Unable to run na62daq [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))		
	setNonBlocking(daq.stdout)

	if not args.dryRun:
		#if not args.autoRun:
		#	raw_input("Press enter to start run (at least 1s before next SOB)")
		#tdSpy_Sock.sendall("@startrun2.spy")
		#startRun()
		dimClient.activateHandler()
		while not dimClient.fired():
			time.sleep(0.5)
		
		while True:
			try:
				data = tdSpy_Sock.recv(4096, 0)
				print data,
				if not data: break
			except IOError:
				break
		print '\033[0m'
		
	daqResults = parseDaqOutput(daq, args)
	
	if not args.dryRun:
		dimClient.stop()
		tdSpy_Sock.sendall("@endrun2.spy")
		data = tdSpy_Sock.recv(1024, 0)
		while True:
			try:
				data = tdSpy_Sock.recv(4096, 0)
				print data,
				if not data: break
			except IOError:
				break
		print '\033[0m'
	
	if not args.dryRun:
		tdSpy_Sock.close()
	
	daq.kill()
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
		cmd = ["./NA62Reco", "-b", "100", "-l", listFile, "-o", recoFile]
		print ' '.join(cmd)
		reco = subprocess.Popen(cmd)
		print "Running reconstruction... please wait"
		reco.wait()
		print "Reconstruction finished"
	except OSError as e:
		print "Unable to run NA62Reco [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))
	finally:
		os.chdir(savedPath)		

def runAnalysis(args, recoFile, anaFile, NA62AnalysisPath):
	try:
		print ' '.join([NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile])
		if args.autoRun and args.testRun:
			#analysis = subprocess.Popen(["/usr/bin/stdbuf", "-o", "0", NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile], stdout=subprocess.PIPE)
			analysis = subprocess.Popen([NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile], stdout=subprocess.PIPE)
		else:
			#analysis = subprocess.Popen(["/usr/bin/stdbuf", "-o", "0", NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile, "-g"], stdout=subprocess.PIPE)
			analysis = subprocess.Popen([NA62AnalysisPath+"/LightBoxTest", "-i", recoFile, "-o", anaFile, "-g"], stdout=subprocess.PIPE)
		setNonBlocking(analysis.stdout)
		print "Running analysis... please wait"
		answer = Answer()
		answer.Bool = None
		illum = 0
		while True:
			while True:
				try:
					line = analysis.stdout.readline()
				except IOError:
					continue
				else:
					break
			line = line.rstrip('\n')
			print line
			print '\033[0m',
			if "100.00%" in line:
				nEvt = line.strip('\n').split(' ')[3].split('/')[1]
				print "nEvt is "+nEvt
			if "llll" in line:
				illum = line.strip('\n').split(' ')[2]
				print "Illumination is "+illum
			if "Analysis complete" in line:
				print "Analysis complete ..."
				answer.Data = 100.0*float(illum)/float(nEvt)
				if args.autoRun and args.testRun:
					answer.Bool = 'y'
				else:
					answer.Bool = raw_input("Are the plots ok? [y/n] ")
				time.sleep(1);
				break
			if "Bye!" in line:
				print "Analysis failed ..."
				raise(MagicScriptError("Fatal"))
	
	except OSError as e:
		print "Unable to run LightBoxTest [Error {0}]: {1}".format(e.errno, e.strerror)
		raise(MagicScriptError("Fatal"))
	
	if answer.Bool==None:
		print "Analysis failed ..."
		raise(MagicScriptError("Fatal"))
		
	analysis.terminate()
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


def setLowVoltage(channels, values):
	channelSetString = "ch %s setval %s "
	lvString = "lv v "
	
	for c,v in zip(channels,values):
		lvString += channelSetString % (c, v)
	
	print lvString

def setThresholds(channels, values):
	channelSetString = "ch %s setval %s "
	thString = "th "
	
	for c,v in zip(channels,values):
		thString += channelSetString % (c, v)
	
	print thString

def singleRun(args, listFile, recoFile, anaFile, NA62Reco, NA62Analysis, i):
	
	if args.scanV or args.scanT:
		listFile=listFile+"scan_%s_" % (i)
		recoFile=recoFile+"scan_%s_" % (i)
		anaFile=anaFile+"scan_%s_" % (i)

	print listFile
	daqResults = rundaq(args)
	
	if args.testRun:
		listFile=listFile+daqResults[1]['File'][24:]
		recoFile=recoFile+daqResults[1]['File'][24:]+".root"
		anaFile=anaFile+daqResults[1]['File'][24:]+".root"
	
	runReco(daqResults, listFile, recoFile, NA62Reco)
	
	answer = runAnalysis(args, recoFile, anaFile, NA62Analysis)

	if answer.Bool.lower() == 'n':
		if not args.testRun:
			return
		else:
			os.remove(listFile)
			os.remove(recoFile)
			os.remove(anaFile)
			for f in [x['File'] for x in daqResults]:
				os.remove(f)
	
	if args.testRun:
		print daqResults[1]['File'][35:37]+"."+daqResults[1]['File'][38:40]+" AllPM ON: Run_"+daqResults[1]['File'][24:]+": Laser On -> "+str(answer.Data)+"\n"
		if answer.Bool.lower() == 'y':
			fd = open("/home/na62bham/LogBook.txt", 'a')
			fd.write(daqResults[1]['File'][35:37]+"."+daqResults[1]['File'][38:40]+" AllPM ON: Run_"+daqResults[1]['File'][24:]+": Laser On -> "+str(answer.Data)+"\n")
			fd.close()
	
	return (listFile,recoFile,anaFile)

def process(args):
	global dimClient
	global channels
	global scanVoltages
	global nScanValues
	global defaultVoltages
	global scanThresholds
	global defaultThresholds

	
	if getenv("NA62RECOSOURCE") == None:
		print "Missing NA62RECOSOURCE environment variable"
		raise(MagicScriptError("Fatal"))
	if getenv("NA62ANALYSIS") == None:
		print "Missing NA62ANALYSIS environment variable"
		raise(MagicScriptError("Fatal"))
	
	NA62Reco = getenv("NA62RECOSOURCE")
	NA62Analysis = getenv("NA62ANALYSIS")
	
	args.LightBox = "LightBox"+str(args.lightBoxNumber)
	lightBoxDir = os.getcwd()+"/results/"+args.LightBox+"/"
	if not args.testRun:
		listFile = lightBoxDir+args.LightBox+".list"
		recoFile = lightBoxDir+args.LightBox+".root"
		anaFile = lightBoxDir+args.LightBox+"Test.root"
		extractDir = lightBoxDir+"tex"
		texFile = lightBoxDir+"tex/"+args.LightBox+".tex"
		BlueIndexMapFile = lightBoxDir+args.LightBox+"BlueIndexMap.txt"
		texPath = lightBoxDir+"tex"
		texFile = texPath+"/"+args.LightBox+".tex"
	else:
		listFile = NA62Reco+"/list_"
		recoFile = NA62Reco+"/Run_"
		anaFile = NA62Analysis+"/TRes_"
		BlueIndexMapFile = "results/"+args.LightBox+"/"+args.LightBox+"BlueIndexMap.txt"
		texPath = "results/"+args.LightBox+"/tex"
	
	if not os.path.exists("results"):
		os.mkdir("results")
	
	if not os.path.exists(lightBoxDir):
		os.mkdir(lightBoxDir)
	
	if not os.path.exists(texPath):
		os.mkdir(texPath)
	
	pmMap = mapPMs(args.mapFile, "datasheet.dat")

	fd = open(BlueIndexMapFile, 'w')
	for pm in pmMap:
		fd.write(str(pmMap[pm]['GeoPos']) + " " + str(pmMap[pm]['BlueSens']) + "\n")
	fd.close()

	if os.path.lexists(os.getcwd()+"/LightBoxBlueIndexMap.txt"):
		os.remove(os.getcwd()+"/LightBoxBlueIndexMap.txt")
	os.symlink(BlueIndexMapFile,os.getcwd()+"/LightBoxBlueIndexMap.txt")
	
	dimClient.start()
		
	if args.scanV:		
		#voltage scan
		for i,lv in enumerate(zip(*scanVoltages.itervalues())):
			setLowVoltage(channels, lv)
			
			(listFile,recoFile,anaFile) = singleRun(args, listFile, recoFile, anaFile, NA62Reco, NA62Analysis, i)
			
		#reset voltages
		setLowVoltage(channels, defaultVoltages.itervalues())
	elif args.scanT:	
		#threshold scan
		for i,th in enumerate(zip(*scanThresholds.itervalues())):
			setThresholds(channels, th)
			
			(listFile,recoFile,anaFile) = singleRun(args, listFile, recoFile, anaFile, NA62Reco, NA62Analysis, i)
		
		setThresholds(channels, defaultThresholds.itervalues())
	else:
		(listFile,recoFile,anaFile) = singleRun(args, listFile, recoFile, anaFile, NA62Reco, NA62Analysis,0)
		
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
		parser_run.add_argument("-a", "--autorun", dest="autoRun", action="store_true", help="Don't ask for any confiramtion from user")
		parser_run.add_argument("-m", "--mapfile", dest="mapFile", required=True, help="Path to file containing PM geo position in the box (SN Pos)")
		parser_run.add_argument("-n", "--nevents", dest="nEvents", required=True, help="Number of events required")
		parser_run.add_argument("-l", "--lightbox", dest="lightBoxNumber", type=int, required=True, help="LightBox number currently being tested")
		parser_run.add_argument("-d", "--dryrun", dest="dryRun", action="store_true", help="Don't try to establish socket connection with tel62")
		parser_run.add_argument("-t", "--testrun", dest="testRun", action="store_true", help="Don't save in the final result dir")
		parser_run.add_argument("--scanV", dest="scanV", action="store_true", help="Run voltage scan")
		parser_run.add_argument("--scanT", dest="scanT", action="store_true", help="Run threshold scan")
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
