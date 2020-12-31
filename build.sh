#!/bin/bash

set -e

cd "`dirname $0`" || { echo "ERROR: Could not enter the repo root directory."; exit 1; }

export PSPDEV=$PWD/pspdev
export PATH=$PATH:$PSPDEV/bin

if test -f "Code/Makefile"; then
	make -j4 --no-print-directory -C Code release
fi

if test -d "ReleaseCandidate"; then
	cp -r Output/RELEASE/%__SCE__SuicideBarbie ReleaseCandidate/
	cp -r Output/RELEASE/__SCE__SuicideBarbie ReleaseCandidate/

	psp-size ReleaseCandidate/__SCE__SuicideBarbie/EBOOT.PBP
fi
