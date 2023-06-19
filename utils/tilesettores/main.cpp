#include <cstdio>
#include <cstdint>
#include <png++/png.hpp>

int main (int argc, char **argv) {
	if (argc < 4) {
		printf ("[rows in px] [cols in px] [input filename] [output filename]\n");
		return -1;
	}

	int rx = atoi (argv[1]);
	int ry = atoi (argv[2]);
	char *input = argv[3];
	char *out = argv[4];

	FILE *fp = fopen (out, "w");
	int y = 1;
	fwrite (&y, sizeof (int), 1, fp);

	char path[255];

	png::image < png::rgba_pixel > img (input);
	uint32_t width = img.get_width ();
	uint32_t height = img.get_height ();

	uint32_t rows = width / rx;
	uint32_t cols = height / ry;

	uint32_t total = rows * cols;

	fwrite (&total, sizeof (int), 1, fp);

	fwrite (&rx, sizeof (int), 1, fp);
	fwrite (&ry, sizeof (int), 1, fp);

	for (int yy = 0; yy < cols; yy++) {
		for (int xx = 0; xx < rows; xx++) {
			uint32_t y = yy * ry;
			uint32_t x = xx * rx;

			for (int my = y; my < (y + ry); my++) {
				for (int mx = x; mx < (x + rx); mx++) {
					uint8_t stream[4];
					stream[0] = img[my][mx].red;
					stream[1] = img[my][mx].green;
					stream[2] = img[my][mx].blue;
					stream[3] = img[my][mx].alpha;
					fwrite (stream, 4, 1, fp);
				}
			}
		}
	}

	fclose (fp);
}
