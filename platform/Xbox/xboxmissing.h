#ifndef __XBOXMISSING_H__
#define __XBOXMISSING_H__

#ifdef  __cplusplus
extern "C" {
#endif

char* getenv(const char* szKey);
int putenv(const char* szKey);
void XExit(void);
void backslash(char *s);
void XFreeMem(void);

#ifdef  __cplusplus
}
#endif
#endif /* __XBOXMISSING_H__ */
