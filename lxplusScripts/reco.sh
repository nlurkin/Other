#!/bin/sh

cd /afs/cern.ch/user/n/nlurkin/scratch0/NA62Reconstruction
source /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/setup.sh
export PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/bin:$PATH
export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/lib64:$LD_LIBRARY_PATH
source scripts/env.sh
/afs/cern.ch/user/n/nlurkin/scratch0/NA62Reconstruction/NA62Analysis -i $1/$2 -o $2
rfcp $2 /castor/cern.ch/user/n/nlurkin/reco/
