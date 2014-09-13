SRC = disasm_x86.cpp instr_x86.cpp detour.cpp memcatch.cpp page.cpp decomp.cpp signature.cpp main.cpp disasm.cpp func.cpp func_x86.cpp
EXTRA_FLAGS = 
CXX = g++
CXXFLAGS = -O0 $(EXTRA_FLAGS) -std=c++0x -fPIC
LDFLAGS =  -fPIC -pthread
BINARY = patch

ARCH = $(shell uname -m)
EXTENSION =
BITMODE = _64
PLATFORM = $(shell uname)
OBJS = $(SRC:%.cpp=%.o)
ifeq ($(PLATFORM), Darwin)
	EXTENSION = .dylib
else
	EXTENSION = .so
endif
OUT = $(BINARY)$(BITMODE)$(EXTENSION)

ifeq ($(ARCH), x86_64)
	ifeq ($(BITMODE), _64)
		ARCH = arch_x64
	else
		ARCH = arch_x86
	endif
endif

ifeq ($(ARCH), x86)
	ARCH = arch_x86
endif

ifeq ($(ARCH), i386)
	ARCH = arch_x86
endif

ifeq ($(ARCH), i686)
	ARCH = arch_x86
endif


CXXFLAGS += -DARCH=$(ARCH)

default: all

all: .dep
	make clean
	make 64
	make clean
	make 32

64: $(BINARY)_64 test_64

32: $(BINARY)_32 test_32

$(OUT): .dep $(OBJS)
	$(CXX) -shared $(LDFLAGS) -o $@ $(OBJS)

$(BINARY)_64:
	make $(BINARY)_64$(EXTENSION) EXTRA_FLAGS="-DBIT_64" BITMODE="_64"

$(BINARY)_32:
	make $(BINARY)_32$(EXTENSION) EXTRA_FLAGS="-m32" BITMODE="_32"

test: .dep $(OBJS) test.o
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) test.o

test_64:
	make test EXTRA_FLAGS="-DBIT_64" BITMODE="_64"

test_32:
	make test EXTRA_FLAGS="-m32" BITMODE="_32"

debug-test_64:
	make test EXTRA_FLAGS="-DDEBUG -DBIT_64 -g" BITMODE="_64"

debug-test_32:
	make test EXTRA_FLAGS="-m32 -DDEBUG -g" BITMODE="_32"

.dep:
	@$(CXX) $(CXXFLAGS) -MM $(SRC) > .dep

clean:
	@rm -Rf *.o $(OUT) .dep

veryclean:
	@rm -Rf *.o $(BINARY)_64$(EXTENSION) $(BINARY)_32$(EXTENSION) test_64 test_32

-include .dep
.PHONY: clean .dep 64 32
