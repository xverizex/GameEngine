#include <cstdio>
#include <png++/png.hpp>
#include <iostream>

int main (int argc, char **argv) {
	if (argc < 3) return -1;

	int total = 6;
	char *name = argv[1];
	char out[255];
	snprintf (out, 255, "%s.texture", argv[2]);

	FILE *fp = fopen (out, "wt");
	int y = 1;
	fwrite (&y, sizeof (int), 1, fp);
	fwrite (&total, sizeof (int), 1, fp);

	char path[255];
	uint32_t width;
	uint32_t height;

	const char *names[] = {
		"_Base_color",
		"_Height",
		"_Metallic",
		"_Mixed_AO",
		"_Normal_OpenGL",
		"_Roughness"
	};

	for (int i = 0; i < total; i++) {
		snprintf (path, 255, "%s%s.png", name, names[i]);
		printf ("names %d: %s; path: %s\n", i, names[i], path);
		png::image < png::rgba_pixel > img (path);
		if (i == 0) {
			width = img.get_width();
			height = img.get_height();
			fwrite (&width, sizeof (int), 1, fp);
			fwrite (&height, sizeof (int), 1, fp);
		}

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
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
