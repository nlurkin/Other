'''
Created on Apr 21, 2015

@author: ncl
'''

from versioning import VersionBumper

def startbump(args):
    args = args.split()
    if len(args)==1:
        #Only display version. Variable is not known
        vb = VersionBumper()
        vb.bumpFileSingleLine(args[0], None, None)
    elif len(args)==2:
        #Bump, variable not known or
        #Only display, variable known
        vb = VersionBumper()
        if args[-1].lower()=="major":
            vb.bumpFileSingleLine(args[0], None, VersionBumper.MAJOR)
        elif args[-1].lower()=="minor":
            vb.bumpFileSingleLine(args[0], None, VersionBumper.MINOR)
        elif args[-1].lower()=="patch":
            vb.bumpFileSingleLine(args[0], None, VersionBumper.PATCH)
        else:
            vb.bumpFileSingleLine(args[0], None, None)
    elif len(args)==3:
        #Bump, Variable known
        vb = VersionBumper()
        if args[-1].lower()=="major":
            vb.bumpFileSingleLine(args[0], args[1], VersionBumper.MAJOR)
        elif args[-1].lower()=="minor":
            vb.bumpFileSingleLine(args[0], args[1], VersionBumper.MINOR)
        elif args[-1].lower()=="patch":
            vb.bumpFileSingleLine(args[0], args[1], VersionBumper.PATCH)
