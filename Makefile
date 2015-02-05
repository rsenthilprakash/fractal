CC = gcc
CFLAGS = -Wall
LIBS = -lm
PNG_CFLAGS = `pkg-config --cflags libpng`
PNG_LIBS = `pkg-config --libs libpng`

SOURCES = fern.c \
          png_utils.c \
          poly.c \
          tree.c \
          maple.c \
          ifs.c

TARGETS =  fern poly tree maple ifs

.PHONY: all
all: $(TARGETS)

fern: fern.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

poly: poly.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

tree: tree.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

maple: maple.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

ifs: ifs.o png_utils.o
	$(CC) $^ $(PNG_LIBS) $(LIBS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $(PNG_CFLAGS) $< -o $@
depend: .depend

.depend: $(SOURCES)
	rm -f ./.depend
	$(CC) $(CFLAGS) $(PNG_CFLAGS) -MM $^ > ./.depend;

include .depend

.PHONY: clean
clean:
	rm -rf $(TARGETS)
	rm -rf *.o
	rm -rf ./.depend
