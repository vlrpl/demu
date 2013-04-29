#CFLAGS  += `llvm-config-3.0 --cflags`
#LDFLAGS += `llvm-config-3.0 --libs --cflags --ldflags core analysis executionengine jit interpreter native`

CFLAGS   := -I./include -I. -g -O2
LDFLAGS  += -ludis86 
#LDFLAGS += $(shell llvm-config-3.0 --libs)
#LDFLAGS += $(shell llvm-config-3.0 --ldflags)
#LDFLAGS += $(shell llvm-config-3.0 --cflags)

DIRS    := . ./arch/x86 
TESTDIR := test

SRC     := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS    := $(addsuffix .o, $(basename $(SRC))) 

PNAME   := demu

all: $(PNAME) test

$(PNAME): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

test:
	$(MAKE) -C $(TESTDIR)

clean:
	rm -f $(OBJS) $(PNAME) 
	$(MAKE) -C $(TESTDIR) clean

.PHONY: clean $(TESTDIR)
