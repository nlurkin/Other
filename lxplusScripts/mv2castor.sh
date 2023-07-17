#!/bin/sh


rfcp $1 /castor/cern.ch/user/n/nlurkin/$2
xxx=$((nsls /castor/cern.ch/user/n/nlurkin/$2/$1) 2>&1)

aaa=`echo $xxx | grep "No such"`
if [[ -z $aaa ]];then
rm $1
fi
