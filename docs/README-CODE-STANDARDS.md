# CODING RULES(Last reviewed on: 07:22:2023)


- Use C89(Aka: ANSI C) coding standard
  Reason: if you use standard the C99/C11/C17 when you are porting to another platform you are difficulting the portability for other systems, for example Commodore64 which the compiler probably supports only C89, Although this can might be problem since Wolf4SDL before of the C89 port([see commit](8cb38d9)) 
  uses C++ functions, so if you want to use other standard for your desired platform, please use the compiler flags of the standard and if your compiler supports C99 or C89 please avoid the use the C11 and C17 standard.
- don´t use type defintions including: `int8_t`, `int16_t`, `int32_t`, `int64_t` and etc
  Reason: same as the second one, if you use type definition please go to `wl_def.h` or `id_w3swrap.h` for abstract the type definition,
  example: `/* Type definition of short errno_t(the most hated type) */ typedef int errno_t;` you see that it can makes easier when you manually type the type definition, it makes easier to the compiler see the type itself without using inclusion headers, so if you plan to use types use it however define it first or preference avoid the usage of types.
- Use ` #ifdef ` when you are porting to a new desired function
  Reason: when are you implementing/porting another you must use `#ifdef` like this:

  ```
  #ifdef SAMPLE_PLAT
  /* Put your ported code here: */
  #else
  /*Put the original code which you ported to your platform
  #endif
  ```

  but if you want to use `#ifndef`, use like this:
  `#ifndef SAMPLE_PLAT`
  `/* The code that doesn´t work on your platform */`
  `#endif`
  because if you use like this:

  ```
  #ifndef SAMPLE_PLATFORM
  /* Original code that doesn´t work on your platform */
  #else
  /* Code ported to your platform standards */
  #endif
  ```

  you can easily make the code lazy and not easier depending of the situation for the programmer.
  Q: but how i can port since there´s other `#ifdef` in place? well you must use `#if defined(SAMPLE_PLATFORM)` and then
  for your desired platform you use `#elif defined(SAMPLE_PLATFORM)` or if you do this you´re done!!!
- Avoid C++ Source files/Code
  Reason: i don´t like seeing c++ code on Wolf4SDL even if it was ported from Wolf4SDL 1.9 or Less, this the reason why i hate the wolf3d community they prefer on easy solution they should instead go and figure out the problem and solve it instead of porting the whole code based in c to C++99, 03 and 11 in some parts this why many developers/programmers including [linuxwolf](https://github.com/linuxwolf6), this is fault of ripper which on the time doesn´t have the proper knowledge about this detail.
  so to not commit the same mistake instead don´t use C++ files except if you wrap a .h file with c content but the source file fill with c++ wrapped, you can also if your platform supports only C++ you can easily compile as C++ instead of porting the whole code to C++ standards.
- When are you creating a file for your specific platform use the Platform dir for creating it
  Reason: don´t create file for your specific platform on the project root instead go to platform and create a folder with the platform name and write it and then include into wl_def.h or any file on the root dir.
- Use the platform prefix while creating a function or encapsulated function for your platform.
  Reason: This makes easier to identify what platform are been ported in and keeps the standards of id tech 0 engine
  here as example:
  ` platform_sample.h `

  ```
  #ifndef PLATFORM_SAMPLE_H
  #define PLATFOMR_SAMPLE_H
  #include <platform_header.h>
  void PLATFORM_functionx();
  boolean PLATFORM_functiony();
  #endif
  ```

  if you do this you´re helping the others identify what platform are you porting into your project.

  That´s all folks now let´s contribute :)