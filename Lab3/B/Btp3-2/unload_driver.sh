#!/bin/sh
#Removal code
if [ $# "<" 1 ] ; then
    echo "Usage: $0 <module_name>"
    exit -1
else
    module="$1"
    #echo "Module Device: $1"
fi
#same as load
rmmod $module || exit 1
#remove kernel module
# remove nodes files
rm -f /dev/${module} /dev/${module}?
exit 0