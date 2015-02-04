CC = gcc
CFLAGS = -Wall
LIBS = -lm
PNG_CFLAGS = `pkg-config --cflags libpng`
PNG_LIBS = `pkg-config --libs libpng`

TARGETS =  fern poly tree maple ifs

.PHONY: all
all: $(TARGETS)

fern: fern.c png_utils.c
	$(CC) $(CFLAGS) $(PNG_CFLAGS) $^ $(PNG_LIBS) $(LIBS) -o $@

poly: poly.c png_utils.c
	$(CC) $(CFLAGS) $(PNG_CFLAGS) $^ $(PNG_LIBS) $(LIBS) -o $@

tree: tree.c png_utils.c
	$(CC) $(CFLAGS) $(PNG_CFLAGS) $^ $(PNG_LIBS) $(LIBS) -o $@

maple: maple.c png_utils.c
	$(CC) $(CFLAGS) $(PNG_CFLAGS) $^ $(PNG_LIBS) $(LIBS) -o $@

ifs: ifs.c png_utils.c
	$(CC) $(CFLAGS) $(PNG_CFLAGS) $^ $(PNG_LIBS) $(LIBS) -o $@

.PHONY: clean
clean:
	rm -rf $(TARGETS)
