SDL_MAJOR_VERSION += 2

BINARY ?= Wolf4SDL
PREFIX ?= /usr/local

INSTALL ?= install
INSTALL_PROGRAM ?= $(INSTALL) -m 555 -s
INSTALL_MAN ?= $(INSTALL) -m 444
INSTALL_DATA ?= $(INSTALL) -m 444

ifeq ($(SDL_MAJOR_VERSION),1)
SDL_CONFIG ?= sdl-config
endif
ifeq ($(SDL_MAJOR_VERSION),2)
SDL_CONFIG ?= sdl2-config
endif

CFLAGS_SDL ?= $(shell $(SDL_CONFIG) --cflags)
LDFLAGS_SDL ?= $(shell $(SDL_CONFIG) --libs)
CFLAGS += -O2 -Wall -W -g -Wpointer-arith -Wreturn-type -Wwrite-strings -Wcast-align -std=gnu99 \
-Wimplicit-int -Wsequence-point $(CFLAGS_SDL)

LDFLAGS += $(LDFLAGS_SDL) -lm
ifeq ($(SDL_MAJOR_VERSION),1)
LDFLAGS += -lSDL_mixer
endif

ifeq ($(SDL_MAJOR_VERSION),2)
LDFLAGS += -lSDL2_mixer
endif

ifneq (,$(findstring MINGW,$(shell uname -s)))
LDFLAGS += -static-libgcc
endif
LW_LIB_SRCS += 3rdparty/lwlib/lw_ai_enemy.c 3rdparty/lwlib/lw_bres.c 3rdparty/lwlib/lw_ctx.c \
3rdparty/lwlib/lw_edit.c 3rdparty/lwlib/lw_fs.c 3rdparty/lwlib/lw_maptool.c 3rdparty/lwlib/lw_misc.c \
3rdparty/lwlib/lw_protmsg.c 3rdparty/lwlib/lw_pwscan.c 3rdparty/lwlib/lw_vec.c

WOLFRAD_SRCS += 3rdparty/wolfrad/wolfrad.c 3rdparty/wolfrad/wr_level.c 3rdparty/wolfrad/wr_lightinfo.c \
3rdparty/wolfrad/lightmap.c 3rdparty/wolfrad/wr_rad.c 3rdparty/wolfrad/wr_radmap.c 3rdparty/wolfrad/wr_raycaster.c \
3rdparty/wolfrad/wr_room.c 3rdparty/wolfrad/wr_screne.c

ID_ENGINE_SRCS += id_ca.c id_crt.o id_in.c id_pm.c id_sd.c id_us.c id_vh.c id_vieasm.c id_vl.c

SRCS += aud_sys/mame/fmopl.c aud_sys/dosbox/dbopl.c $(LW_LIB_SRCS) $(WOLFRAD_SRCS) $(ID_ENGINE_SRCS) signon.c \
wl_ai.c wl_conartist.c wl_benchobo.c wl_clockking.c wl_ed.c wl_firefly.c wl_hooker.c wl_led.c wl_healthmetter.c \
wl_math.c wl_physics.c wl_pimp.c wl_polygon.c wl_sleephobo.c wl_anyactor.c \
wl_act1.c wl_act2.c wl_agent.c wl_atmos.c wl_cloudsky.c wl_debug.c wl_draw.c wl_game.c wl_inter.c \
wl_main.c wl_menu.c wl_parallax.c wl_plane.c wl_play.c wl_scale.c wl_shade.c wl_state.c \
wl_text.c wl_utils.c 

DEPS = $(filter %.d, $(SRCS:.c=.d) )
OBJS = $(filter %.o, $(SRCS:.c=.o) )

.SUFFIXES: .c .d .o

Q ?= @

all: $(BINARY)

ifndef NO_DEPS
depend: $(DEPS)

ifeq ($(findstring $(MAKECMDGOALS), clean depend Data),)
-include $(DEPS)
endif
endif

$(BINARY): $(OBJS)
	@echo '===> LD $@'
	$(Q)$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

.c.o:
	@echo '===> C $<'
	$(Q) $(CC) $(CFLAGS) -c $< -o $@

.c.d:
	@echo '===> DEP $<'
	$(Q)$(CC) $(CFLAGS) -MM $< | sed 's#^$(@F:%.d=%.o):#$@ $(@:%.d=%.o):#' > $@

clean distclean:
	@echo '===> CLEANING...'
	$(Q)rm -fr $(DEPS) $(OBJS) $(BINARY) $(BINARY).exe

install: $(BINARY)
	@echo '===> INSTALLING...'
	$(Q)$(INSTALL) -d $(PREFIX)/bin
	$(Q)$(INSTALL_PROGRAM) $(BINARY) $(PREFIX)/bin
