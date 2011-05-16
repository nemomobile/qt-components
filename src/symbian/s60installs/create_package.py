#!/usr/bin/env python
# -*- coding: utf-8 -*-

import fnmatch
import glob
import os
import re
import sys
from optparse import OptionParser

# Contains modules which will be included in the container SIS package
MODULES = ["src/components"]

# Used to map configure parameters to modules
MODULES_DICTIONARY = { "demos": ["demos"],
                     "extras": ["examples/symbian/extras", "src/symbian/extras"],
                     "examples": ["examples/symbian"],
                     "symbian3": ["src/symbian"],
                     "tests": ["tests/manual"] }

# TARGET names and UID3s will be extracted from the pro files listed
UNIQFIED_PRO_FILES = []

# Create OptionParser for going through command line parameters
usage = "usage: %prog [options]"
parser = OptionParser(usage)
parser.add_option("-v", "--verbose", action="store_true", dest="verbose", help="Verbose output")
parser.add_option("-o", "--onlyunsigned", action="store_const", const="-o", dest="onlyunsigned", help="Create an unsigned package")
parser.add_option("--repository", dest="repository", help="Path to repository, default is ../../..")
parser.add_option("--cert", dest="cert", help="The certificate used for signing, default is QT_SIS_CERTIFICATE environment variable")
parser.add_option("--key", dest="key", help="The certificate's private key file, default is QT_SIS_KEY environment variable")
parser.add_option("--target", dest="target", help="default is \'release-armv5\'")
(options, args) = parser.parse_args()

# Check options
if not options.repository:
    os.chdir("../../..")
    options.repository = os.getcwd().replace("\\", "/") + "/"
if not options.cert: options.cert = os.getenv("QT_SIS_CERTIFICATE")
if not options.key: options.key = os.getenv("QT_SIS_KEY")
if not options.target: options.target = "release-armv5"
if not options.onlyunsigned: options.onlyunsigned = ""

if options.verbose:
    print "Repository: ", options.repository
    print "Certificate file: ", options.cert
    print "Certificate key: ", options.key
    print "Target: ", options.target
    print "Unsigned: ", options.onlyunsigned

# Reads configure parameters from prf file and maps those to modules
def read_config():
    file = open(options.repository + "features/qt-components-config.prf", "r")
    modules = re.findall("\S\w*\S", file.read().rsplit("+=")[1])
    if options.verbose:
        print "Configured parameters:"
        print modules
    for module in modules:
        if MODULES_DICTIONARY.get(module) != None:
            MODULES.extend(MODULES_DICTIONARY[module])

# Walks through all the folders and collects pro files
def install(item):
    try:
        MODULES.index("examples/symbian/extras")
        MODULES.index("src/symbian/extras")
    except ValueError:
        if fnmatch.fnmatch(item, "*extras*"):
            return False
    file = open(options.repository + item, "r")
    result = re.search("TARGET.UID3", file.read())
    file.close()
    return result

# Walks through all the folders and collects pro files
def collect_pro_files():
    pro_files = []
    for module in MODULES:
       for dirpath, dirnames, filenames in os.walk(options.repository + module):
           path = dirpath[len(options.repository):]
           #Filter and fix path
           pro_files.extend(filter(install, [os.path.join(path, file).replace('\\', '/') for file in fnmatch.filter(filenames, "*.pro")]))

    # Remove duplicate pro files
    UNIQFIED_PRO_FILES.extend(list(set(pro_files)))

# Adds header part of the pkg file
def create_package_file():
    file = open(options.repository + "src/symbian/s60installs/qtcomponents.pkg", "w+")
    file.write("&EN\n")
    file.write("#{\"Qt Quick components for Symbian\"},(0x200346DE),1,0,0,TYPE=SA,RU,NR\n")
    file.write("%{\"Nokia\"}\n")
    file.write(":\"Nokia, Qt\"\n")
    return file

# Extracts TARGET and UID3 from included pro files
def read_from_pro_file(pro_file):
    file = open(options.repository + pro_file, "r")
    content = file.read()
    target = re.findall("(?:TARGET\s?=\s?)(?:(?:\$\$\w*\()?)(\w*)(?:\)?)", content)[0]
    uid = re.findall("(?:TARGET.UID3\s*=\s*)(.*)", content)[0]

    return target, uid

# Appends given module to pkg file
def append_to_package_file(pkgFile, pro_file, target, uid):
    # Format: @"src/symbian/symbianplugin.sis",(0x200346DD)

    # Extract path
    path = pro_file[:pro_file.rfind("/")]
    pkgFile.write("@\"%s/%s.sis\",(%s)\n" % (path, target, uid))

def collect_names_and_uids(pkgFile):
    for pro_file in UNIQFIED_PRO_FILES:
        target, uid = read_from_pro_file(pro_file)
        append_to_package_file(pkgFile, pro_file, target, uid)

# MAIN
read_config()
collect_pro_files()

if options.verbose:
    print "Included modules:\n", MODULES
    print "\n"
    print "Included pro files:"
    for file in UNIQFIED_PRO_FILES:
        print file
    print "\n"

# Create the pkg file with extracted modules
pkgFile = create_package_file()

# Open pro file and extract TARGET and TARGET.UID3
collect_names_and_uids(pkgFile)

pkgFile.close()

# Create and sign the SIS package
os.chdir(options.repository)
os.system("createpackage.bat %s -g %ssrc/symbian/s60installs/qtcomponents.pkg %s %s %s" % (options.onlyunsigned, options.repository, options.target, options.cert, options.key))
