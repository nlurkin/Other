#!/bin/sh

echo "./autocheck.sh exampleFile inputDir expected option(d)"

example=$1
indir="/castor/cern.ch/user/n/nlurkin/$2"
expected=$3
option=$4

#listfiles=`find $indir -name "*.root" | sort`
listfiles=`nsls $indir | sort`

for f in $listfiles
do
	echo -en "Checking $f            \t\t\r"
	./check_complete $indir/$example $indir/$f $expected &> /dev/null
	if [ $? -eq 255 ] 
	then
		bad=`echo $f | sed 's/.*_\([0-9]*\)\.root/\1/'`
		echo -en "                                                                                             \r"
		echo $bad
		if [ "$option" == "d" ]
		then
			rfrm $indir/$bad
		fi
	fi
done

echo " "
