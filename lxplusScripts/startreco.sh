#!/bin/env python
import subprocess
import os

def getOutput(cmd, arg):
	p = subprocess.Popen([cmd, arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, err = p.communicate()
	return out, err


out, err = getOutput("nsls", "/castor/cern.ch/user/n/nlurkin/Data")

ifiles = out.split()
out, err = getOutput("nsls", "/castor/cern.ch/user/n/nlurkin/reco")
ofiles = out.split()


for f in ifiles:
	if f[-5:]==".root":
		if not f in ofiles:
			job = "/afs/cern.ch/user/n/nlurkin/scripts/reco.sh rfio:///castor/cern.ch/user/n/nlurkin/Data %s" % (f)
			cmd = "bsub -q 8nh -R \"type=SLC5_64\" \"%s\"" % (job)
			print cmd
			os.system(cmd)
