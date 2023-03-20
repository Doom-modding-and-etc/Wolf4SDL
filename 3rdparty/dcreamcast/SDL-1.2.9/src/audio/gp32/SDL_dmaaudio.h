#ifndef SDL_DMAAUDIO_H_INCLUDED
#define SDL_DMAAUDIO_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GP32_DMA_AUD_ERR_STARTED	   (-1)
#define GP32_DMA_AUD_ERR_MEMORY		   (-2)

int sdl_gp32_dma_audio_start (int nRate, int nBufferSamples, int nReverse);
void sdl_gp32_dma_audio_stop (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SDL_DMAAUDIO_H_INCLUDED */
