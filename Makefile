
#
#	Br8k0ut - Platform Video game
#	Copyright © 2017 Luxon Jean-Pierre
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#	Luxon Jean-Pierre (Gumichan01)
#	website: https://gumichan01.github.io/
#	mail: luxon.jean.pierre@gmail.com
#

# Makefile - Br8k0ut

.PHONY: clean clear all main.o

# You can modify the value of DEBUG
# If you want to use debug or release mode
DEBUG=yes

CC=g++
DEBUG_OBJ=TX_Debug.o
MAIN_OBJ=main.o
SRC=$(wildcard src/*.cpp)
OBJS= $(SRC:.cpp=.o)
# Path to main file directory
MAIN_PATH=./src/

# Executable file
BREAKOUT_EXE=br8k0ut

# Path to directory and include directory
SDL2_I_PATH=`pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
LIB_I_PATH=./include/

# Libraries
LUNATIX_STATIC_LIB=./lib/linux/libLunatix.a
LUNATIX_SHARED_LIB=./lib/linux/libLunatix.so
SDL_LFLAGS=`pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf`

# Warning flags
WFLAGS=-Wall -Wextra


# Select flags according to the compilation mode
ifeq ($(DEBUG),yes)

	# Debug mode
	MAIN_SRC=$(MAIN_PATH)main_dbg.cpp
	CFLAGS=$(WFLAGS) -std=c++11 -g
	OPTIMIZE=-O0
	OPT_SIZE=

else

	# Release mode
	MAIN_SRC=$(MAIN_PATH)main.cpp
	CFLAGS=-w -std=c++11
	OPTIMIZE=-O3
	OPT_SIZE=-s

endif


# Linking flags
LFLAGS=$(LUNATIX_SHARED_LIB) $(LUNATIX_STATIC_LIB) $(SDL_LFLAGS)

#
# Build
#

all : $(BREAKOUT_EXE)

$(BREAKOUT_EXE) : $(OBJS)
ifeq ($(DEBUG),yes)
	@echo $@" - Debug mode"
else
	@echo $@" - Release mode"
endif
	@echo $@" - Linking "
	@$(CC) -o $@ $^ $(CFLAGS) $(OPTIMIZE) $(OPT_SIZE) $(LFLAGS) && \
	echo $@" - Build finished with success"


#
# Object files
#

%.o: %.cpp
	@echo $@" - Compiling "$<
	@$(CC) -c -o $@ $< -I $(SDL2_I_PATH) -I $(LIB_I_PATH) $(CFLAGS)

#
# Clean
#

clean :
	@echo "Delete object file "
	@rm -f $(OBJS)

mrproper : clean
	@echo "Delete target"
	@rm -f $(BREAKOUT_EXE)
