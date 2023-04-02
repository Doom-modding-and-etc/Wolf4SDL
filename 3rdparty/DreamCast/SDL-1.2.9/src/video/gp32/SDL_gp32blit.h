#ifndef SDL_gp32blit_H
#define SDL_gp32blit_H


void SDL_gp32blit(unsigned char *source, unsigned char *dest);
void SDL_gp32blitW(unsigned short *source, unsigned short *dest);

void SDL_gp32blit240x160(unsigned char *source, unsigned char *dest);
void SDL_gp32blitW240x160(unsigned short *source, unsigned short *dest);

void SDL_gp32blit160x144(unsigned char *source, unsigned char *dest);
void SDL_gp32blitW160x144(unsigned short *source, unsigned short *dest);

#endif
