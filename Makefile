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
LDFLAGS_SDL ?= $(shell $(SDL_CONFIG) --libs
CFLAGS += -O2 -Wall -W -g -Wpointer-arith -Wreturn-type -Wwrite-strings -Wcast-align -std=gnu99 \
-Werror-implicit-function-declaration -Wimplicit-int -Wsequence-point $(CFLAGS_SDL)
ifdef GPL
CXXLAGS += -DUSE_GPL -Wall -std=c++2a
endif

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

ifndef GPL
    SRCS += dosbox/dbopl.cpp
endif

SRCS += mame/fmopl.c id_ca.c id_in.c id_pm.c id_sd.c id_us.c id_vh.c id_vl.c signon.c wl_act1.c \
wl_act2.c wl_agent.c wl_atmos.c wl_cloudsky.c wl_debug.c wl_draw.c wl_game.c wl_inter.c \
wl_main.c wl_menu.c wl_parallax.c wl_plane.c wl_play.c wl_scale.c wl_shade.c wl_state.c \
wl_text.c wl_utils.c 

DEPS = $(filter %.d, $(SRCS:.c=.d) $(SRCS:.cpp=.d))
OBJS = $(filter %.o, $(SRCS:.c=.o) $(SRCS:.cpp=.o))

.SUFFIXES: .c .cpp .d .o

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
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	@echo '===> CPP $<'
	$(Q)$(CXX) $(CXXFLAGS) -c $< -o $@

.c.d:
	@echo '===> DEP $<'
	$(Q)$(CC) $(CFLAGS) -MM $< | sed 's#^$(@F:%.d=%.o):#$@ $(@:%.d=%.o):#' > $@

.cpp.d:
	@echo '===> DEP $<'
	$(Q)$(CXX) $(CXXFLAGS) -MM $< | sed 's#^$(@F:%.d=%.o):#$@ $(@:%.d=%.o):#' > $@

clean distclean:
	@echo '===> CLEANING...'
	$(Q)rm -fr $(DEPS) $(OBJS) $(BINARY) $(BINARY).exe

install: $(BINARY)
	@echo '===> INSTALLING...'
	$(Q)$(INSTALL) -d $(PREFIX)/bin
	$(Q)$(INSTALL_PROGRAM) $(BINARY) $(PREFIX)/bin
