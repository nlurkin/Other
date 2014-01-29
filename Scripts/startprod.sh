#!/bin/env python
import subprocess
import os

def getOutput(cmd, arg):
        p = subprocess.Popen([cmd, arg], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        return out, err


nbr = range(0,50)
out, err = getOutput("nsls", "/castor/cern.ch/user/n/nlurkin/Data")
files = out.split()

for i in nbr:
	file = "%s.root" % (i)
	if not file in files:
		job = "/afs/cern.ch/user/n/nlurkin/scripts/prod.sh %s" % (i)
		cmd = "bsub -q 8nh -R \"type=SLC5_64\" \"%s\"" % (job)
		print cmd
		os.system(cmd)
