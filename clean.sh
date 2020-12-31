#!/bin/bash

set -e

cd "`dirname $0`" || { echo "ERROR: Could not enter the repo root directory."; exit 1; }

if test -f "Code/Makefile"; then
	make -C Code clean
fi

if test -d "ReleaseCandidate"; then
	git checkout ReleaseCandidate
fi