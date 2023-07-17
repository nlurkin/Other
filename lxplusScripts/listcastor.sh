#!/bin/sh

opt=""
folder=""
if [[ ! -z $2 ]];
then
	opt=$2
	folder=$1
else
	if [ "$1" = "-l" ];
	then
		opt=$2
	else
		folder=$1
	fi
fi

if [[ -z $opt ]];then
	xxx=$(nsls $opt /castor/cern.ch/user/n/nlurkin/$folder)

	for l in $xxx
	do
		echo "rfio:///castor/cern.ch/user/n/nlurkin/$folder/$l"
	done
else
	nsls $opt /castor/cern.ch/user/n/nlurkin/$folder
fi
