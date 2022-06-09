#!/bin/sh
#Loading code
if [ $# "<" 1 ] ; then # Verificar que tem 2 argumentos
    echo "Usage: $0 <module_name>"
    exit -1
else
    module="$1" #O nome do modulo esta na posiçao 1 do vetor de argumentos
    #echo "Module Device: $1"
fi
insmod -f ./${module}.ko || exit 1 # instalar o kernel module
major=`cat /proc/devices | awk "\\$2==\"$module\" {print \\$1}"| head -n 1` 
#
# cat /proc/devices
#       -> lists al devices installed
# awk "\\$2==\"$module\" {print \\$1}"
#       -> finds the module from last command
# hean -n 1
#       -> isolates the major number to be stored

mknod /dev/${module} c $major 0
#changes file type
chmod a+rw /dev/${module}
#changes permission
exit 0