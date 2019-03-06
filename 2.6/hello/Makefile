#
EXTRA_CFLAGS := -DDEBUG=1

MODS = hello.ko hello_m.ko params.ko
OBJS = hello.o  hello_m.o  params.o
TARGETS = ${MODS}

all: ${TARGETS}

hello.ko: hello.c
	make -C /lib/modules/`uname -r`/build M=`pwd` KBUILD_VERBOSE=1 modules

hello_m.ko: hello_multi.c hello_extern.c
	make -C /lib/modules/`uname -r`/build M=`pwd` KBUILD_VERBOSE=1 modules

params.ko: params.c
	make -C /lib/modules/`uname -r`/build M=`pwd` KBUILD_VERBOSE=1 modules

clean:
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

obj-m := ${OBJS}

hello_m-objs := hello_multi.o hello_extern.o

clean-files := *~ *.o *.ko *.mod.[co] ${TARGETS} Module.*
