crop: crop.c
	cc `MagickWand-config --cflags --cppflags` crop.c `MagickWand-config --ldflags --libs` -o crop

clean:
	rm -f crop

distclean: clean
