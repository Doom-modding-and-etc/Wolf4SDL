//Wolf3s: Rewritten SDL1 code
#include <SDL.h>
#if SDL_MAJOR_VERSION == 2
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    
    screenBits = SDL_GetRendererInfo(renderer, info);

    if (screenBits)
    {
        SDL_SetSurfacePalette(screen, palette);
        info->name;
    }

    if (window == NULL)
    {
        printf("Unable to set %ix%ix video mode: %s\n", screenWidth, screenHeight, SDL_GetError());
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    if ((screen->flags & SDL_GL_DOUBLEBUFFER) != SDL_GL_DOUBLEBUFFER)
    {
        usedoublebuffering = false;
    }
    
    SDL_ShowCursor(SDL_DISABLE);
    
    SDL_SetPaletteColors(palette, gamepal, 0, 256);
    
    screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth,
        screenHeight, screenBits, 0, 0, 0, 0);
    
    if (!screenBuffer)
    {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetColorKey(screenBuffer, NULL, gamepal);
#else

window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
    (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL);

SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888, &screenBits, &r, &g, &b, &a);

screen = SDL_CreateRGBSurface(0, screenWidth, screenHeight, screenBits, r, g, b, a);

if (!screen)
{
    printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth, screenHeight, screenBits, SDL_GetError());
    exit(1);
}

renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

SDL_ShowCursor(SDL_DISABLE);

SDL_SetPaletteColors(screen->format->palette, gamepal, 0, 256);
memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

screenBuffer = SDL_CreateRGBSurface(0, screenWidth,
    screenHeight, 8, 0, 0, 0, 0);
if (!screenBuffer)
{
    printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
    exit(1);
}
SDL_SetPaletteColors(screenBuffer->format->palette, gamepal, 0, 256);

texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    screenWidth, screenHeight);

#endif //SDL_MAJOR_VERSION == 2

#if SDL_MAJOR_VERSION == 2
     //SDL_SetPalette(screen, SDL_PHYSPAL, &col, color, 1);
      SDL_SetPaletteColors(palette, &col, 1, 0);
    else
    {
    //SDL_SetPalette(screenBuffer, SDL_LOGPAL, &col, color, 1);
    SDL_SetPaletteColors(palette, &col, 1, 0);
    SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

    SDL_RenderPresent(renderer);
#else
        SDL_SetPaletteColors(palette, &col, 1, 0);
    else
    {
    SDL_SetPaletteColors(palette, &col, 1, 0);
    SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screenBuffer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    }
#endif

*WIP
#if SDL_MAJOR_VERSION == 2
        SDL_SetPaletteColors(wpalette, palette, 0, 256);
    else
    {
        SDL_SetPaletteColors(wpalette, palette, 0, 256);
        if (forceupdate)
        {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

            SDL_RenderCopy(renderer, texture, NULL, NULL);

            SDL_RenderPresent(renderer);
#else
        SDL_SetPaletteColors(wpalette, palette, 0, 256);
    else
    {
        SDL_SetPaletteColors(wpalette, palette, 0, 256);

        if (forceupdate)
        {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
#endif //SDL_MAJOR_VERSION == 2
        }
    }
*/
#if SDL_MAJOR_VERSION == 1
                SDL_WM_GrabInput(GrabInput ? SDL_GRAB_ON : SDL_GRAB_OFF);
#elif SDL_MAJOR_VERSION == 2
                SDL_SetRelativeMouseMode(GrabInput ? SDL_TRUE : SDL_FALSE);
#endif
//#if SDL_MAJOR_VERSION == 1
//        SDL_WM_GrabInput(SDL_GRAB_ON);
//#elif SDL_MAJOR_VERSION == 2
 //       SDL_SetRelativeMouseMode(SDL_TRUE);
//#endif
#if SDL_MAJOR_VERSION == 2
        SDL_GetWindowGrab(window);
#else
        SDL_SetWindowMouseGrab(window, SDL_TRUE);
#endif
*/
