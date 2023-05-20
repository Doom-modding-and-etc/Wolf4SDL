
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

#ifndef _SDL_CONFIG_LIB_H
#define _SDL_CONFIG_LIB_H

#include "SDL_rwops.h"
#include "SDL_error.h"
#include "SDL_version.h"

#include "begin_code.h"


#include "SDL_config_cfg.h"

/*

 File: Main file which contains whole SDL_Config public API.

 In function declarations you'll often see:

 CFG_String_Arg - if you're using SDL_Config with ASCII strings (ie. not Unicode), then it's equal to const char *.
 CFG_Bool - generally speaking, if you're using this library in C++ application, then it equals *bool*. Otherwise, it's normal *int*.
 As you may expect, in C++ *CFG_True* equals *true* and *CFG_False* equals *false*, while in C,
 they're equal to *1* and *0*, respectively. However, there's also a third option, SDL_bool, and a possibility of tweaking all of this by playing with <CFG_USE_BOOL>
 Sint32 - that's SDL specific typedef for a 32 bit signed integer ( typedef signed int Sint32 ), which ranges
 from -2147483648 to 2147483647 (inclusive).

 Some functions have parameters with default values that can be used only if you're compiling in C++ with
 <CFG_USE_DEFAULT_VALUES> defined.

*/


/* ------------------------------------------------------- */
/*                  Basic setup of library                 */
/* ------------------------------------------------------- */

/* Setup for C function definitions, even when using C++ */
#ifdef __cplusplus

    #include <cstdio>
    extern "C" {

#endif

#ifndef __cplusplus

   #include <stdio.h>

#endif

/* Setup for exporting functions to DLL */
#ifndef CFG_BUILD_DLL
 #undef DECLSPEC
 #define DECLSPEC
#endif

/* Macros */
/* ------------------------------------------------------- */
/*              Values returned by functions               */
/* ------------------------------------------------------- */

 /* Evaluates to true */
 #define CFG_OK 1

 /* Evaluates to false */
 #define CFG_ERROR 0

 #define CFG_CRITICAL_ERROR (-1)

 #define CFG_GROUP_CREATED 2
 #define CFG_GROUP_SELECTED 3

 #define CFG_ENTRY_CREATED 4

 /*

 They are also used internally to distinguish between the four entry types.
 
 */

 #define CFG_BOOL 1
 #define CFG_INTEGER 2
 #define CFG_FLOAT 3
 #define CFG_TEXT 4

/* ------------------------------------------------------- */
/*                Values passed to functions               */
/* ------------------------------------------------------- */

 #define CFG_GLOBAL 0
 #define CFG_SELECTED 0
 #define CFG_ENTRY_ITERATION 0

 #define CFG_SORT_ORIGINAL 1
 #define CFG_SORT_CONTENT 2
 #define CFG_SORT_TYPE_CONTENT 3

 #define CFG_SPACE_ENTRIES          1
 /*#define CFG_SPACE_ALL              2*/
 #define CFG_NO_COMMENTS            4
 #define CFG_NO_COMMENTS_ENTRIES    8
 #define CFG_NO_COMMENTS_GROUPS     16
 #define CFG_COMPRESS_OUTPUT        32

 #define CFG_ITERATE_NORMAL 1
 #define CFG_ITERATE_REVERSE (-1)


/* ------------------------------------------------------- */
/*                General library functions                */
/* ------------------------------------------------------- */

extern DECLSPEC

/*

 Function: CFG_LinkedVersion

 Returns:

 Version of the dynamically linked SDL_Config library. It may be used for checking
 that DLL is compatible with our staticly-linked library version, or just to print
 version information somewhere to user. It may be used even when you're using *staticaly* linked SDL_Config. 
 
 If you want to obtatin the compile-time version of the SDL_Config library, use macro SDL_CFG_VERSION(X).
 
 Example:
 
 (start code)
 
 const SDL_version * sdl_cfg_version = CFG_LinkedVersion();
 CFG_Char buffer[20];
 sprintf(buffer, "%u.%u.%u", sdl_cfg_version->major, sdl_cfg_version->minor, sdl_cfg_version->patch);
 // buffer contains version info
  
  (end code)
 */

const SDL_version * CFG_LinkedVersion( void );

/*
 Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
*/
#define SDL_CFG_MAJOR_VERSION	0
#define SDL_CFG_MINOR_VERSION	5
#define SDL_CFG_PATCHLEVEL  	0

/*
 This macro can be used to fill a version structure with the compile-time version of the SDL_Config library.
*/
#define SDL_CFG_VERSION(X)	    					\
 {			                						\
  (X)->major = SDL_CFG_MAJOR_VERSION;				\
  (X)->minor = SDL_CFG_MINOR_VERSION;				\
  (X)->patch = SDL_CFG_PATCHLEVEL;			    	\
 }

/*

 Defines: CFG_SetError, CFG_GetError

 We are using SDL's error handling system. Every time you get sth different from *CFG_OK*, you can obtain
 textual description in english of what has happened.

 Generally, *CFG_CRITICAL_ERROR* indicates that memory wasn't allocated or couldn't read in file.
 After this kind of error, you should stop working with affected file, since something went
 *terribly* wrong.

 On the other hand, *CFG_ERROR* means only that, the operation wasn't performed successfully
 ie. file wasn't opened properly or group wasn't found.
*/

#define CFG_SetError	SDL_SetError
#define CFG_GetError	SDL_GetError

/* ------------------------------------------------------- */

/*
 Structure: CFG_File

 This little structure is used to determine on what file we are working currently.

*/

typedef struct CFG_File
 {
   struct CFG_Internal_File * internal_file;
 } CFG_File;

/* ------------------------------------------------------- */
/*                     File operations                     */
/*                       Group: Files                      */
/* ------------------------------------------------------- */

extern DECLSPEC

/*

 Function: CFG_OpenFile

 Opens and parses file. If any groups / entries existed in file before, they will stay untouched (and new ones will be added).
 Opening file selects it.

 If you pass null (0) as filename, then new config file will be created, but initially it will be empty.
 You can once again pass it to CFG_OpenFile for parsing, but you don't have to - ie. you can fill it
 with groups and entries manually, and than save somewhere.


 Returns:

   - *CFG_OK* - file was opened and parsed without errors.
   - *CFG_ERROR* - couldn't open file (probably file doesn't exist or is opened by someone else)
   - *CFG_CRITICAL_ERROR* - couldn't allocate memory for group or file, couldn't seek in stream or
                            error occurred while reading from stream

 Notes:

  - filename may be absolute or relative, it isn't changed by library in any way
  - null pointer in file isn't checked.
  - global group is selected in file if parsing operation was successfull.

*/

 int CFG_OpenFile( CFG_String_Arg filename, CFG_File * file );

extern DECLSPEC

/*
 Function: CFG_OpenFile_RW

 The same as <CFG_OpenFile>, the only difference is that it loads file directly from SDL_RWops stream.

 Notes:

  - if there were any successfull read operations, source won't be "rewinded" to the state before them
  - stream isn't closed, you should call SDL_RWclose by yourself
  - *source* should point to already created SDL_RWops stream. Especially, it mustn't be null, or expect SIGFAULT

*/

 int CFG_OpenFile_RW( SDL_RWops * source, CFG_File * file );


extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*

 Function: CFG_SaveFile

 Saves currently selected file to filename.
 Can be called multiple times for different filenames.
 Selected group stays selected.

 There are three modes with which you can save your files (pass one of them to "mode", by default: CFG_SORT_ORIGINAL):

	- *CFG_SORT_ORIGINAL* - order of groups and entries is just like in original file. Groups and entries
      that were created by you, are saved after original ones, in order of creation.

	- *CFG_SORT_CONTENT* - by content sorted (all groups and entries are sorted alphabetically).
    - *CFG_SORT_TYPE_CONTENT* - by content sorted inside groups (groups sorted alphabetically,
      entries sorted alphabetically within groups of their own type, ie. integers, floats.)


  In all situations, entries from global group are saved first.

  "Flags" accepts following values (by default: CFG_SPACE_ENTRIES):

    - *CFG_SPACE_ENTRIES* - adds additional newline between two entries, where second one has pre comment
    - *CFG_NO_COMMENTS* - group and entry comments (both pre- and post- comments) aren't saved
    - *CFG_NO_COMMENTS_ENTRIES* - entry comments (both pre- and post- comments) aren't saved
    - *CFG_NO_COMMENTS_GROUPS* - group comments (both pre- and post- comments) aren't saved
    - *CFG_COMPRESS_OUTPUT* - removes all spaces, that otherwise would be added to improve readability etc.
                              Useful for sending config files over internet, saving them to archives, when trying
                              to minimise the amount of lost space etc. If you want to have even more condensed
                              output, combine this with *CFG_NO_COMMENTS*. Also, *CFG_SPACE_ENTRIES* is ignored
                              if *CFG_COMPRESS_OUTPUT* was passed.
  >
  You may bitwise-OR them, ie.
  > CFG_SaveFile( "config.cfg", CFG_SORT_ORIGINAL, CFG_SPACE_ENTRIES | CFG_NO_COMMENTS);

  If you don't want to use any of them, just pass as flags 0. Passing 0 as filename, performs save operation
  to the original file from which it has been loaded (the same as passing <CFG_GetSelectedFileName> as filename).

  Returns:

   - *CFG_OK* - file was successfully saved.
   - *CFG_ERROR* - couldn't create file with specified name.
   - *CFG_CRITICAL_ERROR* - passed unknown value as the *mode* parameter

*/

 int CFG_SaveFile( CFG_String_Arg filename = 0, int mode = CFG_SORT_ORIGINAL, int flags = CFG_SPACE_ENTRIES);
#else
 int CFG_SaveFile( CFG_String_Arg filename, int mode, int flags );
#endif

extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*
 Function: CFG_SaveFile_RW

 The same as <CFG_SaveFile>, the only difference is that it saves file directly to SDL_RWops stream.

 Notes:

  - *destination* should point to already created SDL_RWops stream
  - *destination* must be not null, or expect SIGFAULT
  - obviously, SDL_RWops stream that you pass as *destination*, need to support writing operation
    (seeking isn't required)
  - this function always returns *CFG_OK*
  - stream isn't closed, you should call *SDL_RWclose* by yourself

*/

 int CFG_SaveFile_RW( SDL_RWops * destination, int mode = CFG_SORT_ORIGINAL, int flags = CFG_SPACE_ENTRIES);
#else
 int CFG_SaveFile_RW( SDL_RWops * destination, int mode, int flags );
#endif

extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*

 Function: CFG_CloseFile

 If file is not null (!0), function closes file that was passed in.
 Otherwise, if file equals *CFG_SELECTED* or it is null (0), function closes the currently selected file,
 and you must select another config file before calling other functions that perform any operations
 on files/groups/entries, or you will get errors (SIGFAULTs).

 Returns:

   - *CFG_OK* - file was closed successfully.
   - *CFG_ERROR* - no file was currently selected, and you wanted to close *currently selected file*
    (that is, by passing 0).

*/

 int CFG_CloseFile( CFG_File * file = CFG_SELECTED );
#else
 int CFG_CloseFile( CFG_File * file );
#endif

extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*

 Function: CFG_ClearFile

 Removes all groups and entries from file.

 If file is not null (!0), it operates on file that was passed in.
 Otherwise, if file equals *CFG_SELECTED* or it is null, function operates on currently selected file.

 Notes:

  - file isn't closed, you can use it as usual
  - obviously, global group isn't removed, only its entries are removed
  - file that was selected, stays selected
  - if groups were successfully removed from file, global group is selected
  - you should stop iteration through <CFG_StartGroupIteration>, <CFG_SelectNextGroup> etc. if you were doing it

 Returns:

   - *CFG_OK* - file was cleared successfully.
   - *CFG_ERROR* - no file was currently selected, and you wanted to clear *currently selected file*
    (that is, by passing 0).

*/

 int CFG_ClearFile( CFG_File * file = CFG_SELECTED );
#else
 int CFG_ClearFile( CFG_File * file );
#endif

extern DECLSPEC

/*

 Function: CFG_SelectFile

 Selects this file for all further operations.

 It's up to you check if that file is correct (ie. not null), library doesn't make any checks.

 Expect SIGFAULT when you're using API functions and there's no file selected.

*/

 void CFG_SelectFile(CFG_File * file);

extern DECLSPEC

/*

 Function: CFG_GetSelectedFile

 Returns pointer to selected file.

*/

CFG_File * CFG_GetSelectedFile( void );

extern DECLSPEC

/*

 Function: CFG_GetSelectedFileName

 Returns the name of file, from which the currently selected config file has been initially loaded.

*/

CFG_String_Arg CFG_GetSelectedFileName( void );


/* ------------------------------------------------------- */
/*                    Group operations                     */
/*                      Group: Groups                      */
/* ------------------------------------------------------- */

extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*

 Function: CFG_SelectGroup

 Selects group with appropriate name in the currently selected file.
 By passing name of group that doesn't exist and CFG_True, function can be used also for creating groups.

 If group equals *CFG_GLOBAL* or null (0), then global group is selected.

 Returns:

	- *CFG_OK* - operation was successfull.
	- *CFG_GROUP_CREATED* - you tried to select group that doesn't exist, and create == CFG_True.

      Group with such name will be created. Initially it will be empty. That way, you don't need to
      worry about checking whether group exists, and if not, creating it.
      Though this isn't error, CFG_GetError() will return appropriate message, just to keep consistency in API
      (Read more in: <CFG_SetError, CFG_GetError>).

    - *CFG_ERROR* - you tried to select group that doesn't exist, and create == CFG_False. No new group was created.
       Group that was selected up to now, stays selected.

*/

 int CFG_SelectGroup(CFG_String_Arg group = CFG_GLOBAL, CFG_Bool create = CFG_True);
#else
 int CFG_SelectGroup(CFG_String_Arg group, CFG_Bool create);
#endif

extern DECLSPEC

#ifdef CFG_USE_DEFAULT_VALUES

/*

 Function: CFG_RemoveGroup

 Removes group with appropriate name.
 Removing group has this effect: all entries are removed first, then group is removed.

 When global group is selected and you try to remove it, you will remove all its entries,
 but global group itself won't be deleted.

 Passing *CFG_GLOBAL* or null (0) as group has effect of clearing global group from all entries.

 Returns:

   - *CFG_OK* - group was successfully removed
   - *CFG_ERROR* - you tried to remove group that doesn't exist

 Notes:

   - if you remove a group that was currently selected, then global group is selected automatically.

*/

 int CFG_RemoveGroup(CFG_String_Arg group = CFG_GLOBAL);
#else
 int CFG_RemoveGroup(CFG_String_Arg group);
#endif


extern DECLSPEC

/*

 Function: CFG_GroupExists

 Check if group exists in currently selected file.

 Returns:

	- *CFG_True* - group was found.
	- *CFG_False* - group wasn't found or you passed null as group name

*/

 CFG_Bool CFG_GroupExists(CFG_String_Arg group);


extern DECLSPEC

/*

 Function: CFG_GetSelectedGroupName

 Returns the name of currently selected group in currently selected file.

 Returns:

 - pointer to read-only null terminated string. You musn't change it, but you can copy its value somewhere else.
 - if global group was selected, returns null (0).


*/

 CFG_String_Arg CFG_GetSelectedGroupName( void );

extern DECLSPEC

/*

 Function: CFG_GetGroupCount();

 Returns the number of groups in currently selected file ( *not counting global group* ).

*/

 Uint32 CFG_GetGroupCount( void );

/* ------------------------------------------------------- */
/*               Individual entry operations               */
/*                     Group: Entries                      */
/* ------------------------------------------------------- */

/*

 Order of types: bool, int, float, text.

*/

extern DECLSPEC

/*

 Function: CFG_GetEntryType

 Returns the type of one particular entry:

  - CFG_INTEGER
  - CFG_FLOAT
  - CFG_BOOL 
  - CFG_TEXT
  - 0 when no entry was found

 Can be used both for single and multiple value entries.
 Searches for key in currently selected group of currently selected file.

 */

 int CFG_GetEntryType( CFG_String_Arg key );

/* ------------- Existance checking ------------- */

/*

 Functions: CFG_BoolEntryExists, CFG_IntEntryExists, CFG_FloatEntryExists, CFG_TextEntryExists

 Check if entry exists in currently selected group.

 >CFG_Bool CFG_BoolEntryExists  ( CFG_String_Arg entry );
 >CFG_Bool CFG_IntEntryExists   ( CFG_String_Arg entry );
 >CFG_Bool CFG_FloatEntryExists ( CFG_String_Arg entry );
 >CFG_Bool CFG_TextEntryExists  ( CFG_String_Arg entry );

 Returns:

	- *CFG_True* - entry was found
	- *CFG_False* - entry wasn't found or you passed null as entry name

*/

extern DECLSPEC CFG_Bool CFG_BoolEntryExists(CFG_String_Arg entry);
extern DECLSPEC CFG_Bool CFG_IntEntryExists(CFG_String_Arg entry);
extern DECLSPEC CFG_Bool CFG_FloatEntryExists(CFG_String_Arg entry);
extern DECLSPEC CFG_Bool CFG_TextEntryExists(CFG_String_Arg entry);


/* -------------- Reading entries --------------- */

/*
 Functions: CFG_ReadBool, CFG_ReadInt, CFG_ReadFloat, CFG_ReadText

 Read the value of entry with *key* name.

 > CFG_Bool        CFG_ReadBool  ( CFG_String_Arg key, CFG_Bool defaultVal       );
 > Sint32          CFG_ReadInt   ( CFG_String_Arg key, Sint32 defaultVal         );
 > float           CFG_ReadFloat ( CFG_String_Arg key, float defaultVal          );
 > CFG_String_Arg  CFG_ReadText  ( CFG_String_Arg key, CFG_String_Arg defaultVal );


 Behaviour:

   - if key wasn't found in selected group, it's searched in global group
   - if it also wasn't found in global group, new key is created with *defaultVal* in selected group,
     and *defaultVal* is returned
   - function tries to avoid confilcts by not creating another entry of different type when one already
     exists in group. In such situation, it will check both currently selected group and global group, and choose
     one that won't create conflicts.

 Notes:

   - of course, it operates on currently selected file
   - CFG_ReadText retruns pointer to original, internal string that belongs to entry, or pointer to the default one
     you passed it. So, you must copy it by yourself if you'd like to make any changes to it.

 Multiple value entry iteration:

   - pass *CFG_ENTRY_ITERATION* as key to *obtain* the current value of iterator.
     For more details, see <CFG_StartMultiValueEntryIteration>

 Returns:

   Value that was obtained from existing entry, or *defaultVal* if such entry didn't existed.
   If *CFG_ENTRY_ITERATION* was passed as key and you didn't iterate through multi value entry, returns 0.

 */

extern DECLSPEC CFG_Bool   CFG_ReadBool     ( CFG_String_Arg key, CFG_Bool defaultVal       );
extern DECLSPEC Sint32     CFG_ReadInt      ( CFG_String_Arg key, Sint32 defaultVal         );
extern DECLSPEC float      CFG_ReadFloat    ( CFG_String_Arg key, float defaultVal          );
extern DECLSPEC CFG_String_Arg CFG_ReadText ( CFG_String_Arg key, CFG_String_Arg defaultVal );

/* -------------- Writing entries --------------- */

/*

 Functions: CFG_WriteBool, CFG_WriteInt, CFG_WriteFloat, CFG_WriteText

 Change the value of entry with *key* name.
 If entry doesn't exist, it's created automatically in selected group.
 
 Multiple value entry iteration:

   - pass *CFG_ENTRY_ITERATION* as key to *change* the current value of iterator.
     For more details, see <CFG_StartMultiValueEntryIteration>


 > int CFG_WriteBool  ( CFG_String_Arg key, CFG_Bool value       );
 > int CFG_WriteInt   ( CFG_String_Arg key, Sint32 value         );
 > int CFG_WriteFloat ( CFG_String_Arg key, float value          );
 > int CFG_WriteText  ( CFG_String_Arg key, CFG_String_Arg value );

 Returns:

  - *CFG_OK* - entry existed and was updated successfully
  - *CFG_ENTRY_CREATED* - entry was created and initialized by value
  - *CFG_ERROR* - found entry differs in type (ie. it is integer and you wanted to write float) or, if you have
                  tried to write value to  

*/

extern DECLSPEC int CFG_WriteBool(CFG_String_Arg key, CFG_Bool value);
extern DECLSPEC int CFG_WriteInt(CFG_String_Arg key, Sint32 value);
extern DECLSPEC int CFG_WriteFloat(CFG_String_Arg key, float value);
extern DECLSPEC int CFG_WriteText(CFG_String_Arg key, CFG_String_Arg value);

/* -------------- Removing entries -------------- */

/*

 Functions: CFG_RemoveBoolEntry, CFG_RemoveIntEntry, CFG_RemoveFloatEntry, CFG_RemoveTextEntry

 Remove specified entry from currently selected group.

 > int CFG_RemoveBoolEntry  ( CFG_String_Arg entry );
 > int CFG_RemoveIntEntry   ( CFG_String_Arg entry );
 > int CFG_RemoveFloatEntry ( CFG_String_Arg entry );
 > int CFG_RemoveTextEntry  ( CFG_String_Arg entry );

 Returns:

  - *CFG_OK* - when such entry existed and was removed
  - *CFG_ERROR* - when such entry doesn't exist or found entry's type differed from the one that you wanted to remove

*/

extern DECLSPEC int CFG_RemoveBoolEntry(CFG_String_Arg entry);
extern DECLSPEC int CFG_RemoveIntEntry(CFG_String_Arg entry);
extern DECLSPEC int CFG_RemoveFloatEntry(CFG_String_Arg entry);
extern DECLSPEC int CFG_RemoveTextEntry(CFG_String_Arg entry);

/* ------------------------------------------------------- */
/*                 Iterating through groups                */
/*                  Group: Group iteration                 */
/* ------------------------------------------------------- */

extern DECLSPEC

/*

 Function: CFG_StartGroupIteration

 Selects first group. Global group is ignored. You can call it even when there are no groups.

 You must call this function before using <CFG_SelectNextGroup> or <CFG_IsLastGroup>, so that
 iterator initialization will be performed. Forgetting to do it may lead to some nasty bugs!

 Iterating through groups:

 (start code)

  for ( CFG_StartGroupIteration(); !CFG_IsLastGroup(); CFG_SelectNextGroup() )
   {
    // now you can use all functions that operate on currently selected group
   }

 (end code)
*/

 void CFG_StartGroupIteration( void );


extern DECLSPEC

/*

 Function: CFG_SelectNextGroup

 Selects next group, in *alphabethical* order.  Likewise <CFG_StartGroupIteration>, global group is ignored
 (it won't be selected ever when iterating through groups).

 For more informations about iteration, see <CFG_StartGroupIteration>.

 Additional functionality:

 If someone needs the ability to iterate through groups in order, in which they were found in the original file,
 then sorry, but currently there's no way to do it. If there is any public appeal for such
 functionality, I may implement it so that you could just pass additional flag to <CFG_SetIterationDirection>.
 The reason why it's wasn't implemented in the first place, is simple - apart from new flag, it would require also
 additional function, which would be used only in this case.

*/

 void CFG_SelectNextGroup( void );


extern DECLSPEC

/*

 Function: CFG_IsLastGroup

 Returns CFG_True if you have reached *past the end* of all groups (just like STL containers member function end() ).

 For more informations about iteration, see <CFG_StartGroupIteration>.

*/

 CFG_Bool CFG_IsLastGroup( void );

extern DECLSPEC

 /*

 Function: CFG_RemoveSelectedGroup

 Removes currently selected group.
 Removing group has this effect: all entries are removed first, selected group is removed,
 then next group is selected (in normal alphabethical order, just like when iterating).

 Very important:

 This function can be used only when iterating through groups. Using it to groups selected by
 <CFG_SelectGroup> etc. is not supported currently, and will lead to errors. If there is any public appeal for such
 functionality, I may reimplement it so that you could use it also in other situations. The reason why it's wasn't
 implemented in the first place, is simple - it would be rather hard / tedious to do it.
 Now in such situations just call CFG_RemoveGroup( CFG_GetSelectedGroupName() ); and be done with it :-) though
 remember to cache result of CFG_GetSelectedGroupName() since calling it along with CFG_RemoveSelectedGroup()
 many times would incur serious performance overhead.

 When iterating over groups and removing them, do it like this:

 (start code)

 for ( CFG_StartGroupIteration(); !CFG_IsLastGroup(); )
   {
    if (sth)          
     {
      CFG_RemoveSelectedGroup();      
     }
    else
     {    
      CFG_SelectNextGroup();
     }
   }

 (end code)

*/

 void CFG_RemoveSelectedGroup( void );


extern DECLSPEC

 /*

 Function: CFG_SetIterationDirection

 Sets the direction of iteration for the currently selected file. Shouldn't be called in the middle of iteration.

 Direction accepts following values:

  - *CFG_ITERATE_NORMAL* - normal iteration, in forward, from first to last group
  - *CFG_ITERATE_REVERSE* - reverse iteration, from last to first group
  - *0* - don't change direction of iteration, useful for returning current value without changing anything.

 _By default, CFG_ITERATE_NORMAL is selected._

 Returns:
 Old direction of iteration (*CFG_ITERATE_NORMAL* or *CFG_ITERATE_REVERSE*)

 */

  int CFG_SetIterationDirection( int direction );

/* ------------------------------------------------------- */
/*              Multiple value entry iteration             */
/*                Group: Multi value entry                 */
/* ------------------------------------------------------- */

 extern DECLSPEC

 /*

 Function: CFG_IsEntryMultiValue 

 Returns true if key has multiple values. Searches for key in currently selected group of currently selected file.
 If no such value exists, function returns false.

 */

  CFG_Bool CFG_IsEntryMultiValue( CFG_String_Arg key );

 extern DECLSPEC

 /*

 Function: CFG_StartMultiValueEntryIteration

 After calling, it selects key for the iteration over its values. 
 Key is in currently selected group of currently selected file.

 If specified entry doesn't exist, whole iteration will fail after first <CFG_IsLastMultiValueEntry>.
 If you select one group and start iteration over values of one of its multiple value entries, then select another
 group and also start multiple value entry iteration - then those two iterations are separate, you can freely
 switch between the groups and iterators won't mess up.

 If your multi value entry occurs to have only one value ;-) iteration will stil work as expected.


 Iterating through multiple value entries:

 (start code)

 for ( CFG_StartMultiValueEntryIteration("entry"); !CFG_IsLastMultiValueEntry(); CFG_SelectNextMultiValueEntry() )
   {
    // now you can use those functions to operate on entries:
    // reading: CFG_Read*(CFG_ENTRY_ITERATION, 0);    
    // writing: CFG_Write*(CFG_ENTRY_ITERATION, value);
   }

 (end code)

 */

  void CFG_StartMultiValueEntryIteration( CFG_String_Arg key );


 extern DECLSPEC

 /*

 Function: CFG_IsLastMultiValueEntry

 Returns true if it's last value in current multiple value list iteration (similiar to STL containters member function .end())

 */

  CFG_Bool CFG_IsLastMultiValueEntry( void );


 extern DECLSPEC

 /*

 Function: CFG_SelectNextMultiValueEntry

 Selects next value in current multiple value list iteration.

 */

  void CFG_SelectNextMultiValueEntry( void );

 extern DECLSPEC

 /*

 Function: CFG_RemoveMultiValueEntry

 Removes currently selected entry from multiple value list iteration.
 When you use it, don't call <CFG_SelectNextMultiValueEntry> in for loop, since it automatically selects next entry
 after removal.
 
 So, you would be iterating like this:

 (start code)

 for ( CFG_StartMultiValueEntryIteration("entry"); !CFG_IsLastMultiValueEntry(); )
   {
    if (sth)
     {
      CFG_RemoveMultiValueEntry();
     }
    else 
     {
      CFG_SelectNextMultiValueEntry();     
     }
   }

 (end code)

 */

  void CFG_RemoveMultiValueEntry( void );


/* ------------------------------------------------------- */

#ifdef __cplusplus

   } /* ends extern "C" */

#endif

#include "close_code.h"

#endif /* file inclusion guard */
