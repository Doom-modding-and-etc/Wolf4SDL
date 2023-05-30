/* PS2_DATA_LOADER.C */
#include "ps2_data_loader.h"

#include <ps2_printf.h>
#include <SDL_mixer.h>

Mix_Music *loader_mus;
SDL_Surface *loader_surface = NULL;
SDL_Surface *loader_image = NULL;
SDL_Surface *loader_icn = NULL;
/* SDL_Surface *loader_image_btPresses[btMax]; */
SDL_Window *loader_window = NULL;


boolean PS2_SDL_CreateWindow()
{
    int ret;
    boolean success = true;
    ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER); //Initialize Video, Audio and Joystick.
    if(ret < 0)
    {
        ps2_printf("Failed to initialize SDL", 2);
        success = false;
    }

    window = SDL_CreateWindow("Wolf4SDL-PS2 Loader By Wolf3s", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    640, 448, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if(window == NULL)
    {
        ps2_printf("Failed to create window", 2);
        success = false;
    }
    
    loader_surface = SDL_GetWindowSurface(window);
    if(loader_surface == NULL)
    {
        ps2_printf("Failed to get window surface", 2);
        success = false;
    }

    return success;
}

SDL_Surface* PS2_SDL_LoadBMP(const char* filename)
{
    SDL_Surface* imgsurface = SDL_LoadBMP(filename);
    if(!imgsurface)
    {
        ps2_printf("Failed to load image", 2);
        return imgsurface;
    }

    return imgsurface;
}

void PS2_SDL_LoaderStart()
{
    if(!PS2_SDL_CreateWindow())
    {
        printf("Failed to load the window");
    }
    else
    {
        /* Load the backgrund music. */
        Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096);
        loader_mus = Mix_LoadMUS("Loader/sounds/loader_music.wav");
        Mix_PlayMusic(loader_mus, -1); 
        
        /* Load the images in BMP only. */
        loader_image = SDL_LoadBMP("Loader/background images/loader_image.bmp");

        loader_icn = SDL_LoadBMP("Loader/background images/loader_icon.bmp");

        SDL_Rect LoaderIcnRect;

        LoaderIcnRect.x = 180;
        LoaderIcnRect.y = 0;
        LoaderIcnRect.w = 640;
        LoaderIcnRect.h = 448;

        SDL_BlitSurface(loader_image, NULL, loader_surface, NULL);
        SDL_BlitSurface(loader_icn, NULL, loader_surface, &LoaderIcnRect);

        /* TODO: Setup game controller for ps2 */
/*
        //Loader event.
        boolean loader_quit = true;

        SDL_Event event;

        while(!quit)
        {
            while(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT) //What could be SDL_QUIT on PS2?
                {
                    loader_quit = false;
                }            
                //TODO: Add sound effects when pressed the dualshock 2 buttons;
            }

        }
*/ 
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(200000);
    PS2_SDL_Shutdown();
}

void PS2_SDL_Shutdown()
{
    Mix_FreeMusic(loader_mus);
    Mix_Quit();
    SDL_FreeSurface(loader_image);
    SDL_FreeSurface(loader_surface);
    SDL_FreeSurface(loader_icn);
    SDL_Quit();
    SDL_DestroyWindow(window);
}