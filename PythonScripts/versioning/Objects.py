'''
Created on Apr 21, 2015

@author: ncl
'''

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def printcol(cmt):
    print bcolors.FAIL + "----> " + cmt + bcolors.ENDC


class VersionObject():
    '''
    classdocs
    '''


    def __init__(self, major=None, minor=None, patch=None):
        '''
        Constructor
        '''
        if major=='' or major is None:
            self._major = None
        else:
            self._major = int(major)
        if minor=='' or minor is None:
            self._minor = None
        else:
            self._minor = int(minor)
        if patch=='' or patch is None:
            self._patch = None
        else:
            self._patch = int(patch)        
    
    def getN(self):
        if not self._major is None and not self._minor is None and not self._patch is None:
            return 3
        elif not self._major is None and not self._minor is None:
            return 2
        elif not self._major is None:
            return 1
        else :
            return 0        
    
    def upMajor(self):
        if not self._major is None:
            self._major += 1

    def upMinor(self):
        if not self._minor is None:
            self._minor += 1

    def upPatch(self):
        if not self._patch is None:
            self._patch += 1
        
    def resetMajor(self):
        if not self._major is None:
            self._major = 0

    def resetMinor(self):
        if not self._minor is None:
            self._minor = 0

    def resetPatch(self):
        if not self._patch is None:
            self._patch = 0
        
    def updateDico(self, dico):
        if not self._major is None:
            dico['major'] = self._major
        if not self._minor is None:
            dico['minor'] = self._minor
        if not self._patch is None:
            dico['patch'] = self._patch
    
    def __repr__(self):
        return self.__str__()
    
    def __str__(self):
        str_rep = ""
        if not self._major is None:
            str_rep += str(self._major)
        if not self._minor is None:
            str_rep += "." + str(self._minor)
        if not self._patch is None:
            str_rep += "." + str(self._patch)
        
        return str_rep
    