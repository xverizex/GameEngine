#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "packer.h"

static const char header[] = ".RES";

typedef struct _data_storage {
	uint32_t  type;
	uint32_t count;
	uint64_t  *pos;
	uint64_t *size;
	uint8_t   **data;
} data_storage;

static FILE *inc_file;
static FILE *res_file;

static void
write_inc_header ()
{
	fprintf (inc_file,
			"enum {\n"
			"\tRES_SHADERS,\n"
			"\tRES_FONTS,\n"
			"\tRES_OBJECTS,\n"
			"\tRES_SPRITES,\n"
			"\tRES_SOUNDS,\n"
			"\tN_RES\n"
			"};\n\n");
}

static uint64_t start_data_pos;

static void
write_res_header ()
{
	uint16_t check_byte = 1;
	fwrite (header, 1, sizeof (header) - 1, res_file);
	fwrite (&check_byte, 1, 2, res_file);

	start_data_pos = ftell (res_file);

	uint64_t pos = 0L;
	for (uint32_t i = 0; i < N_RES; i++) {
		fwrite (&pos, 1, 8, res_file);
	}
}

static void
convert_name_to_enum (char *n)
{
	uint32_t len = strlen (n);
	for (uint32_t i = 0; i < len; i++) {
		if (n[i] >= 'a' && n[i] <= 'z')
			n[i] -= 32;
		else if (n[i] == '.')
			n[i] = '_';
	}
}

static uint8_t *
get_file_data (const char *dir, const char *filename, uint64_t *filesize)
{
	char path[512];
	snprintf (path, 512, "%s/%s", dir, filename);

	printf ("path: %s\n", path);
	FILE *fp = fopen (path, "r+");
	if (!fp)
		return NULL;

	fseek (fp, 0, SEEK_END);
	*filesize = ftell (fp);
	fseek (fp, 0, SEEK_SET);

	uint64_t readed;

	uint8_t *data = malloc (*filesize);
	readed = fread (data, 1, *filesize, fp);
	fclose (fp);

	printf ("readed: %lu\n", readed);

	return data;
}

static int
is_not_res (const char *name)
{
	if (!strncmp (name, ".", 2) || !strncmp (name, "..", 3))
		return 1;

	return 0;
}

data_storage *
get_build_data (uint32_t type, const char *dir_name, const char *end)
{
	data_storage *dt = malloc (sizeof (data_storage));
	dt->type = type;

	fprintf (inc_file, "enum {\n");

	uint8_t *s = NULL;
	dt->data = NULL;
	dt->pos = NULL;
	dt->count = 0;
	dt->size = NULL;
	uint64_t pos = 0;

	DIR *dir = opendir (dir_name);
	struct dirent *d;

	while (d = readdir (dir)) {
		const char *name = d->d_name;
		if (is_not_res (name))
			continue;

		char *n = strdup (name);
		convert_name_to_enum (n);
		fprintf (inc_file, "\t%s,\n", n);
		int index = dt->count++;
		dt->pos = realloc (dt->pos, sizeof (uint64_t) * dt->count);
		dt->data = realloc (dt->data, sizeof (void *) * dt->count);
		dt->size = realloc (dt->size, sizeof (uint64_t) * dt->count);
		uint8_t *data = get_file_data (dir_name, name, &dt->size[index]);
		dt->pos[index] = pos;
		dt->data[index] = data;
		pos += dt->size[index];
	}

	closedir (dir);

	fprintf (inc_file, "\t%s\n};\n\n", end);

	return dt;
}

static uint64_t
write_resource (data_storage *dt)
{
	uint64_t pos_type_res = ftell (res_file);

	uint64_t pos_cur = 0L;
	uint32_t count = dt->count;

	/* write type res count */
	for (uint32_t i = 0; i < count; i++) {
		fwrite (&pos_cur, 1, sizeof (uint64_t), res_file);
	}

	/* write data res */
	for (uint32_t i = 0; i < count; i++) {
		uint64_t pos_pre = ftell (res_file);

		fwrite (&dt->size[i], 1, sizeof (uint64_t), res_file);
		fwrite (dt->data[i], 1, dt->size[i], res_file);

		uint64_t pos_post = ftell (res_file);

		uint64_t pos_cur_data = pos_type_res + (i * sizeof (uint64_t));

		fseek (res_file, pos_cur_data, SEEK_SET);
		fwrite (&pos_pre, 1, sizeof (uint64_t), res_file);
		fseek (res_file, pos_post, SEEK_SET);
	}

	return pos_type_res;
}

static void
write_header (uint32_t type, uint64_t pos)
{
	uint64_t temp = ftell (res_file);

	uint64_t pos_in_header = start_data_pos + type * sizeof (uint64_t);

	fseek (res_file, pos_in_header, SEEK_SET);
	fwrite (&pos, 1, sizeof (uint64_t), res_file);

	fseek (res_file, temp, SEEK_SET);
}

#if 0
void example ()
{
	struct data *d = unpacker_get (RES_SHADERS, SHADER_LINES_VERT);
}
#endif

int main (int argc, char **argv)
{
	inc_file = fopen ("res.h", "w");
	write_inc_header ();

	data_storage *shaders = get_build_data (RES_SHADERS, "shaders", "N_SHADERS");
	data_storage *fonts = get_build_data (RES_FONTS, "fonts", "N_FONTS");
	data_storage *objects = get_build_data (RES_OBJECTS, "objects", "N_OBJECTS");
	data_storage *sprites = get_build_data (RES_SPRITES, "sprites", "N_SPRITES");
	data_storage *sounds = get_build_data (RES_SOUNDS, "sounds", "N_SOUNDS");

	fclose (inc_file);

	const char *project_resource_file = "Data.res";
	res_file = fopen (project_resource_file, "w");
	if (!res_file) {
		fprintf (stderr, "[err]: inpossible to create data file.\n");
		return -1;
	}

	write_res_header ();

	uint64_t pos_shaders = write_resource (shaders);	
	uint64_t pos_fonts = write_resource (fonts);	
	uint64_t pos_objects = write_resource (objects);	
	uint64_t pos_sprites = write_resource (sprites);	
	uint64_t pos_sounds = write_resource (sounds);	

	write_header (RES_SHADERS, pos_shaders);
	write_header (RES_FONTS, pos_fonts);
	write_header (RES_OBJECTS, pos_objects);
	write_header (RES_SPRITES, pos_sprites);
	write_header (RES_SOUNDS, pos_sounds);

	fclose (res_file);
}
