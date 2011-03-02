TEMPLATE = subdirs

# These autotests consist of things such as static code checks
# which require that the autotest is run on the same machine
# doing the build - i.e. cross-compilation is not allowed.
win32|mac|linux-g++* {
    !embedded:!maemo5:!maemo6:SUBDIRS += maketestselftest
}
