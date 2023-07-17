#!/bin/sh


rfcp /castor/cern.ch/user/n/nlurkin/$1 /castor/cern.ch/user/n/nlurkin/$2
xxx=$((nsls /castor/cern.ch/user/n/nlurkin/$2) 2>&1)

aaa=`echo $xxx | grep "No such"`
if [[ -z $aaa ]];then
rfrm /castor/cern.ch/user/n/nlurkin/$1
fi
