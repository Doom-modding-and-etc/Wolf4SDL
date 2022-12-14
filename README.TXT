Wolf4SDL by Moritz "Ripper" Kroll (http://www.chaos-software.de.vu - OFFLINE)
Original Wolfenstein 3D by id Software (http://www.idsoftware.com)
Modifications to r262 by Andy_Nonymous and others
(http://diehardwolfers.areyep.com/viewtopic.php?t=6693)
=============================================================================

Wolf4SDL is an open-source port of id Software's classic first-person shooter
Wolfenstein 3D to the cross-platform multimedia library "Simple DirectMedia
Layer (SDL)" (http://www.libsdl.org). It is meant to keep the original feel
while taking advantage of some improvements mentioned in the list below.

Discord:
----------
https://discord.gg/YNTdx5dXF6


Main features:
--------------
 - Cross-platform:
      Supported operating systems are at least:
       - Windows 98, Windows ME, Windows 2000, Windows XP(32 and 64 bit), Windows Vista(32 and 64 bit), Windows 7 (32 and 64 bit), Windows 8(32 and 64 bit) Windows 10 (32 and 64 bit) and Windows 11(64 Bits only)
       - Linux
       - BSD variants
       - Mac OS X (x86 and x64)
       - KallistiOS (used for Dreamcast)
       Only little endian platforms like x86, ARM and SH-4 are currently supported.
       - Nintendo Switch
       - Nintendo 3DS(WIP On Makefile part)
       - GP2X
       - Sega Saturn.
       - Sony Playstation 2
       - Xbox/Xbox 360(W.I.P)

 - AdLib sounds and music:
      This port includes the OPL2 emulator from MAME, so you can not only
      hear the AdLib sounds but also music without any AdLib-compatible
      soundcard in near to perfect quality!

 - Multichannel digitized sounds:
      Digitized sounds play on 8 channels! So in a fire fight you will
      always hear, when a guard opens the door behind you ;)

 - Higher screen resolutions:
      Aside from the original 320x200 resolution, Wolf4SDL currently
      supports any resolutions being multiples of 320x200 or 320x240,
      the default being 640x400.
      Unlike some other ports, Wolf4SDL does NOT apply any bilinear
      or similar filtering, so the graphics are NOT blurred but
      pixelated just as we love it.

 - Fully playable with only a game controller:
      Wolf4SDL can be played completely without a keyboard. At least two
      buttons are required (shoot/YES and open door/NO), but five or more
      are recommended (run, strafe, ESC).
	  
 - Toggle Screen Feature:
    Wolf4SDL can toggle into fullscreen if you press ALT+ENTER
 
 - Linux Wolf Library + WolfRad incorporation
	Wolf4SDL Has Linux Wolf Library + WolfRad incorporated pure wip but still useful
	for modding development
	
Additional features:
--------------------

 - Two additional view sizes:
      Wolf4SDL supports one view size using the full width of the screen
      and showing the status bar, like in Mac-enstein, and one view size
      filling the whole screen (press TAB to see the status bar).

 - (Nearly) unlimited sound and song lengths:
      Mod developers are not restricted to 64kB for digitized sounds and
      IMF songs anymore, so longer songs and digitized sounds with better
      quality are possible.

 - Resuming ingame music:
      When you come back to the game from the menu or load a save game, the
      music will be resumed where it was suspended rather than started from
      the beginning.

 - Freely movable pushwalls:
      Moving pushwalls can be viewed from all sides, allowing mod developers
      to place them with fewer restrictions. The player can also follow the
      pushwall directly instead of having to wait until the pushwall has left
      a whole tile.

 - Optional integrated features for mod developers:
      Wolf4SDL already contains the shading, directional 3D sprites,
      floor and ceiling textures, high resolution textures/sprites,
      parallax sky, cloud sky and outside atmosphere features, which
      can be easily activated in version.h.


The following versions of Wolfenstein 3D data files are currently supported
by the source code (choose the version by commenting/uncommenting lines in
version.h as described in that file):

 - Wolfenstein 3D v1.1 full Apogee
 - Wolfenstein 3D v1.4 full Apogee
 - Wolfenstein 3D v1.4 full GT/ID/Activision
 - Wolfenstein 3D v1.4 full Imagineer (Japanese)
 - Wolfenstein 3D v1.0 shareware Apogee
 - Wolfenstein 3D v1.1 shareware Apogee
 - Wolfenstein 3D v1.2 shareware Apogee
 - Wolfenstein 3D v1.4 shareware
 - Spear of Destiny full
 - Spear of Destiny demo
 - Spear of Destiny - Mission 2: Return to Danger (not tested)
 - Spear of Destiny - Mission 3: Ultimate Challenge (not tested)


How to play:
------------

To play Wolfenstein 3D with Wolf4SDL, you just have to copy the original data
files (e.g. *.WL6) into the same directory as the Wolf4SDL executable.
Please make sure, that you use the correct version of the executable with the
according data files version as the differences are hardcoded into the binary!

SDL1.2:
On Windows SDL.dll and SDL_mixer.dll must also be copied into this directory.
SDL 1.2.15 32 bit Runtimes can be acquired at: 
https://github.com/wolfysdl/SDL-Archive/raw/main/SDL-1.2.15-win32.zip and download "SDL-1.2.15-win32.zip"

SDL 1.2.15 64 bit Runtimes can be acquired at: 
https://github.com/wolfysdl/SDL-Archive/raw/main/SDL-1.2.15-win32-x64.zip and download "SDL-1.2.15-win32-x64.zip"

SDL_mixer 1.2.12 Runtimes can be acquired at:
https://github.com/wolfysdl/SDL_mixer-archive/raw/main/SDL_mixer-1.2.12-win32.zip and download "SDL_mixer-1.2.12-win32.zip"

or

SDL2:
On Windows SDL2.dll and SDL2_mixer.dll must also be copied into this directory.

SDL2 2.26.0 32 bit Runtimes can be acquired at:
https://github.com/wolfysdl/SDL-Archive/raw/main/SDL2-2.26.0-win32-x86.zip and download "SDL_mixer-2.26.0-win32-x86.zip"

SDL2 2.26.0 64 bit Runtimes can be acquired at:
https://github.com/wolfysdl/SDL-Archive/raw/main/SDL2-2.26.0-win32-x64.zip and download "SDL_mixer-2.26.0-win32-x64.zip"

SDL2_mixer 2.0.4 32 bit Runtimes can be acquired at:
https://github.com/wolfysdl/SDL_mixer-archive/raw/main/SDL2_mixer-2.6.2-win32-x86.zip and download "SDL2_mixer-2.0.4-win32-x86.zip"

SDL2_mixer 2.0.4 64 bit Runtimes can be acquired at:
https://github.com/wolfysdl/SDL_mixer-archive/raw/main/SDL2_mixer-2.6.2-win32-x64.zip and download "SDL2_mixer-2.0.4-win32-x64.zip"

deprecated links:
SDL1.2:
https://www.libsdl.org/download-1.2.php 
SDL_mixer:
https://www.libsdl.org/projects/SDL_mixer/release-1.2.html
SDL2:
https://www.libsdl.org/download-2.0.php 
SDL2_mixer:
https://www.libsdl.org/projects/SDL_mixer/

If you play in windowed mode (--windowed parameter), press SCROLLLOCK or F12
to grab the mouse. Press it again to release the mouse.

Usage:
------

Wolf4SDL supports the following command line options:
 --help                 This help page
 --tedlevel <level>     Starts the game in the given level
 --baby                 Sets the difficulty to baby for tedlevel
 --easy                 Sets the difficulty to easy for tedlevel
 --normal               Sets the difficulty to normal for tedlevel
 --hard                 Sets the difficulty to hard for tedlevel
 --nowait               Skips intro screens
 --windowed[-mouse]     Starts the game in a window [and grabs mouse]
 --res <width> <height> Sets the screen resolution
                        (must be multiple of 320x200 or 320x240)
 --resf <w> <h>         Sets any screen resolution >= 320x200
                        (which may result in graphic errors)
 --bits <b>             Sets the screen color depth
                        (Use this when you have palette/fading problem
                        or perhaps to optimize speed on old systems.
                        Allowed: 8, 16, 24, 32, default: "best" depth)
 --nodblbuf             Don't use SDL's double buffering
 --extravbls <vbls>     Sets a delay after each frame, which may help to
                        reduce flickering (SDL does not support vsync...)
                        (unit is currently 8 ms, default: 0)
 --joystick <index>     Use the index-th joystick if available
 --joystickhat <index>  Enables movement with the given coolie hat
 --samplerate <rate>    Sets the sound sample rate (given in Hz)
 --audiobuffer <size>   Sets the size of the audio buffer (-> sound latency)
                        (given in bytes)
 --ignorenumchunks      Ignores the number of chunks in VGAHEAD.*
                        (may be useful for some broken mods)
 --configdir <dir>      Directory where config file and save games are stored
                        (Windows default: current directory,
                        others: $HOME/.wolf4sdl)

For Spear of Destiny the following additional options are available:
 --mission <mission>    Mission number to play (1-3)
 --goodtimes            Disable copy protection quiz


Compiling from source code:
---------------------------

The current version of the source code is available on GitHub at:
   https://github.com/Doom-modding-and-etc/Wolf4SDL

The following ways of compiling the source code are supported:
 - Makefile for Linux, BSD variants, MinGW/MSYS, dreamcast(see README.dc), Nintendo Switch(See README.md), 
Nintendo 3DS and Sony Playstation 2. see README-dc.txt for more details
 - Visual C++ (Wolf4SDL.VC2022.sln and Wolf4SDL.VC2022.vcproj) (see README-VC.txt)
 - Special compiling for Dreamcast (see README-dc.txt)
 - Xcode (for Mac OS X, macosx/Wolf4SDL.xcodeproj/project.pbxproj)
 - Special compiling for GP2X (see README-GP2X.txt)

- Deprecated:
 - Visual C++ 2008 (Wolf4SDL.VC9.sln and Wolf4SDL.VC9.vcproj)
 - Visual C++ 2005 (Wolf4SDL.sln and Wolf4SDL.vcproj)
 - Visual C++ 6 (Wolf4SDL.dsw and Wolf4SDL.dsp)
 - Code::Blocks 8.02 (Wolf4SDL.cbp)
 - Dev-C++ 5.0 Beta 9.2 (4.9.9.2) (Wolf4SDL.dev) (see README-devcpp.txt)
 - Visual C++ 2019 (Wolf4SDL.VC2019.sln and Wolf4SDL.VC2019.vcproj) (see README-VC.txt)
 
To compile the source code you need the development libraries of
 
SDL or SDL2 (https://github.com/wolfysdl/SDL-Archive) and
SDL_mixer or SDL2_mixer (https://github.com/wolfysdl/SDL_mixer-archive)

Warning: pick the devel version

Deprecated links:
 - SDL (https://www.libsdl.org/download-1.2.php) and
 - SDL_mixer (https://www.libsdl.org/projects/SDL_mixer/release-1.2.html)
 
 or
 
 - SDL2 (https://www.libsdl.org/) and
 - SDL2_mixer (https://www.libsdl.org/projects/SDL_mixer/)

- https://github.com/wolfysdl/SDL-Archive
- https://github.com/wolfysdl/SDL_mixer-archive

and have to adjust the include and library paths in the projects accordingly.

Please note, that there is no official SDL_mixer development pack for MinGW,
yet, but you can get the needed files from a Dev-C++ package here:
http://sourceforge.net/project/showfiles.php?group_id=94270&package_id=151751
Just rename the file extension from ".devpack" to ".tar.bz2" and unpack it
with for example WinRAR. Then add the directories include/SDL and lib to the
according search paths in your project.

IMPORTANT: Do not forget to take care of version.h!
   By default it compiles for "Wolfenstein 3D v1.4 full GT/ID/Activision"!

TODOs:
------
 - Add plataform readme.
 - Fix some versions of Wolfenstein 3d.
 - Prepare to SDL3 Support.
 - Support to sega saturn: wl_draw.c, wl_main.c and wl_menu.c
 - Support OpenBSD.
 

Known bugs:
-----------
Severe: Bug on the changed key controls keys like: w a s d or something on the Wolf3d part.
Spear of destiny works file atleast.
Keyboard on menu screen won´t work after cleanup the kbrd defs.

Troubleshooting:
----------------

 - If your frame rate is low, consider using the original screen resolution
   (--res 320 200) or lowering the sound quality (--samplerate 22050)

Credits:
--------
 - Special thanks to id Software! Without the source code we would still have
   to pelt Wolfenstein 3D with hex editors and disassemblers ;D
 - Special thanks to the DOSBox team for providing a GPL'ed OPL2/3 emulator!
 - Special thanks to the MAME developement team for providing the source code
   of the OPL2 emulator!
 - Speacial thanks to AryanWolf3d for upgrade this source port
 - Ultra special many thanks to the prboom+ team for providing the c port of dosbox opl
 - Many thanks to "Der Tron" for hosting the svn repository, making Wolf4SDL
   FreeBSD compatible, testing, bugfixing and cleaning up the code!
 - Thanks to Chris Chokan for his improvements on Wolf4GW (base of Wolf4SDL)
 - Thanks to Pickle for the GP2X support and help on 320x240 support
 - Thanks to fackue for the Dreamcast support
 - Thanks to Chris Ballinger for the Mac OS X support
 - Thanks to Xilinx, Inc. for providing a list of maximum-length LFSR counters
   used for higher resolutions of fizzle fade
 - Thanks to Terraformer9x and Nexion for the advice and help for the Mixer sound problem
 - Thanks to Fabian greffrath for the patches on the code 
 - Thanks to keeganatorr for the Nintendo Switch support
 - Thanks to Fabian Sangalard for the awesoeme crt feature used on Chocolate Wolfenstein 3d. 
 - Thanks to vbt for the port of sega saturn
 - Thanks to WayneCa for upgrade the original Wolf
 - Thanks to hax0kartik for the port of Wolf4SDL to Nintendo 3DS

Licenses:
---------
 - The original source code of Wolfenstein 3D:
     At your choice:
     - license-id.txt or
     - license-gpl.txt
 - The OPL2 emulator:
     - license-mame.txt (fmopl.c)
