#!/bin/env python

import os
import re
import signal
import subprocess


pid = subprocess.Popen("ps ax".split(" "), stdout=subprocess.PIPE)

(out, _) = pid.communicate()

for line in out.split("\n"):
	if "na62-farm" in line:
		cmdArray = re.split("\s+", line.strip())
		startIndex = [ i for i, word in enumerate(cmdArray) if word.endswith('na62-farm') ][0]
		print startIndex
		os.kill(int(cmdArray[0]), signal.SIGKILL)
		paramList = cmdArray[startIndex+1:]
		paramList.append("--verbosity=3")
		paramList.append("--logtostderr=1")
		paramList.append("--printMissingSources=1")
		os.execvp("na62-farm", paramList)