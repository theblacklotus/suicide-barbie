#!/bin/bash

# apt install autoconf automake bison flex gcc-9 libncurses-dev make subversion texinfo wget libusb-dev build-essential libreadline-dev
# or 
# brew install autoconf automake flex gcc@9 ncurses make subversion texinfo wget libusb libusb-compat sdl

set -e

cd "`dirname $0`" || { echo "ERROR: Could not enter the repo root directory."; exit 1; }

export PSPDEV=$PWD/pspdev
export PATH=$PATH:$PSPDEV/bin

if command -v psp-config &> /dev/null
then
	CONFIG_PATH=`which psp-config`
	if [[ x"$CONFIG_PATH" != x"$PSPDEV/bin/psp-config" ]]
	then
		echo "ERROR: pspdev path is different from \$PSPDEV; Multiple installations?"; exit 1;
	fi
	echo "PSPDEV=$PSPDEV"
	exit 0;
fi

# ancient homebrew psptoolchain
if test ! -d "psptoolchain"; then
git clone https://github.com/erique/psptoolchain.git || { exit 1; }
fi

git submodule update --init --remote || { exit 1; }

cd psptoolchain || { exit 1; }

git checkout master || { exit 1; }

git pull --ff-only || { exit 1; }

if test -d $PSPDEV; then
rm -r $PSPDEV || { exit 1; }
fi

./toolchain.sh || { exit 1; }

git clean -fdx || { exit 1; }

cd .. || { exit 1; }


# somewhat recent psplink
if test ! -d "psplinkusb"; then
git clone https://github.com/pspdev/psplinkusb.git || { exit 1; }
fi

git submodule update --init --remote || { exit 1; }

cd psplinkusb || { exit 1; }

make -f Makefile.clients install || { exit 1; }

git clean -fdx || { exit 1; }

cd .. || { exit 1; }

echo "DONE!"
echo "PSPDEV=$PSPDEV"
