#!/bin/sh
status=$1
if [ "$status" = "ok" ]
then
echo "led has been tested. It's ok!"
fi

if [ "$status" = "err" ]
then
echo "led has been tested. It's fail!"
fi