TEMPLATE = subdirs

# Disable test in windows as it cannot be run due to linker limitation
!symbian:!win32:SUBDIRS += \
# This one depends on private Qt headers :-(
#    qdatetimemodel \
    qrangemodel
