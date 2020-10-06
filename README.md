![barbielogo]
###	Suicide Barbie by The Black Lotus   
####      A PlayStation® Portable™ demo

####	Release History

* [Source code](https://github.com/theblacklotus/suicide-barbie) (MIT licensed) released December 31st, 2020.
* OE/M33 'slim' version released on July 8th, 2008.
* 'Polished' party version released on June 27th, 2007.
* Original production released at [Breakpoint 2007](https://breakpoint.untergrund.net/2007/), 1st place wild demo.
 
  :trumpet:  [pouët.net](https://www.pouet.net/prod.php?which=30284)  
  :movie_camera:  [YouTube](https://www.youtube.com/watch?v=Q1J7EBzwN3Q)

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
        Cathy / Nesnausk! for support and photojournalism in Bingen  

---

####	How To Run The Production

    ~ Copy the contents of either archive from the [releases](../../releases) page to the ms0:/PSP/GAME/ folder.  
    ~ Run the production from the XMB.  
    ~ If you're using a OE/M33 firmware, and the original 150 binaries, copy the files to ms0:/PSP/GAME150/ instead.  

This production has been tested and confirmed working under firmware 1.00, 1.50 and 3.40 OE-A (150 binary) and 3.90 M33-3 and 4.01 M33-2 (OE/M33/Slim binary)

####	How To Build And Run From Source

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
####	License

The source code found in this repository is licensed under the [MIT license](LICENSE) (*except where other licenses apply*).  
The source assets (*textures, meshes, etc, found under [Data](Data)*) are Copyright © 2020 and licensed under a [CC BY-SA 4.0 license](https://creativecommons.org/licenses/by-sa/4.0/).

---


[barbielogo]: www/barbielogo.png  "Suicide Barbie"
