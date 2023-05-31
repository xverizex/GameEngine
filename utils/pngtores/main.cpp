#include <cstdio>
#include <png++/png.hpp>

int main (int argc, char **argv) {
	if (argc < 4) return -1;

	int total = atoi (argv[1]);
	char *name = argv[2];
	char *out = argv[3];

	FILE *fp = fopen (out, "w");
	int y = 1;
	fwrite (&y, sizeof (int), 1, fp);
	fwrite (&total, sizeof (int), 1, fp);

	total++;

	char path[255];
	uint32_t width;
	uint32_t height;

	for (int i = 1; i < total; i++) {
		snprintf (path, 255, "%s_%d.png", name, i);
		png::image < png::rgba_pixel > img (path);
		if (i == 1) {
			width = img.get_width();
			height = img.get_height();
			fwrite (&width, sizeof (int), 1, fp);
			fwrite (&height, sizeof (int), 1, fp);
		}

		for (int y = 0; y < height; y++) {
			for (int x = width - 1; x >= 0; x--) {
				uint8_t stream[4];
				stream[0] = img[y][x].red;
				stream[1] = img[y][x].green;
				stream[2] = img[y][x].blue;
				stream[3] = img[y][x].alpha;
				fwrite (stream, 4, 1, fp);
			}
		}
	}

	fclose (fp);
}
