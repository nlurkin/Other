#!/bin/sh

example=$1
listfiles=$2
expected=$3
option=$4

echo "example : $example"
echo "listfiles : $listfiles"
echo "expected : $expected"
echo "option : $option"

for f in $listfiles
do
	/home/fynu/nlurkin/python/CheckOutput/check_complete $example $f $expected &> /dev/null
	r=$?
	echo -en "Checking $f $r"
	if [ $r -eq 255 ] 
	then
		bad=`echo $f | sed 's/.*_\([0-9]*\)\.root/\1/'`
		echo -en " ==> BAD\n"
		if [ "$option" == "d" ]
		then
			rm $bad
		fi
	else
		echo -en " ==> OK\n"
	fi
done

echo " "
