IMPREFIX=/usr/local/ImageMagick
CFLAGS=-I$(IMPREFIX)/include/ImageMagick -fopenmp -O2
LDFLAGS=-L$(IMPREFIX)/lib -Wl,-rpath=$(IMPREFIX)/lib -Wl,--as-needed
LDLIBS=-lMagickWand -lMagickCore

crop: crop.c
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $< $(LDLIBS)

clean:
	rm -f crop

distclean: clean
