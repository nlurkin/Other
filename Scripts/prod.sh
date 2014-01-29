#!/bin/sh

cd /afs/cern.ch/user/n/nlurkin/scratch0/NA62MC
source /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/setup.sh
export PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/bin:$PATH
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/lib64:$LD_LIBRARY_PATH
source scripts/env.sh
cd $TMPDIR
cp -r /afs/cern.ch/user/n/nlurkin/scratch0/NA62MC/Beam .
cp /afs/cern.ch/user/n/nlurkin/scratch0/NA62MC/*.txt .
/afs/cern.ch/user/n/nlurkin/scratch0/NA62MC/bin/Linux-g++/NA62MC /afs/cern.ch/user/n/nlurkin/scratch0/NA62MC/macros/StandardRun.mac $1
#mv pluto.root /afs/cern.ch/user/n/nlurkin/prod/$1.root
rfcp pluto.root /castor/cern.ch/user/n/nlurkin/Data/$1.root
