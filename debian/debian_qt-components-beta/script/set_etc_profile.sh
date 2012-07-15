#!/bin/bash
if grep -q "qt-components-beta" /etc/profile
then
     echo "qt-components-beta environment setting already added to /etc/profile, doing nothing"
else
     echo '
if [ -e /etc/profile.d/qt-components-beta.sh ] ; then
     . /etc/profile.d/qt-components-beta.sh
fi' >> /etc/profile
fi
