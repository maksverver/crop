#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>

int width_out, height_out;
const char *path_in, *path_out;

int dimensions_valid(int width, int height)
{
    return width >= 16 && height >= 16 && width <= 10000 && height <= 10000;
}

void parse_args(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf( stdout,
        /*  0.........1..........2.........3.........4.........5 */
            "Usage: crop <width> <height> <path-in> <path-out>\n"
            "Resizes and crops an image so the resulting image "
            "has dimensions width x height.\n" );
        exit(1);
    }

    width_out  = atoi(argv[1]);
    height_out = atoi(argv[2]);
    path_in    = argv[3];
    path_out   = argv[4];

    if (!dimensions_valid(width_out, height_out))
    {
        fprintf(stderr, "Invalid output dimensions: %d x %d\n", width_out, height_out);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    MagickWand *wand;
    double x, y;
    int width_in, height_in;
    int width_crop, height_crop;
    MagickBooleanType result;

    parse_args(argc, argv);

    MagickWandGenesis();

    wand = NewMagickWand();
    if (MagickReadImage(wand, path_in) != MagickTrue)
    {
        fprintf(stderr, "Could not read input image from path %s!\n", path_in);
        exit(1);
    }

    width_in  = (int)MagickGetImageWidth(wand);
    height_in = (int)MagickGetImageHeight(wand);
    if (!dimensions_valid(width_in, height_in))
    {
        fprintf(stderr, "Invalid input dimensions: %d x %d\n", width_in, height_in);
        exit(1);
    }

    width_crop  = height_in*width_out/height_out;
    height_crop = width_in *height_out/width_out;

    /* Debug-print plan */
    /*
    fprintf(stderr, "INFO: Crop from %d x %d to %d x %d\n", width_in, height_in,
        width_crop < width_in ? width_crop : width_in,
        height_crop < height_in ? height_crop : height_in );
    fprintf(stderr, "INFO: Resize from %d x %d to %d x %d\n",
        width_crop < width_in ? width_crop : width_in,
        height_crop < height_in ? height_crop : height_in,
        width_out, height_out );
    */

    if (width_in > width_crop)
    {
        if (MagickCropImage(wand,
            width_crop, height_in, (width_in - width_crop)/2, 0) != MagickTrue)
        {
            fprintf(stderr, "Horizontal cropping failed!\n", path_in);
            exit(1);
        }
    }
    if (height_in > height_crop)
    {
        if (MagickCropImage(wand,
            width_in, height_crop, 0, (height_in - height_crop)/2) != MagickTrue)
        {
            fprintf(stderr, "Vertical cropping failed!\n", path_in);
            exit(1);
        }
    }

    if (MagickResizeImage(wand, width_out, height_out, LanczosFilter, 1) != MagickTrue)
    {
        fprintf(stderr, "Could not resize image!\n", path_in);
        exit(1);
    }

    MagickSetImageCompressionQuality(wand, 90);

    if (MagickWriteImage(wand, path_out) != MagickTrue)
    {
        fprintf(stderr, "Could not write output image!\n", path_in);
        exit(1);
    }

    return 0;
}
