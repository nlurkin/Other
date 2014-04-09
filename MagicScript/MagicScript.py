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
import subprocess
from Cheetah.Template import Template



__all__ = []
__version__ = 0.1
__date__ = '2014-04-09'
__updated__ = '2014-04-09'

class CLIError(Exception):
	'''Generic exception to raise and log different fatal errors.'''
	def __init__(self, msg):
		super(CLIError).__init__(type(self))
		self.msg = "E: %s" % msg
	def __str__(self):
		return self.msg
	def __unicode__(self):
		return self.msg

def mapPMs(fileName, datasheet):
	with open(fileName, 'r') as fd:
		pms = list(fd)

	pmMap = dict([[x, {'GeoPos':int(y)}] for x, y in [x.rstrip('\n').split(' ') for x in pms]])
	
	with open(datasheet, 'r') as fd:
		pms = list(fd)
		
	pms.pop(0)
	pmsData = [[snum, {"CatLum":float(cat_lum), "AnLum":float(an_lum), "DarkCurr":float(d_curr), "BlueSens":float(b_sens), "Gain":float(gain)}] for 
			snum, cat_lum, an_lum, d_curr, b_sens, gain in [x.strip('\n').split(' ') for x in pms]]
	
	if pmMap.has_key(pmsData[0][0]):
		pmMap[pmsData[0][0]].update(pmsData[0][1]) 
	
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
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.connect(("localhost", 10))
		
	# subprocess.Popen("na62daq", stdout=open("na62daq.out",'w'))
	daq = subprocess.Popen("na62daq", stdout=subprocess.PIPE)
	
	if not args.dryRun:
		raw_input("Press enter to start run (at least 1s before next SOB)")
		sock.sendall("@startrun2.spy")
	
	daqResults = parseDaqOutput(daq, args)
	
	if not args.dryRun:
		sock.sendall("@endrun2.spy")
	
	daq.kill()
	if not args.dryRun:
		sock.close()
	
	return daqResults

def runReco(daqResults):
	fileList = open("LightBoxN.list", 'w')
	for f in [x['File'] for x in daqResults[1:-1]]:
		fileList.write(f + "\n")
	
	fileList.close()
	
	reco = subprocess.Popen(["NA62Reco", "-l LightBoxN.list", "-o LightBoxN.root"])
	print "Running reconstruction... please wait"
	reco.wait()
	print "Reconstruction finished"

def runAnalysis():
	analysis = subprocess.Popen(["LightBoxTest", "-i LightBoxN.root", "-o LightBoxNTest.root", "-g"], stdout=subprocess.PIPE)
	print "Running analysis... please wait"
	for line in analysis.stdout:
		if "Analysis complete" in line:
			print "Analysis complete ..."
			answer = raw_input("Are the plots ok? [y/n] ")
			break;
	
	analysis.kill()
	return answer

def GenerateTex(pmMap, fileName):
	searchList= [{'lightBoxNumber':0, 'pmSN':pmMap.keys()[0], 'pm':pmMap[pmMap.keys()[0]]}]
	t = Template(file='PMTemplate.tex', searchList=searchList)
	
	fd = open(fileName, 'w')
	fd.writelines(str(t))
	
	
	

def process(args):
	pmMap = mapPMs(args.mapFile, "datasheet.dat")
	
	daqResults = rundaq(args)
	
	runReco(daqResults)
	
	answer = runAnalysis()
	
	if answer.lower() == 'n':
		return
	
	for pm in pmMap:
		extract = subprocess.Popen(["ExtractPlots", "-n " + str(pmMap[pm]['GeoPos']), "-i LightBoxN.root", "-I LightBoxNTest.root", "-o path/to/tex/dir/LighBoxN"])
		extract.wait()
		GenerateTex(pmMap, "test.tex")
		latex = subprocess.Popen(["pdflatex", "test.tex"])
		latex.wait()
	
	
	
	
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
		parser.add_argument("-m", "--mapfile", dest="mapFile", required=True, help="Path to file containing PM geo position in the box (SN Pos)")
		parser.add_argument("-n", "--nEvents", dest="nEvents", required=True, help="Number of events required")
		parser.add_argument("-d", "--dryrun", dest="dryRun", action="store_true", help="Don't try to establish socket connection with tel62")
		parser.add_argument('-V', '--version', action='version', version=program_version_message)
		
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
	
	process(args)


if __name__ == "__main__":
	sys.exit(main())
