## Wolf4SDL-PS2

Official quick support of Wolf4SDL source port to the Sony´s Playstation 2 made by André Guilherme
it´s under construction but you can still comipile and contribute.

[Demonstration video](https://www.youtube.com/watch?v=H0uJxuxbpu0)

# Screenshots:

None at the moment.

## Controls:

None at the moment.

## Compiling

Firstly you to install the latest version of [ps2sdk](https://github.com/ps2dev/ps2sdk) + [ps2sdk-ports(With SDL2 and SDL2_mixer)](https://github.com/ps2dev/ps2dev) however use this [shell script](https://github.com/ps2dev/ps2dev.git) to automate the process

then you go to the Wolf4SDL dir and hit

```
$ make all -f Makefile.ps2 
```

This should generate a bin Called:

```
Wold4SDL.elf
```

## Instalattion:

Copy this Wolf4SDL.elf and Copy the whole ``.wl6`` or ``.sod`` data into a folder called ``data/``
then use pcsx2 to start the game

## Known issues:

- data is not loaded properly
- modules they are not starting
- the game won´t run on real hardware.

## TODO´S:

- Change keyboard pads.
- Support the usb/hdd loading data files.
- Figure out why the modules are sucking to load :(
- ~~Switch u32 to uint32_t for major compatbility~~
