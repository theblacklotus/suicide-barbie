![barbielogo]
###	Suicide Barbie by The Black Lotus   
####      A PlayStation® Portable™ demo

####	Release History

* [Source code](https://github.com/theblacklotus/suicide-barbie) (MIT licensed) released December 31st, 2020.
* OE/M33 'slim' version released on July 8th, 2008.
* 'Polished' party version released on June 27th, 2007.
* Original production released at [Breakpoint 2007](https://breakpoint.untergrund.net/2007/).
 
####    Awards & Nominations
* 1st place [Console / Real Wild](https://breakpoint.untergrund.net/2007/compos_misc.php) demo competition at [Breakpoint 2007](https://breakpoint.untergrund.net/2007/).
* [Public Choice](https://www.pouet.net/awards.php#2007public-choice) nominee at [Scene.org 6th Annual Demoscene Awards](http://awards.scene.org/).
 
  :trumpet:  [pouët.net](https://www.pouet.net/prod.php?which=30284)  
  :movie_camera:  [YouTube](https://www.youtube.com/watch?v=Q1J7EBzwN3Q)

---

###	How Suicide Barbie Came To Be

After the success of the [Starstruck](https://www.pouet.net/prod.php?which=25778) demo, the team needed a new challenge. It had to be a demo on a new platform. On a well defined and fixed platform, but with hardware powerful enough for the Creative Vision™. Limited, yet modern. Platform unhindered by layers of software abstractions one finds in PC.  
So, inspired by the PlayStation® 2 demo [4 Edges](https://www.pouet.net/prod.php?which=24552) and hacker spirit of the [Homebrew](https://github.com/pspdev/pspsdk) community we soon had a winner - a miniaturized beast of PlayStation® with a vibrant screen. A harbinger of the new future where great visuals fit in your pocket - PlayStation® Portable™!

The goal was simple: make the most Entertaining Visual Experience, the Best Demo for the platform! And although the author is clearly biased here, but it looks like Suicide Barbie holds its ground even today and still is the best demo on the PlayStation® Portable™.

#### PlayStation® Portable™ Specs
Original PlayStation® Portable™ SoC was built on a 90nm CMOS process and was powered by 1.2V. The very first devices shipped in Japan in December 2004 and hit Europe by the end of 2005.

    ~ 32-bit [MIPS R4000](https://en.wikipedia.org/wiki/R4000) CPU clocked @ 222 MHz.  
    ~ VFPU compute co-processor pushing 2.6 GigaFlops.  
    ~ [Graphics Core](https://www.techpowerup.com/gpu-specs/sony-psp-1000-gpu.b8184) triangle rasterizer with fixed T&L pipeline that can spit up to 33M triangles and cover 665M pixels per second.  
    ~ 32 MiB main RAM with 2.6Gb/s of bandwidth.  
    ~ 2 MiB Graphics eDRAM with 5.3Gb/s of bandwidth.  

![psp]

#### The Process

As Suicide Barbie follows a story and sports massive amounts of content (relatively speaking), we would adopt the new process of production. Pick the music, make a storyboard, plan the art, setup the data pipeline, get the demo running from the day one, add the scenes as they come from the artist, continuously iterate on the quality. Ship it.

Back in 2006/2007 we didn't have the luxury of functional emulators for the PlayStation Portable. At least nothing on the level we aimed for. So the obvious thing to do - write cross-platform code and make the demo run on PC identical to PSP. All the effects would be duplicated. A little bit of extra work of course, but huge upside with the fast iteration time and easy workflow for the artists.

One innovation we had was the ability to scrub the playback time at any point. Basically the demo could run forward and back in time. Perfect for polish.  We kept this mechanism alive all the way until the end, and only disabled it when we switched to compressed (ATRAC3) streamed audio (as that made "per sample" scrubbing hard).

Data pipeline was setup in a rather simple, but effective way - a bunch of batch files. In goes the source data, out goes the compiled data ready for blob loading into the memory. With this data pipeline we could build a separate scene or the whole demo. Run [goPsp.bat](Data/DemoTest/goPsp.bat) and you get the PSP version. Run [goDx9.bat](Data/DemoTest/goDx9.bat) and you can run it on PC.

Finally, if you push the boundaries of the consoles, you have to have streaming. Suicide Barbie does not fit into 32 MiB of main RAM and constantly stream the content ahead of the player.

_NOTE: While (most of) the code is present in this repository we haven't bothered upgrading the vs2005 projects, so DX9 playback is lost (except for the most adventurous)._

---

####	Credits

        Direction, Graphics:   Louie / TBL^CNCD  
        Graphics:                    Jintz  
  
        Code:                          ReJ / Nesnausk!  
        Code:                          eriQue / soniK cliQue  
  
        Music:                         Elusive _'Pete loves Veronica mix'_  
                                               (Veronica Maggio ['Dumpa Mig'](https://www.youtube.com/watch?v=fphQeXPvTaI))

        Animations:                Micke   
        Animations:                Johra   
        Animations:                Duffe   

        Additional Graphics:  Eracore / Rebels

        Platform Consultant: Chip
 
####	Additional Credits & Inspiration

        Roberto Roseano (Barbie Suicide)  
        Psyop
 
####	Special Thanks

        [pspdev](https://github.com/pspdev) community for homebrew  
        TyRaNiD for [psplink](https://github.com/pspdev/psplinkusb) (this demo would not've been possible without it)  
        orlok^simple / Nesnausk! for mutant lib  
        daywish^cathy / Nesnausk! for support and photojournalism in Bingen  

---

###	How To Run The Production on PlayStation® Portable™

This production has been tested and confirmed working under firmware 1.00, 1.50 and 3.40 OE-A (150 binary) and 3.90 M33-3 and 4.01 M33-2 (OE/M33/Slim binary). Use a fast memory stick like SanDisk 2GB to guarantee a smooth viewing experience.

    ~ Unpack the contents of the archive from the [releases](../../releases) page to the `ms0:/PSP/GAME/` folder on your [Memory Stick](https://en.wikipedia.org/wiki/Memory_Stick).  
    ~ If you're using Open Edition [OE/M33](https://playstationdev.wiki/pspdevwiki/index.php?title=M33_Firmware) firmware, and the original 150 binaries, copy the files to `ms0:/PSP/GAME150/` instead.  
    ~ Run the production from the XMB (PlayStation Portable menu screen) as you would do with a regular game.  

####	How To Run The Production in an Emulator

If you do not own a PlayStation® Portable™ device, we recommend installing the [`ppsspp`](https://www.ppsspp.org/) emulator.  
PPSSPP even lists **Suicide Barbie** within its *PPSSPP Homebrew Store*, making it easy to enjoy the production.

---
###	How To Build and Run From Source

_This has only been tested with macOS 10.14+ (using [brew](https://brew.sh/)) and Ubuntu 20.04 LTS (Focal Fossa)_

macOS  : `brew install autoconf automake flex gcc@9 ncurses make subversion texinfo wget ppsspp`\
Ubuntu : `apt install autoconf automake bison flex gcc-9 libncurses-dev make subversion texinfo wget`

1. Bootstrap the environment using a 2006'ish snapshot of the homebrew PSP toolchain and SDK via  

	`./bootstrap.sh`

0. Build the source via

	`./build.sh`

0. Either use [psplinkusb](https://github.com/pspdev/psplinkusb), and run it via `usbhostfs_pc` using

	`./psplink.sh`  
\
or, run it emulated (assuming [`ppsspp`](https://www.ppsspp.org/) is installed) using

	`./emulate.sh`

---
###	Why Open-Source This?

Good question - and while there is not a single reason to do this there are two worth highlighting:  
Firstly we were repeatedly asked, both publicly and in private conversations, to release the source code.  
And as the code was sitting in a backup of an old Perforce repository not even we had direct access to it for a long time.
It was a good enough reason to resurrect the code and migrate it to Git.

Secondly it seems the PSP Homebrew Community is still active and, as already noted, this is as far as we can tell the best demoscene production for the PSP - almost 15 years later.  
Maybe it will inspire someone else out there to create something interesting for the PSP (or any other platform).

The source code itself is pretty much in a frozen state at this point - we don't expect PRs or further development of the production.
But it might serve useful for anyone interested in porting the code, writing emulators or perhaps working on an FPGA reimplementation of the PSP.

---

### Core Crew
![corecrew]

### Bingen, Germany, a few hours before the deadline
![coding]
Photos are courtesy of [Daywish](https://www.instagram.com/eglezioma/).

---
####	License

The source code found in this repository is licensed under the [MIT license](LICENSE) (*except where other licenses apply*).  
The source assets (*textures, meshes, etc, found under [Data](Data)*) are Copyright © 2020 and licensed under a [CC BY-SA 4.0 license](https://creativecommons.org/licenses/by-sa/4.0/).

[barbielogo]: www/barbielogo.png  "Suicide Barbie"
[psp]: www/OnDevice.jpg "Suicide Barbie on PSP1000"
[corecrew]: www/CoreCrew.jpg  "eriQue | Louie500 | ReJ"
[coding]: www/Coding.jpg "Coding in Bingen 2007"

