
GC = gcc
GFLAGS = -lm -O3

SRCS = mand.c lodepng.c

mand: $(SRCS)
	$(GC) -o $@ $^ $(GFLAGS)

