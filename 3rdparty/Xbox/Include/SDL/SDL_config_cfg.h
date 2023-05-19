
/*

    _____ ____  __       ______            _____
   / ___// __ \/ /      / ____/___  ____  / __(_)___ _
   \__ \/ / / / /      / /   / __ \/ __ \/ /_/ / __  /
  ___/ / /_/ / /___   / /___/ /_/ / / / / __/ / /_/ /
 /____/_____/_____/   \____/\____/_/ /_/_/ /_/\__, /
                                             /____/


 Library for reading and writing configuration files in an easy, cross-platform way.

 - Author: Hubert "Koshmaar" Rutkowski
 - Contact: koshmaar@poczta.onet.pl
 - Version: 0.5
 - Homepage: http://sdl-cfg.sourceforge.net/
 - License: GNU LGPL

*/


/*
 File: File which controls how SDL_Config library will be compiled.
 
 First, you should know, that in order to change behaviour of SDL_Config, you must make changes directly in _SDL_config_cfg.h_ file,
 then recompile SDL_Config, and finally recompile your application. For the list of things that can be tweakeaed, see below.

*/

/* ----------------------- STRING VERSION */

/*
 Define: CFG_GCC_STRING_PROBLEMS

 Defining CFG_GCC_STRING_PROBLEMS may be useful when you're compiling C programs on GCC/MinGW.
 If you will run onto linking problems, try uncommenting it, maybe it will help.

 Unfortunately, I tried it on MinGW 3.5 and it wasn't linkining properly due to string::substr().

 *By default, not defined.*
*/

/*#define CFG_GCC_STRING_PROBLEMS*/
#ifdef CFG_GCC_STRING_PROBLEMS

  #include <cstdlib>

  #include <ext/malloc_allocator.h>

  #include <string>

  typedef std::basic_string<char, std::char_traits<char>, __gnu_cxx::malloc_allocator<char> > CFG_Internal_String;

  #define CFG_Internal_String_Arg const CFG_Internal_String &

  #define CFG_String CFG_Internal_String

#else

  #define CFG_Internal_String_Arg const std::string &

  #define CFG_String std::string

#endif


#define CFG_Char char

/* Used in public function declarations */

#define CFG_String_Arg const CFG_Char *


/* ----------------------- LIB BEHAVIOUR */

#ifdef _MSC_VER /* VisualC++ */

 #define CFG_COMPILER_VISUALC
 #define CFG_ASM_INT3 __asm { int 3 }

#else /* probably GCC */

 #define CFG_COMPILER_GCC
 #define CFG_ASM_INT3 asm("int3");

#endif

/*

 Define: CFG_BUILD_DLL

 Defines that library will be build as dynamically linked library.
 If it's not defined, static version will be build.

 *Important note*: in order to compile DLL version you also have to define CFG_USE_DLL somewhere in the 
 SDL_Config project's options (ie. "C/C++ -> Preprocessor -> Preprocessor definitions" in Visual Studio) if you're not using official SDL_Config project file.

 *By default, defined.*

 */


#define CFG_BUILD_DLL

/*

 Define: CFG_PROFILE_LIB

 If defined, library will generate output to stdout about how much miliseconds it took to Open/Save files.
 Beware that you must call SDL_Init(SDL_INIT_TIMER) before calling any SDL_Config functions and SDL_Quit()
 at end of program if you are using this functionality (unless you're doing this already). It uses SDL's SDL_GetTicks()
 timer which usually has a few milisecond precision.

 *By default, not defined.*

*/

/*#define CFG_PROFILE_LIB*/

/*

 Define: CFG_REMOVE_GROUP_SPACES

 If defined, spaces from within group declarations will be removed, otherwise they will stay untouched, ie.

> [ my_group ] // if CFG_REMOVE_GROUP_SPACES defined, you will get "my_group"
> [ my_group ] // if CFG_REMOVE_GROUP_SPACES not defined, you will get " my_group "

 *By default, defined.*

*/

#define CFG_REMOVE_GROUP_SPACES


/*

 Define: CFG_USE_MULTIMAP

 If defined, you can keep multiple groups with the same name in one file.
 To access them, you have to iterate over all groups.

 *By default, not defined.*

*/

/*#define CFG_USE_MULTIMAP*/

/*
 What string version use in function definitions and internally, when parsing.

 _Currently not supported._
*/

#define CFG_USE_STD_STRINGS
/*#define CFG_USE_UNICODE_STRINGS*/

/*                    
 Define: CFG_USE_BOOL
 
 Native bool support is maintained for people using C++. It is assumed that if you are compiling in C++,
 you want to use bool, and in this way those defintions are constructed. However, you may choose
 to use int instead - just change CFG_USE_BOOL value to *1*.

 If you want to use SDL_Config from within C code, change CFG_USE_BOOL value to *1* before compiling the library.

 There's also third option, SDL_bool, for details consult SDL documentation.

 Summary:

 0 - C++ bool (default)
 1 - int
 2 - SDL_bool

 *By default, 0.*
*/

 #define CFG_USE_BOOL 0

#if CFG_USE_BOOL == 0

 #define CFG_Bool bool
 #define CFG_True true
 #define CFG_False false

#elif CFG_USE_BOOL == 1

 #define CFG_Bool int
 #define CFG_True 1
 #define CFG_False 0

#elif CFG_USE_BOOL == 2

 #define CFG_Bool SDL_bool
 #define CFG_True SDL_TRUE
 #define CFG_False SDL_FALSE

#endif

/*

 Define: CFG_USE_DEFAULT_VALUES

 It controls what version of function definitions will be compiled: those with or without default values.
 Obviously, default values are supported and can be used only in C++, so if you're compiling with C, undefine
 CFG_USE_DEFAULT_VALUES before compilation of SDL_Config.
 
 *By default, defined.*

*/

#define CFG_USE_DEFAULT_VALUES

/*

 Used only by CFG_Internal_isspace, you should comment second defition only when your compiler
 doesn't support inlines and issues errors.

*/

//#define CFG_INLINE 
#define CFG_INLINE inline

/* ---------------------- OPERATORS and KEYWORDS */

/*

 You should be extremely careful when changing those characters.

*/

#define CFG_OPERATOR_GROUP_START '['
#define CFG_OPERATOR_GROUP_END ']'

#define CFG_OPERATOR_EQUALS '='
#define CFG_OPERATOR_DBL_QUOTE '"'

#define CFG_OPERATOR_ESCAPE_SEQ '\\'

#define CFG_MULTIPLE_VALUE_SEPARATOR ','

/* ---------------------- BOOLS */

/* You can freely change those, but make sure that they can't be categorized as:

	1. Integer - don't use only digits inside
	2. Float - don't use only digits and one dot inside
	3. Text - don't use CFG_OPERATOR_DBL_QUOTE characters at start and end

   Remember that they're case insensitive.
 */

#define CFG_KEYWORD_TRUE_1 "true"
#define CFG_KEYWORD_FALSE_1 "false"

#define CFG_KEYWORD_TRUE_2 "yes"
#define CFG_KEYWORD_FALSE_2 "no"

#define CFG_KEYWORD_TRUE_3 "on"
#define CFG_KEYWORD_FALSE_3 "off"

/* ---------------------- COMMENTS */

/*
  Beware: CFG_COMMENT_1 is always treated as two chars (that means, ie. if CFG_COMMENT_1 == / then //
  will be recognized as comment) and this behaviour can't be easily changed without rewriting
  many places of the library.
*/

#define CFG_COMMENT_1 '/'
#define CFG_COMMENT_2 '#'
#define CFG_COMMENT_3 ';'

/* C like comment (the same that you see in _this_ line) */

#define CFG_COMMENT_C1 '/'
#define CFG_COMMENT_C2 '*'


/* ---------------------- DEBUGGING */


/*

 Define: CFG_DEBUG_LIB

 If defined, library will be sending debugging output to stderr.

 *By default, it's not defined.*

*/

/*#define CFG_DEBUG_LIB*/

/*

 Define: CFG_ASSERT_NULL_ENABLED

 When defined, if the pointer to currently selected file equals 0 at any point where it would
 cause mysterious SEGFAULTS etc. this will happen: to sderr.txt will be outputted such an information:

 (start code)
 Application which uses SDL_Config library (version: %VERSION) has created
 an errorneus situation: pointer to selected file is equal to NULL (0) at
 %LINE line of file %FILE, so that function %FUNCTION can't continue.
 (end code)

 Also, application will be immediately terminated through calling assembler interrupt 3.

 *By default, it's defined.*

 Note:
 If *__FUNCTION__* macro isn't supported by your compiler, you may run into errors
 when compiling library with *CFG_ASSERT_NULL_ENABLED*. However, it should be supported by all
 modern compilers (ie. GCC, VisualC++).
*/

#define CFG_ASSERT_NULL_ENABLED

#ifdef CFG_DEBUG_LIB
 #ifdef CFG_ASSERT_NULL_ENABLED
  #define CFG_ASSERT_NULL if (CFG_Internal_selected_file == 0)\
                           { CFG_Internal_Assert_Null(__FUNCTION__, __FILE__, __LINE__); CFG_ASM_INT3 }
 #else
  #define CFG_ASSERT_NULL
 #endif
#else
 #define CFG_ASSERT_NULL
#endif


#ifdef CFG_DEBUG_LIB
 #define CFG_MAX_INTEGER_WARNING {CFG_DEBUG("Warning: detected integer value (%s) doesn't fit into [ -2147483648 .."\
                                           " 2147483647 ] range.\n         Changed its type to text.\n""\
                                           ", _entry.c_str());}

 #define CFG_DEBUG(x, y) fprintf(stderr, x, y);
#else
 #define CFG_MAX_INTEGER_WARNING
 #define CFG_DEBUG(x, y) ;
#endif


/*

 Critical library errors:

  001 - CFG_ReadBool()
  002 - CFG_Internal_RWfind()
  003 - CFG_GetSelectedGroupName()
  004 - CFG_StartGroupIteration(), CFG_SelectNextGroup, CFG_IsLastGroup, CFG_RemoveSelectedGroup

*/

#ifdef CFG_DEBUG_LIB
 #define CFG_LOG_CRITICAL_ERROR(x) fprintf(stderr,"CRITICAL ERROR NO. %i - if you see this, please mail" \
                                              " about it to: koshmaar@poczta.onet.pl", x);
#else
 #define CFG_LOG_CRITICAL_ERROR(x) SDL_SetError("CRITICAL ERROR NO." #x );
#endif
