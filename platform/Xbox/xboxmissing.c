#undef FindFirstFileA

#include <direct.h>
#include <io.h>
#include <malloc.h>
#include <stdio.h>
#include <xtl.h>
#include "xboxconfig.h"

#undef chdir

int chdir(const char* szKey)
{
	return 0;
}

char* getenv(const char* szKey)
{
	return NULL;
}

int putenv(const char* szKey)
{
	return 0;
}

char* getcwd(const char* szKey)
{
	return 0;
}

void XExit(void)
{
	XboxSaveConfig(); 
	XLaunchNewImage(NULL,NULL);
}

void backslash(char *s)
{
	while(*s)
	{
		if(*s == '/')
		*s = '\\';
		s++;
	}
}  

char *xbox_renamer(const char *oldname)
{
	static char newname[512];

	memset(newname, 0, sizeof(newname));
	if (oldname[0] == '.') sprintf(newname, "D:%s", ++oldname);
	 else if (oldname[1] == ':') strcpy(newname, oldname);
	  else if ((oldname[0] == '/') || (oldname[0] == '\\')) sprintf(newname, "D:%s", oldname);
	   else sprintf(newname, "D:\\%s", oldname);

	backslash(newname);
	return(newname);
} 

#undef fopen

FILE *fopex(const char *file, const char *mode)
{
	return fopen(xbox_renamer(file), mode);
}

#undef open

int opex(const char *path, int mode)
{
	return _open(xbox_renamer(path), mode);
}

#undef mkdir

int mkdix(const char *dir)
{
	return _mkdir(xbox_renamer(dir));
}

#undef unlink

int unlinx(const char *file)
{
	return _unlink(xbox_renamer(file));
}

#undef stat

int stax (const char *path, struct stat *buffer)
{
	return _stat(xbox_renamer(path), buffer);
}


WINBASEAPI HANDLE WINAPI FindFirstFilex(IN LPCSTR lpFileName, OUT LPWIN32_FIND_DATA lpFindFileData)
{
	return FindFirstFile(xbox_renamer(lpFileName), lpFindFileData);
}

void exix (int code)
{
	XExit();
}

void XFreeMem(void)
{
	MEMORYSTATUS stat;
	static char         g_debugLoggerString[1024];

    /* Get the memory status. */
    GlobalMemoryStatus( &stat );
	sprintf(g_debugLoggerString, "Free Mem = %d\n", (int)(stat.dwAvailPhys));
	OutputDebugString(g_debugLoggerString);
}

