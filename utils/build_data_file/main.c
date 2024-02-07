#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>

#define NAME_DATA_FILE            "Game.data"
#define RES_ENUM                  "ResEnum.h"

/*
 * SHADERS = 0
 * FONTS = 1
 * TEXTURES = 2
 * SOUND_FILE = 3
 */

#define POS_SHADERS                (0 * sizeof (uint64_t))
#define POS_FONTS                  (1 * sizeof (uint64_t))
#define POS_TEXTURES               (2 * sizeof (uint64_t))
#define POS_SOUND                  (3 * sizeof (uint64_t))
#define POS_OBJ                    (4 * sizeof (uint64_t))

enum {
	R_SHADERS,
	R_FONTS,
	R_TEXTURES,
	R_SOUND_FILE,
	N_R
};

struct file_info {
	char name[255];
	char define_name[255];
};

struct element {
	struct file_info *db[N_R];
	uint32_t count_file_info[N_R];
	uint32_t count_block[N_R];
	uint32_t cur_size_block[N_R];
};

uint32_t block = 512;

struct element el;

void restructure_blocks (uint32_t index)
{
	el.count_block[index]++;
	el.cur_size_block[index] = block * el.count_block[index];
	el.db[index] = realloc (el.db[index], sizeof (struct file_info) * el.cur_size_block[index]);
}

static void make_defined_name (char *d, char *n)
{
	char *res = "RES_";
	int len_res = strlen (res);

	strncpy (d, res, len_res);
	d += len_res;

	int len = strlen (n);
	for (int i = 0; i < len; i++) {
		if (n[i] == '.')
			d[i] = '_';
		else if (n[i] >= 'a' && n[i] <= 'z')
			d[i] = n[i] - 32;
		else
			d[i] = n[i];
	}
}

void make_enum ()
{
	static const char *list[N_R] = {
		"\tN_SHADER_RES\n",
		"\tN_FONTS_RES\n",
		"\tN_TEXTURES_RES\n",
		"\tN_SOUNDS_RES\n",
		"\tN_OBJ_RES\n"
	};

	FILE *fp = fopen (RES_ENUM, "w");
	fprintf (fp, 
			"#ifndef RES_ENUM_H\n"
			"#define RES_ENUM_H\n"
			"\n"
		);

	fprintf (fp, 
			"enum {\n"
			"\tR_SHADERS,\n"
			"\tR_FONTS,\n"
			"\tR_TEXTURES,\n"
			"\tR_SOUNDS,\n"
			"\tR_OBJ,\n"
			"\tN_R\n"
			"};\n\n"
			);


	for (uint32_t index = 0; index < N_R; index++) {
		fprintf (fp, "enum {\n");

		for (int i = 0; i < el.count_file_info[index]; i++) {
			fprintf (fp, "\t%s,\n", el.db[index][i].define_name);
		}

		fprintf (fp, list[index]);
		fprintf (fp, "};\n\n");
	}

	fprintf (fp, "#endif\n");

	fclose (fp);
}

int main (int argc, char **argv)
{
	for (int i = 0; i < N_R; i++) {
		el.db[i] = malloc (block * el.count_block[i] * sizeof (struct file_info));
		el.count_block[i] = 1;
		el.cur_size_block[i] = el.count_block[i] * block;
	}

	DIR *dir = opendir (".");
	uint32_t count = 0;

	FILE *data_list = fopen ("data.list", "r");
	if (!data_list) {
		fprintf (stderr, "should been a file data.list\n");
		return -1;
	}

	char line[512];
	while (fgets (line, 512, data_list)) {
		char name[256];
		char defined_name[256] = {0, };
		uint32_t type;
		sscanf (line, "%s %d", name, &type);

		make_defined_name (defined_name, name);

		struct file_info *fs = el.db[type];
		strncpy (fs[el.count_file_info[type]].name, name, strlen (name));
		strncpy (fs[el.count_file_info[type]].define_name, defined_name, strlen (defined_name));
		el.count_file_info[type]++;
		if (el.count_file_info[type] >= el.cur_size_block[type])
			restructure_blocks (type);
	}	

	make_enum ();

	FILE *log = fopen ("pack.log", "w");
	FILE *dt = fopen (NAME_DATA_FILE, "w");

	uint64_t little_big_engian = 1L;
	fwrite (&little_big_engian, sizeof (uint64_t), 1, dt);

	uint64_t group = 0L;
	for (int i = 0; i < 4; i++) {
		fwrite (&group, sizeof (uint64_t), 1, dt);
	}

	uint64_t group_pos[N_R] = {0L,};
	uint64_t off = 0L;

	for (int i = 0; i < N_R; i++) {
		group_pos[i] = ftell (dt);

		fseek (dt, sizeof (uint64_t) + (i * sizeof (uint64_t)), SEEK_SET);
		fwrite (&group_pos[i], sizeof (uint64_t), 1, dt);
		fseek (dt, group_pos[i], SEEK_SET);

		for (int m = 0; m < el.count_file_info[i]; m++) {
			fwrite (&off, sizeof (uint64_t), 1, dt);
		}
	}

	for (uint32_t index = 0; index < N_R; index++) {
		uint64_t i = 0;
		for (; i < el.count_file_info[index]; i++) {
			FILE *file = fopen (el.db[index][i].name, "r");
			uint64_t size = 0L;
			fseek (file, 0, SEEK_END);
			size = ftell (file);
			fseek (file, 0, SEEK_SET);

			uint8_t *data = malloc (size);
			fread (data, size, 1, file);
			fclose (file);

			off = ftell (dt);

			fwrite (&size, sizeof (uint64_t), 1, dt);
			fwrite (data, size, 1, dt);

			uint64_t pos_continue = ftell (dt);

			fseek (dt, group_pos[index] + sizeof (uint64_t) * i, SEEK_SET);
			fwrite (&off, sizeof (uint64_t), 1, dt);

			fseek (dt, pos_continue, SEEK_SET);

			fprintf (log, "%s = %lx\n", el.db[index][i].name, off);

			free (data);
		}
	}

	fclose (log);
	fclose (dt);
}
