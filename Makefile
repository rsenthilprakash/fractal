CC = gcc
CFLAGS = -Wall
LIBS = -lm
CPPLIBS = -lstdc++
PNG_CFLAGS = `pkg-config --cflags libpng`
PNG_LIBS = `pkg-config --libs libpng`

C_SOURCES = fern.c \
            png_utils.c \
            poly.c \
            tree.c \
            maple.c \
            ifs.c \
            julia.c \
            mandel.c \

CPP_SOURCES =  julia_iim.cpp \

DEPENDS = $(C_SOURCES:%.c=%.d) $(CPP_SOURCES:%.cpp=%.d)

C_TARGETS = fern poly tree maple ifs julia mandel
CPP_TARGETS = julia_iim
TARGETS = $(C_TARGETS) $(CPP_TARGETS)

.PHONY: all
all: $(TARGETS)

$(C_TARGETS): % : %.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

$(CPP_TARGETS): % : %.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(CPPLIBS) -o $@

png_utils.o: png_utils.c
	$(CC) -c $(CFLAGS) $(PNG_CFLAGS) $< -o $@

.c.o:
	$(CC) -c $(CFLAGS) -MMD $< -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) -MMD $< -o $@

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -f $(TARGETS)
	rm -f *.o
	rm -f $(DEPENDS)
