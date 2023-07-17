'''
Created on Apr 21, 2015

@author: ncl
'''
import os
import re
from Objects import VersionObject, printcol
from versioning.Objects import bcolors
    
class VersionBumper():
    '''
    classdocs
    '''
    MAJOR = 1
    MINOR = 2
    PATCH = 3
    
    _commonVarNames = ["version", "rev"]
    
    def __init__(self):
        '''
        Constructor
        '''
        pass
    
    def _rebuildLine(self, dico):
        return "{pre}{var}{middle}{major}{sep1}{minor}{sep2}{patch}{end}".format(**dico)
    
    def _findInLine(self, line, flag, vName):
        versionMatch = None
        m = re.search("(?P<pre>.*)(?P<var>" + vName + ")(?P<middle>[^0-9]*)(?P<major>[0-9]+)(?P<sep1>[^0-9]*)(?P<minor>[0-9]*)(?P<sep2>[^0-9]*)(?P<patch>[0-9]*)(?P<end>[^0-9]*)", line, flag)
        if m:
            versionMatch = m.groupdict()
        return versionMatch

    def _doBumpVersion(self, data, version, dico, line, element):
        if element==self.MAJOR:
            printcol("Bumping MAJOR number")
            version.upMajor()
            version.resetMinor()
            version.resetPatch()
        elif element==self.MINOR:
            printcol("Bumping MINOR number")
            version.upMinor()
            version.resetPatch()
        elif element==self.PATCH:
            printcol("Bumping PATCH number")
            version.upPatch()
            
        version.updateDico(dico)
    
    def getVersion(self, filePath, versionVariable, data):
        Lversion = []
        LversionMatch = []
        LlineMatch = []
        if versionVariable is None:
            flag = re.IGNORECASE
            for vName in self._commonVarNames:
                for i, line in enumerate(data):
                    v = self._findInLine(line, flag, vName)
                    if v is not None:
                        version = VersionObject(v['major'], v['minor'], v['patch'])
                        if version.getN()==2:
                            v['end'] = v['sep2']
                            v['sep2'] = ""
                        elif version.getN()==1:
                            v['end'] = v['sep1']
                            v['sep2'] = ""
                            v['sep1'] = ""
                        Lversion.append(version)
                        LlineMatch.append(i)
                        LversionMatch.append(v)
        else:
            for i, line in enumerate(data):
                v = self._findInLine(line, 0, versionVariable)
                if v is not None:
                    version = VersionObject(v['major'], v['minor'], v['patch'])
                    if version.getN()==2:
                        v['end'] = v['sep2']
                        v['sep2'] = ""
                    elif version.getN()==1:
                        v['end'] = v['sep1']
                        v['sep2'] = ""
                        v['sep1'] = ""
                    Lversion.append(version)
                    LlineMatch.append(i)
                    LversionMatch.append(v)

        return(Lversion, LversionMatch, LlineMatch)
        
    def bumpFileSingleLine(self, filePath, versionVariable, element):
        if not os.path.isfile(filePath):
            print "Error: %s is not a file or cannot be read" % filePath
            return
        
        with open(filePath) as fd:
            data = fd.readlines()

        (version, dico, modLine) = self.getVersion(filePath, versionVariable, data)
        
        if len(version)==0:
            printcol("Version is not found")
            return
        
        printcol("List of current versions:")
        for i,v in enumerate(version):
            print "%s : \"%s\"" % (v, self._rebuildLine(dico[i]).rstrip())
        
        # Don't bump, stop here
        if element!=self.MAJOR and element!=self.MINOR and element!=self.PATCH:
            return
          
        if len(version)==1:
            # Only one line corresponding to version number found
            iVersion = [0]
        else:
            #Multiple lines corresponding to version number found
            #Ask user to choose which one
            print "Multiple version string found:"
            for i,v in enumerate(version):
                print " [%i] %s" % (i,self._rebuildLine(dico[i]).rstrip())
            
            print "Which ones do you want to bump?"
            ans = raw_input("List of comma separated numbers or all: ")
            if ans.lower()=="all":
                iVersion = range(0, len(version))
            else:
                iVersion = [int(x) for x in ans.split(",")]

        modified = False
        for iMod in iVersion:
            printcol("Modifying: ")
            print bcolors.FAIL + "- %s : \"%s\"" % (version[iMod], self._rebuildLine(dico[iMod]).rstrip()) + bcolors.ENDC
            self._doBumpVersion(data, version[iMod], dico[iMod], modLine[iMod], element)
            print bcolors.OKGREEN + "+ %s : \"%s\"" % (version[iMod], self._rebuildLine(dico[iMod]).rstrip()) + bcolors.ENDC
       
            ans = raw_input("Do you agree? [Y/N]").lower()
            if ans=="y":
                data[modLine[iMod]] = self._rebuildLine(dico[iMod])
                modified = True
            else:
                continue
        
        if modified:
            with open(filePath, 'w') as fd:
                fd.writelines(data)
