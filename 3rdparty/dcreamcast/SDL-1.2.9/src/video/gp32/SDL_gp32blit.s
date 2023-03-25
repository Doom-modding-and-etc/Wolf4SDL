	.text
	.align	2
	.global	SDL_gp32blit, SDL_gp32blitW
	.global SDL_gp32blit240x160, SDL_gp32blitW240x160
	.global SDL_gp32blit160x144, SDL_gp32blitW160x144
	.type	SDL_gp32blit,function
	.type	SDL_gp32blitW,function
	.type	SDL_gp32blit240x160,function
	.type	SDL_gp32blitW240x160,function
	.type	SDL_gp32blit160x144,function
	.type	SDL_gp32blitW160x144,function


SDL_gp32blit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.

	mov	r2, #239
otro:
	add	r1, r1, r2
	ldrb	r3, [r0, #0]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #1]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #2]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #3]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #4]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #5]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #6]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #7]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #8]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #9]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #10]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #11]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #12]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #13]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #14]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #15]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #16]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #17]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #18]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #19]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #20]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #21]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #22]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #23]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #24]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #25]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #26]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #27]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #28]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #29]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #30]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #31]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #32]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #33]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #34]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #35]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #36]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #37]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #38]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #39]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #40]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #41]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #42]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #43]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #44]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #45]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #46]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #47]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #48]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #49]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #50]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #51]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #52]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #53]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #54]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #55]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #56]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #57]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #58]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #59]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #60]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #61]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #62]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #63]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #64]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #65]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #66]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #67]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #68]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #69]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #70]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #71]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #72]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #73]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #74]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #75]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #76]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #77]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #78]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #79]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #80]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #81]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #82]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #83]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #84]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #85]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #86]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #87]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #88]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #89]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #90]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #91]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #92]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #93]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #94]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #95]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #96]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #97]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #98]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #99]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #100]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #101]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #102]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #103]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #104]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #105]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #106]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #107]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #108]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #109]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #110]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #111]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #112]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #113]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #114]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #115]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #116]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #117]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #118]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #119]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #120]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #121]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #122]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #123]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #124]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #125]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #126]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #127]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #128]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #129]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #130]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #131]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #132]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #133]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #134]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #135]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #136]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #137]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #138]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #139]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #140]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #141]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #142]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #143]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #144]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #145]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #146]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #147]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #148]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #149]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #150]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #151]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #152]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #153]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #154]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #155]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #156]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #157]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #158]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #159]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #160]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #161]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #162]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #163]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #164]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #165]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #166]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #167]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #168]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #169]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #170]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #171]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #172]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #173]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #174]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #175]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #176]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #177]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #178]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #179]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #180]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #181]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #182]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #183]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #184]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #185]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #186]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #187]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #188]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #189]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #190]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #191]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #192]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #193]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #194]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #195]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #196]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #197]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #198]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #199]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #200]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #201]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #202]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #203]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #204]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #205]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #206]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #207]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #208]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #209]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #210]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #211]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #212]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #213]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #214]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #215]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #216]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #217]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #218]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #219]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #220]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #221]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #222]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #223]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #224]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #225]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #226]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #227]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #228]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #229]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #230]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #231]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #232]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #233]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #234]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #235]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #236]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #237]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #238]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #239]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #240]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #241]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #242]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #243]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #244]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #245]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #246]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #247]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #248]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #249]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #250]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #251]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #252]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #253]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #254]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #255]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #256]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #257]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #258]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #259]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #260]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #261]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #262]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #263]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #264]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #265]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #266]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #267]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #268]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #269]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #270]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #271]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #272]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #273]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #274]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #275]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #276]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #277]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #278]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #279]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #280]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #281]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #282]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #283]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #284]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #285]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #286]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #287]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #288]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #289]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #290]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #291]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #292]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #293]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #294]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #295]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #296]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #297]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #298]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #299]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #300]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #301]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #302]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #303]
	strb	r3, [r1, #3600]
	add	r1,r1,#3840

	ldrb	r3, [r0, #304]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #305]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #306]
	strb	r3, [r1, #480]
	ldrb	r3, [r0, #307]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #308]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #309]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #310]
	strb	r3, [r1, #1440]
	ldrb	r3, [r0, #311]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #312]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #313]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #314]
	strb	r3, [r1, #2400]
	ldrb	r3, [r0, #315]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #316]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #317]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #318]
	strb	r3, [r1, #3360]
	ldrb	r3, [r0, #319]
	strb	r3, [r1, #3600]

	sub	r1, r1, #65536
	sub	r1, r1, #7424
	sub	r1, r1, r2
	add	r0, r0, #320
	subs	r2, r2, #1
	bxlt	lr
	b	otro

SDL_gp32blitW:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.


	mov     r2, #480
	sub	r2, r2, #2
otroW:
	add     r1, r1, r2
	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #128]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #130]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #132]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #134]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #136]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #138]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #140]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #142]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #144]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #146]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #148]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #150]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #152]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #154]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #156]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #158]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #160]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #162]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #164]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #166]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #168]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #170]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #172]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #174]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #176]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #178]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #180]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #182]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #184]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #186]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #188]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #190]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #192]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #194]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #196]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #198]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #200]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #202]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #204]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #206]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #208]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #210]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #212]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #214]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #216]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #218]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #220]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #222]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #224]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #226]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #228]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #230]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #232]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #234]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #236]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #238]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #240]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #242]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #244]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #246]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #248]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #250]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #252]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #254]
	strh	r3, [r1]
	add	r0,r0,#256
	add	r1,r1,#480

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #128]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #130]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #132]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #134]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #136]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #138]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #140]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #142]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #144]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #146]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #148]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #150]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #152]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #154]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #156]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #158]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #160]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #162]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #164]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #166]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #168]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #170]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #172]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #174]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #176]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #178]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #180]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #182]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #184]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #186]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #188]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #190]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #192]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #194]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #196]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #198]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #200]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #202]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #204]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #206]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #208]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #210]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #212]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #214]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #216]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #218]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #220]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #222]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #224]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #226]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #228]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #230]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #232]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #234]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #236]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #238]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #240]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #242]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #244]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #246]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #248]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #250]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #252]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #254]
	strh	r3, [r1]
	add	r0,r0,#256
	add	r1,r1,#480

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1,r1,#480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	sub	r1, r1, #151552
	sub	r1, r1, #1568
	sub	r1, r1, r2
	add	r0, r0, #128
	subs	r2, r2, #2
	bxlt	lr
	b	otroW


SDL_gp32blit240x160:
	mov	r4, #2
	mov	r2, #239
otro240x160:
	add	r1, r1, r2
	ldrb	r3, [r0, #0]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #1]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #2]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #3]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #4]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #5]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #6]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #7]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #8]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #9]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #10]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #11]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #12]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #13]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #14]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #15]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #16]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #17]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #18]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #19]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #20]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #21]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #22]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #23]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #24]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #25]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #26]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #27]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #28]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #29]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #30]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #31]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #32]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #33]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #34]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #35]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #36]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #37]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #38]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #39]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #40]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #41]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #42]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #43]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #44]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #45]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #46]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #47]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #48]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #49]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #50]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #51]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #52]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #53]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #54]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #55]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #56]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #57]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #58]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #59]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #60]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #61]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #62]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #63]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #64]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #65]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #66]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #67]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #68]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #69]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #70]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #71]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #72]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #73]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #74]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #75]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #76]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #77]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #78]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #79]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #80]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #81]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #82]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #83]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #84]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #85]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #86]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #87]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #88]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #89]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #90]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #91]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #92]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #93]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #94]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #95]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #96]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #97]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #98]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #99]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #100]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #101]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #102]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #103]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #104]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #105]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #106]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #107]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #108]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #109]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #110]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #111]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #112]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #113]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #114]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #115]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #116]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #117]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #118]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #119]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #120]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #121]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #122]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #123]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #124]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #125]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #126]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #127]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #128]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #129]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #130]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #131]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #132]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #133]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #134]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #135]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #136]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #137]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #138]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #139]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #140]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #141]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #142]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #143]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #144]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #145]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #146]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #147]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #148]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #149]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #150]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #151]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #152]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #153]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #154]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #155]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #156]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #157]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #158]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #159]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #160]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #161]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #162]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #163]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #164]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #165]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #166]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #167]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #168]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #169]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #170]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #171]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #172]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #173]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #174]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #175]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #176]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #177]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #178]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #179]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #180]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #181]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #182]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #183]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #184]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #185]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #186]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #187]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #188]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #189]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #190]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #191]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #192]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #193]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #194]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #195]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #196]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #197]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #198]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #199]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #200]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #201]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #202]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #203]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #204]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #205]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #206]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #207]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #208]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #209]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #210]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #211]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #212]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #213]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #214]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #215]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #216]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #217]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #218]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #219]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #220]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #221]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #222]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #223]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #224]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #225]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #226]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #227]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #228]
	strb	r3, [r1, #0]
	ldrb	r3, [r0, #229]
	strb	r3, [r1, #240]
	ldrb	r3, [r0, #230]
	strb	r3, [r1, #480]
	strb	r3, [r1, #720]
	ldrb	r3, [r0, #231]
	strb	r3, [r1, #960]
	ldrb	r3, [r0, #232]
	strb	r3, [r1, #1200]
	ldrb	r3, [r0, #233]
	strb	r3, [r1, #1440]
	strb	r3, [r1, #1680]
	ldrb	r3, [r0, #234]
	strb	r3, [r1, #1920]
	ldrb	r3, [r0, #235]
	strb	r3, [r1, #2160]
	ldrb	r3, [r0, #236]
	strb	r3, [r1, #2400]
	strb	r3, [r1, #2640]
	ldrb	r3, [r0, #237]
	strb	r3, [r1, #2880]
	ldrb	r3, [r0, #238]
	strb	r3, [r1, #3120]
	ldrb	r3, [r0, #239]
	strb	r3, [r1, #3360]
	strb	r3, [r1, #3600]

	sub	r1, r1, #65536
	sub	r1, r1, #7424
	sub	r1, r1, r2
	add	r0, r0, #240
	subs	r4, r4, #1
	bge	otro240x160_2
	mov	r4, #2
	sub	r0, r0, #240
otro240x160_2:
	subs	r2, r2, #1
	bxlt	lr
	b	otro240x160




SDL_gp32blitW240x160:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.


	mov     r2, #480
	sub	r2, r2, #2
	mov	r4, #2
otroW240x160:
	add	r1, r1, r2

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #128]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #130]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #132]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #134]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #136]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #138]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #140]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #142]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #144]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #146]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #148]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #150]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #152]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #154]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #156]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #158]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #160]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #162]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #164]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #166]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #168]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #170]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #172]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #174]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #176]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #178]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #180]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #182]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #184]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #186]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #188]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #190]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #192]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #194]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #196]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #198]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #200]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #202]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #204]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #206]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #208]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #210]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #212]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #214]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #216]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #218]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #220]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #222]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #224]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #226]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #228]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #230]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #232]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #234]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #236]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #238]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #240]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #242]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #244]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #246]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #248]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #250]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480
	add	r0, r0, #252

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #128]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #130]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #132]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #134]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #136]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #138]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #140]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #142]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #144]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #146]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #148]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #150]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #152]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #154]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #156]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #158]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #160]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #162]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #164]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #166]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #168]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #170]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #172]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #174]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #176]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #178]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #180]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #182]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #184]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #186]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #188]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #190]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #192]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #194]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #196]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #198]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #200]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #202]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #204]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #206]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #208]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #210]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #212]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #214]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #216]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #218]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #220]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #222]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #224]
	strh	r3, [r1]
	add	r1, r1, #480
	ldrh	r3, [r0, #226]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]

	sub	r1, r1, #151552
	sub	r1, r1, #1568
	sub	r1, r1, r2
	add	r0, r0, #228
	subs	r4, r4, #1
	bge	otroW240x160_2
	mov	r4, #2
	sub	r0, r0, #480
otroW240x160_2:
	subs	r2, r2, #2
	bxlt	lr
	b	otroW240x160




SDL_gp32blit160x144:
	mov	r5, #1
	mov	r4, #2
	mov	r2, #239
otro160x144:
	add	r1, r1, r2

	ldrb	r3, [r0, #0]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #1]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #2]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #3]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #4]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #5]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #6]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #7]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #8]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #9]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #10]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #11]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #12]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #13]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #14]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #15]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #16]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #17]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #18]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #19]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #20]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #21]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #22]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #23]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #24]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #25]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #26]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #27]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #28]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #29]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #30]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #31]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #32]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #33]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #34]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #35]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #36]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #37]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #38]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #39]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #40]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #41]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #42]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #43]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #44]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #45]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #46]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #47]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #48]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #49]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #50]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #51]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #52]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #53]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #54]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #55]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #56]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #57]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #58]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #59]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #60]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #61]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #62]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #63]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #64]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #65]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #66]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #67]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #68]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #69]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #70]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #71]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #72]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #73]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #74]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #75]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #76]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #77]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #78]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #79]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #80]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #81]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #82]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #83]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #84]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #85]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #86]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #87]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #88]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #89]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #90]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #91]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #92]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #93]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #94]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #95]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #96]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #97]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #98]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #99]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #100]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #101]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #102]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #103]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #104]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #105]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #106]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #107]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #108]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #109]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #110]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #111]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #112]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #113]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #114]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #115]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #116]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #117]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #118]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #119]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #120]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #121]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #122]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #123]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #124]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #125]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #126]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #127]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #128]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #129]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #130]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #131]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #132]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #133]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #134]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #135]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #136]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #137]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #138]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #139]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #140]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #141]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #142]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #143]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #144]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #145]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #146]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #147]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #148]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #149]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #150]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #151]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]
	add	r1, r1, #3840

	ldrb	r3, [r0, #152]
	strb	r3, [r1,#0]
	strb	r3, [r1, #240]

	ldrb	r3, [r0, #153]
	strb	r3, [r1,#480]
	strb	r3, [r1, #720]

	ldrb	r3, [r0, #154]
	strb	r3, [r1,#960]
	strb	r3, [r1, #1200]

	ldrb	r3, [r0, #155]
	strb	r3, [r1,#1440]
	strb	r3, [r1, #1680]

	ldrb	r3, [r0, #156]
	strb	r3, [r1,#1920]
	strb	r3, [r1, #2160]

	ldrb	r3, [r0, #157]
	strb	r3, [r1,#2400]
	strb	r3, [r1, #2640]

	ldrb	r3, [r0, #158]
	strb	r3, [r1,#2880]
	strb	r3, [r1, #3120]

	ldrb	r3, [r0, #159]
	strb	r3, [r1,#3360]
	strb	r3, [r1, #3600]

	sub	r1, r1, #65536
	sub	r1, r1, #7424
	sub	r1, r1, r2
	add	r0, r0, #160
	subs	r4, r4, #1
	bge	otro160x144_2
	sub	r0, r0, #160
	mov	r4, #2
	sub	r4, r4, r5
	subs	r5, r5, #1
	bge	otro160x144_2
	mov	r5, #1
otro160x144_2:
	subs	r2, r2, #1
	bxlt	lr
	b	otro160x144



SDL_gp32blitW160x144:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.


	mov	r5, #1
	mov	r4, #2
	mov     r2, #480
	sub	r2, r2, #2
otroW160x144:
	add	r1, r1, r2

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #64]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #66]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #68]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #70]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #72]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #74]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #76]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #78]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #80]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #82]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #84]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #86]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #88]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #90]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #92]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #94]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #96]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #98]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #100]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #102]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #104]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #106]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #108]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #110]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #112]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #114]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #116]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #118]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #120]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #122]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #124]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #126]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #128]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #130]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #132]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #134]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #136]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #138]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #140]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #142]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #144]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #146]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #148]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #150]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #152]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #154]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #156]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #158]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #160]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #162]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #164]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #166]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #168]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #170]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #172]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #174]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #176]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #178]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #180]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #182]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #184]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #186]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #188]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #190]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #192]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #194]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #196]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #198]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #200]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #202]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #204]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #206]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #208]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #210]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #212]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #214]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #216]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #218]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #220]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #222]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #224]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #226]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #228]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #230]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #232]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #234]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #236]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #238]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #240]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #242]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #244]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #246]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #248]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #250]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #252]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #254]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480
	add	r0, r0, #256

	ldrh	r3, [r0, #0]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #2]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #4]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #6]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #8]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #10]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #12]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #14]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #16]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #18]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #20]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #22]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #24]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #26]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #28]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #30]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #32]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #34]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #36]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #38]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #40]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #42]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #44]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #46]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #48]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #50]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #52]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #54]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #56]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #58]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #60]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]
	add	r1, r1, #480

	ldrh	r3, [r0, #62]
	strh	r3, [r1]
	add	r1, r1, #480
	strh	r3, [r1]

	sub	r1, r1, #151552
	sub	r1, r1, #1568
	sub	r1, r1, r2
	add	r0, r0, #64
	subs	r4, r4, #1
	bge	otroW160x144_2
	sub	r0, r0, #320
	mov	r4, #2
	sub	r4, r4, r5
	subs	r5, r5, #1
	bge	otroW160x144_2
	mov	r5, #1
otroW160x144_2:
	subs	r2, r2, #2
	bxlt	lr
	b	otroW160x144



