#!/bin/bash

set -e

cd "`dirname $0`" || { echo "ERROR: Could not enter the repo root directory."; exit 1; }

if [ -z "$1" ]
then
	TARGET="release"
else
	TARGET="$1"
fi	

if [ -z "$PSPDEV" ]
then
	echo "PSPDEV not set.."
	export PSPDEV=$PWD/pspdev
	export PATH=$PATH:$PSPDEV/bin
fi

if command -v psp-config &> /dev/null
then
	CONFIG_PATH=`which psp-config`
	if [[ x"$CONFIG_PATH" != x"$PSPDEV/bin/psp-config" ]]
	then
		diff <(echo "$CONFIG_PATH" ) <(echo "$PSPDEV/bin/psp-config") || true
		echo "ERROR: pspdev path is different from \$PSPDEV; Multiple installations?"; exit 1;
	fi
else
	echo "ERROR: No PSP toolchain? Run ./bootstrap.sh"; exit 1;
fi

echo "PSPDEV=$PSPDEV"
echo "TARGET=$TARGET"

if test -f "Code/Makefile"; then
	make -j4 --no-print-directory -C Code $TARGET
fi

if test -d "ReleaseCandidate"; then
	cp -r Output/RELEASE/%__SCE__SuicideBarbie ReleaseCandidate/
	cp -r Output/RELEASE/__SCE__SuicideBarbie ReleaseCandidate/

	psp-size ReleaseCandidate/__SCE__SuicideBarbie/EBOOT.PBP
fi
