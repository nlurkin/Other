print "Import versioning module ... ",

from VersionBumper import VersionBumper
from Objects import VersionObject
from bumpFromCLI import startbump


print "Done"
print "Usage:"
print " versioning.startbump(\"fileName [[variableName] major|minor|patch]\")"
print "    fileName: File containing version number"
print "    variableName: Version variable to update (if known, else automatic discovery)"
print "    major|minor|patch: Element to bump"
print ""
