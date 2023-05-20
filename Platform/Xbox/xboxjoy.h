#ifndef __XBOXJOY_H__
#define __XBOXJOY_H__

#include <SDL.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern int xpos;
extern int ypos;
extern int xstretch;
extern int ystretch;
extern int flickerfilter;
extern int m720p;

void XboxInitVideo (int flags);
void XboxSetScreen (void);

#ifdef  __cplusplus
}
#endif
#endif /* __XBOXJOY_H__ */
