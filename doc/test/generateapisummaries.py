import sys
import re
import os
import shutil
import glob
import subprocess

spacespattern = re.compile("\s+")

def extractmembers(file):
    if not os.path.exists(file):
        return []
    f = open(file)
    newmembers = []
    for line in f:
        m = propertypattern.match(line)
        if m:
            newmembers.append(spacespattern.sub(" ", m.group(1)))
        m = methodpattern.match(line)
        if m:
            newmembers.append(spacespattern.sub(" ", m.group(1)))
        m = signalpattern.match(line)
        if m:
            newmembers.append(spacespattern.sub(" ", "signal " + m.group(1)))
    return newmembers

# ### Script start ###
if len(sys.argv) <> 2:
    print 'SYNTAX: comparedocwithimplementation.py [platform]'
    sys.exit(0)

# standard locations etc.
docsource = "../src"
implementationsource = "../../src"
platformname = sys.argv[1]

implementedmembers = "implementation"
documentedmembers = "documentation"
commondoc = "common"
commonimplementation = "components"

# Clear the temp folders
if (os.path.exists(implementedmembers)):
    shutil.rmtree(implementedmembers)
if (os.path.exists(documentedmembers)):
    shutil.rmtree(documentedmembers)
os.mkdir(implementedmembers)
os.mkdir(documentedmembers)

# ### Collect the documented members from the common folder and the platform folder

# Create list of all documented components common and platform, unique entries only
allfiles = set()
files = glob.glob(os.path.join(docsource,commondoc,"qt-components-*.qdoc"))
for item in files:
    allfiles|=set([os.path.basename(item)])
files = glob.glob(os.path.join(docsource,platformname,"qt-components-*.qdoc"))
for item in files:
    allfiles|=set([os.path.basename(item)])


# extract the member info from each documentation file into separate files for each component

  # patterns for member information
propertypattern = re.compile(".*\\qmlproperty\s*(.*)")
methodpattern = re.compile(".*\\qmlmethod\s*(.*)")
signalpattern = re.compile(".*\\qmlsignal\s*(.*)")
namepattern = re.compile(".*-(.*).qdoc")

for item in allfiles:
    componentname = namepattern.match(item).group(1)
    print componentname
    docfile = os.path.join(docsource,platformname,item)
    members = extractmembers (docfile)
    docfile = os.path.join(docsource,commondoc,item)
    members.extend(extractmembers (docfile))
    if members <> [] :
        members.sort()
        f = open (os.path.join(documentedmembers,componentname+".txt"),"w")
        for member in members:
            f.write(member + "\n")
        f.close

# extract the member info from the implemented components

 # platform qml files
files = glob.glob(os.path.join(implementationsource,platformname,"*.qml"))
for item in files:
    print item
    itempath = os.path.abspath(item)
    subprocess.call(["extractmembers",itempath,implementedmembers])

 #  common qml files
files = glob.glob(os.path.join(implementationsource,commonimplementation,"*.qml"))
for item in files:
    print item
    itempath = os.path.abspath(item)
    subprocess.call(["extractmembers",itempath,implementedmembers])


