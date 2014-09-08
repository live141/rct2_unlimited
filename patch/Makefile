SRC = disasm_x86.cpp instr_x86.cpp detour.cpp memcatch.cpp page.cpp decomp.cpp signature.cpp main.cpp
EXTRA_FLAGS = 
CXX = g++
CXXFLAGS = -O0 $(EXTRA_FLAGS)
LDFLAGS =  -fPIC
BINARY = patch

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

default: all

all: .dep 64

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
	make test EXTRA_FLAGS="-DDEBUG -DBIT_64" BITMODE="_64"

debug-test_32:
	make test EXTRA_FLAGS="-m32 -DDEBUG" BITMODE="_32"

.dep:
	@$(CXX) $(CXXFLAGS) -MM $(SRC) > .dep

clean:
	@rm -Rf *.o $(OUT) .dep

veryclean:
	@rm -Rf *.o $(BINARY)_64$(EXTENSION) $(BINARY)_32$(EXTENSION) test_64 test_32

-include .dep
.PHONY: clean .dep 64 32
