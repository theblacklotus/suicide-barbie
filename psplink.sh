#!/bin/bash

set -e

cd "`dirname $0`" || { echo "ERROR: Could not enter the repo root directory."; exit 1; }

export PSPDEV=$PWD/pspdev
export PATH=$PATH:$PSPDEV/bin

killall usbhostfs_pc || echo "no usbhostfs_pc found"
usbhostfs_pc -mcd Output/RELEASE ReleaseCandidate/__SCE__SuicideBarbie/BarbieData &

while ! nc -z localhost 10000; do
  sleep 0.1
done

sleep 1
pspsh
