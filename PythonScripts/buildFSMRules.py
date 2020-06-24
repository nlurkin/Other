#!/bin/env python

'''
Created on 22 Jun 2017

@author: ncl
'''

global VERBOSE
import sys

from yaml import Loader, SafeLoader
import yaml

from enum import Enum
class RuleType(Enum):
    kLOW = 1
    KHIGH = 2

def construct_yaml_str(self, node):
    # Override the default string handling function 
    # to always return utf-8 strings
    return str(self.construct_scalar(node).encode("utf-8"))
Loader.add_constructor(u'tag:yaml.org,2002:str', construct_yaml_str)
SafeLoader.add_constructor(u'tag:yaml.org,2002:str', construct_yaml_str)


def readYamlFiles(filePath):
    with open(filePath, "r") as fd:
        fsmdecr = yaml.load(fd)

    return fsmdecr

def getLowerStates(state, statesList):
    index = statesList.index(state)
    if index==0:
        return []
    if index==len(statesList):
        return statesList[:-1]
    return statesList[:index]

def getHigherStates(state, statesList):
    index = statesList.index(state)
    if index==0:
        return statesList[1:]
    if index==len(statesList):
        return []
    return statesList[index+1:]

def getPreviousState(state, statesList):
    index = statesList.index(state)
    if index==0:
        return None
    return statesList[index-1]

def isHigher(state1, state2, statesList):
    return state1 in getHigherStates(state2, statesList)

def getHighestOf(states, statesList):
    highest = -1
    for s in states:
        index = statesList.index(s)
        if index>highest:
            highest = index
    if highest==-1:
        return []
    return statesList[highest]

def findMappedState(state, statesList, mapping, childStates):
    #print state, mapping
    while not state in mapping and not state is None:
        state = getPreviousState(state, statesList)
    
    if state in childStates:
        return state, state
    if state is None:
        return None, None
        
    return state, mapping[state]
                    
def printSet(valSet):
    return "{{ {0} }}".format(",".join(valSet))

def getStatesFromNaturalList(stateOut, statesList):
    if stateOut in statesList:
        printVerb("{0} was found in the list of states".format(stateOut))
        return stateOut, set([stateOut])
    printVerb("{0} was not found in the list of states".format(stateOut))
    return None, set()

def getStatesFromMap(stateOut, parentStatesList, child, rType):
    
    if not 'Mapping' in child:
        return None, set()

    corrState, foundStates = findMappedState(stateOut, parentStatesList, child['Mapping'], child['Node']['States'])
    corrStateIn, _ = findMappedState(stateIn, parentStatesList, child['Mapping'], child['Node']['States'])
    
    printVerb("The correspondances {0} was found for {1} (stateOut) in the mapping at state {2}".format(foundStates, stateOut, corrState))
    printVerb("A correspondance was found for {0} (stateIn) in the mapping at {1}".format(stateIn, corrStateIn))
    
    if foundStates is None:
        print "Error: {0} has no mapping on device {1}".format(stateOut, child['Node']['Name'])
        sys.exit(-1)
    if rType==RuleType.kLOW and corrStateIn==corrState:
        printVerb("Rule is of type low and the device has no influence on this transition")
        return None, None
    highState = getHighestOf(foundStates, child['Node']['States'])
    childState = set(foundStates)
    
    if len(highState)==0:
        if rType==RuleType.KHIGH:
            #highState = child['Node']['States'][-1]
            childState = set(child['Node']['States'])
        else:
            return None, None
    printVerb("The list of accepted states for the transition are {0}".format(childState))
    return corrState, childState

def printVerb(strV):
    if VERBOSE:
        print strV

def printStateRules(stateIn, statesList, rType, children):
    if rType==RuleType.kLOW:
        keyword = "ANY"
        joinWord = " or \n"
        checkStates = getLowerStates(stateIn, statesList)

    elif rType==RuleType.KHIGH:
        
        keyword = "ALL"
        joinWord = " and \n"
        checkStates = reversed(getHigherStates(stateIn, statesList))
    
    for stateOut in checkStates:
        printVerb("Going from state {0} to state {1}".format(stateIn, stateOut))
        cond = "when ( {listConditions} ) move_to {0}"
        listConditions = []
        for sub in children:
            printVerb("Looking at device {Name}".format(**sub['Node']))
            highestState_nat, childStateSet_nat = getStatesFromNaturalList(stateOut, sub['Node']['States'])
            corrState_map, childStateSet_map = getStatesFromMap(stateOut, statesList, sub, rType)
            
            if highestState_nat is None and not 'Mapping' in sub:
                print "Error: {0} has no mapping on device {1}".format(stateOut, sub['Node']['Name'])
                sys.exit(-1)

            if corrState_map is None and childStateSet_map is None:
                continue
            
            
            #highestState = highestState_nat if not None else corrState_map
            if highestState_nat is None or (corrState_map == stateOut):
                childStateSet = childStateSet_nat.union(childStateSet_map)
            else:
                childStateSet = childStateSet_nat
            if rType==RuleType.KHIGH and len(childStateSet)==len(sub['Node']['States']):
                #childStateSet.update(getHigherStates(highestState, sub['Node']['States']))
                childStateSet.discard(stateIn)
                
            printVerb("After merging the natural and mapping, the list of states for the transition is {0}".format(childStateSet))
            listConditions.append("( ${keyword}${Name} in_state {0} )".format(printSet(childStateSet), keyword=keyword, **sub['Node']))
        print cond.format(stateOut, listConditions=joinWord.join(listConditions))
        print 


def getCorrespondingStates(stateList, child):
    childStates = set()
    for state in stateList:
        childStates.update(set(child['Mapping'][state]))
    
    return childStates
        

def getCorrespondingNaturalState(state, child):
    if state in child['Node']["States"]:
        return [state]
    return None

def getCorrespondingMappedState(state, child):
    if not 'Mapping' in child:
        return None
    
    if state in child['Mapping']:
        return child['Mapping'][state]
    
    return None

def getClosestCorrespondance(statesList, child):
    printVerb([statesList, child['Mapping']])
    for state in reversed(statesList):
        printVerb("Testing " + state) 
        if state in child["Mapping"]:
            return child["Mapping"][state]
    return None
        
def printTransitionRule(stateIn, stateOut, nodeStates, children):
    if isHigher(stateIn, stateOut, nodeStates):
        rType = RuleType.kLOW
        keyword = "ANY"
        joinWord = " or \n"
    else:
        rType = RuleType.KHIGH
        keyword = "ALL"
        joinWord = " and \n"
    
    cond = "when ( {listConditions} ) move_to {0}"
    listConditions = []
    for child in children:
        if child['Mapping'][stateOut] is None:
            print "Error: {0} has no mapping on device {1}".format(stateOut, child['Node']['Name'])
            sys.exit(-1)
        
        if child['Mapping'][stateOut] == child['Mapping'][stateIn] and rType==RuleType.kLOW:
            childStates = set()
        else:
            childStates = set(child['Mapping'][stateOut])
            
        if len(childStates)==0 and rType==RuleType.kLOW:
            continue
        elif len(childStates)==0 and rType==RuleType.KHIGH:
            childStates = set(child['Node']['States'])
            childStates.discard(stateIn)
            
        if rType==RuleType.KHIGH:
            childStates.update(getCorrespondingStates(getHigherStates(stateOut, nodeStates), child))
        listConditions.append("( ${keyword}${Name} in_state {0} )".format(printSet(childStates), keyword=keyword, **child['Node']))
    print cond.format(stateOut, listConditions=joinWord.join(listConditions))
    print

def printStateRules2(stateIn, nodeStates, children):
    checkStates = getLowerStates(stateIn, nodeStates)
    for stateOut in checkStates:
        printTransitionRule(stateIn, stateOut, nodeStates, children)
    
    checkStates = getHigherStates(stateIn, nodeStates)
    for stateOut in reversed(checkStates):
        printTransitionRule(stateIn, stateOut, nodeStates, children)

def buildCorrespondanceMap(node):
    for state in node['States']:
        for child in node['Children']:
            if not 'Mapping' in child:
                child['Mapping'] = {}
            printVerb(child['Mapping'])
            naturalStates = getCorrespondingNaturalState(state, child)
            mappedStates = getCorrespondingMappedState(state, child)
            
            if naturalStates is None and mappedStates is None:
                closest = getClosestCorrespondance(getLowerStates(state, node['States']), child)
                printVerb([closest, child['Node']['States'][-1]])
                if list(closest)[0] == child['Node']['States'][-1]:
                    nodeStates = closest
                else:
                    nodeStates = None
            else:
                nodeStates = []
            
            if not naturalStates is None:
                nodeStates.extend(naturalStates)
            if not mappedStates is None:
                nodeStates.extend(mappedStates)
            printVerb([child['Node']['Name'], state, naturalStates, mappedStates])
            child['Mapping'][state] = nodeStates
    
    for child in node['Children']:
        printVerb("{Node[Name]}: {Mapping}".format(**child))
    
if __name__ == '__main__':
    global VERBOSE
    VERBOSE=False
    fsmdescr = readYamlFiles(sys.argv[1])
    
    for node in fsmdescr['logical']:
        if not 'Colors' in node:
            print "Error: No colors map for device {Name}".format(**node)
            sys.exit(-1)
        buildCorrespondanceMap(node)
        print "---------------------------------------"
        print "Generating rules for " + node["Name"]
        for stateIn in reversed(node["States"]):
            if not stateIn in node['Colors']:
                print "Error: No color mapped for state {0} on device {Name}".format(stateIn, **node)
                sys.exit(-1)
            print "############"
            print "Rules for state: " + stateIn
            print "Color: " + node['Colors'][stateIn]
            print "############"
            printStateRules2(stateIn, node['States'], node['Children'])
            print 
        #sys.exit(0)
        
        #    printStateRules(stateIn, node['States'], RuleType.kLOW, node['Children'])
        #    printStateRules(stateIn, node['States'], RuleType.KHIGH, node['Children'])
        #    print 
        #raw_input("continue")
    