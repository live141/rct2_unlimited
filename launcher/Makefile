SRC = main.cpp
LOADER_32 = loader_32.asm
LOADER_64 = loader_64.asm

ASM = /opt/local/bin/nasm
ASMFLAGS_64 = 
ASMFLAGS_32 = 
CXX = g++
CXXFLAGS = -O3 -DBIT_64 #-DDEBUG
LDFLAGS =
BINARY = launcher


OBJS = $(SRC:%.cpp=%.o)
PLATFORM = $(shell uname)
EXTENSION = 
OUT = $(BINARY)$(EXTENSION)

ifeq ($(PLATFORM), Darwin)
ASMFLAGS_64 += -f macho64
ASMFLAGS_32 += -f macho32
else
ASMFLAGS_64 += -f elf64
ASMFLAGS_32 += -f elf32
endif

default: all

all: .dep loader_32 loader_64 $(OUT)

loader_64: $(LOADER_64)
	$(ASM) $(ASMFLAGS_64) $< -o $@

loader_32: $(LOADER_32)
	$(ASM) $(ASMFLAGS_32) $< -o $@

$(OUT): .dep $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

.dep:
	@$(CXX) $(CXXFLAGS) -MM $(SRC) > .dep

clean:
	@rm -Rf *.o $(OUT) .dep loader_32 loader_64

-include .dep
.PHONY: clean .dep
