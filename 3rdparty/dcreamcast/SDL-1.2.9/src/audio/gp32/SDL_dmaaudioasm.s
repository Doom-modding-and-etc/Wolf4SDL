
	.global sdl_gp32_dma_audio_ISR

	.section ".text"
	.align 4
	.code  32

	.type	sdl_gp32_dma_audio_ISR, %function

sdl_gp32_dma_audio_ISR:
	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}

	bl sdl_gp32_audio_process

	ldmia	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
	subs pc, lr, #4

	.size	sdl_gp32_dma_audio_ISR, .-sdl_gp32_dma_audio_ISR
