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

struct file_info {
	uint32_t num;
	char name[255];
	char define_name[255];
};

uint32_t block = 512;
uint32_t fs_count_file_info;
uint32_t fffs_count_file_info;
uint32_t fts_count_file_info;
uint32_t fss_count_file_info;
uint32_t fs_count_block = 1;
uint32_t fffs_count_block = 1;
uint32_t fts_count_block = 1;
uint32_t fss_count_block = 1;
uint32_t fs_cur_size_block;
uint32_t fffs_cur_size_block;
uint32_t fts_cur_size_block;
uint32_t fss_cur_size_block;

struct file_info *fs;
struct file_info *fss;
struct file_info *fffs;
struct file_info *fts;

void fs_restructure_blocks ()
{
	fs_count_block++;
	fs_cur_size_block = block * fs_count_block;
	fs = realloc (fs, sizeof (struct file_info) * fs_cur_size_block);
}
void fss_restructure_blocks ()
{
	fss_count_block++;
	fss_cur_size_block = block * fss_count_block;
	fss = realloc (fss, sizeof (struct file_info) * fss_cur_size_block);
}
void fffs_restructure_blocks ()
{
	fffs_count_block++;
	fffs_cur_size_block = block * fffs_count_block;
	fffs = realloc (fffs, sizeof (struct file_info) * fffs_cur_size_block);
}
void fts_restructure_blocks ()
{
	fts_count_block++;
	fts_cur_size_block = block * fts_count_block;

	fts = realloc (fts, sizeof (struct file_info) * fts_cur_size_block);
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
			"\tN_R\n"
			"};\n\n"
			);

	fprintf (fp, "enum {\n");

	for (int i = 0; i < fs_count_file_info; i++) {
		fprintf (fp, "\t%s,\n", fs[i].define_name);
	}

	fprintf (fp, "\tN_SHADER_RES\n");

	fprintf (fp, "};\n\n");

	fprintf (fp, "enum {\n");

	for (int i = 0; i < fffs_count_file_info; i++) {
		fprintf (fp, "\t%s,\n", fffs[i].define_name);
	}

	fprintf (fp, "\tN_FONTS_RES\n");

	fprintf (fp, "};\n\n");

	fprintf (fp, "enum {\n");

	for (int i = 0; i < fts_count_file_info; i++) {
		fprintf (fp, "\t%s,\n", fts[i].define_name);
	}

	fprintf (fp, "\tN_TEXTURES_RES\n");

	fprintf (fp, "};\n\n");

	fprintf (fp, "enum {\n");

	for (int i = 0; i < fss_count_file_info; i++) {
		fprintf (fp, "\t%s,\n", fss[i].define_name);
	}

	fprintf (fp, "\tN_SOUNDS_RES\n");

	fprintf (fp, "};\n\n");

	fprintf (fp, "#endif\n");

	fclose (fp);
}

int main (int argc, char **argv)
{
	fs = malloc (block * fs_count_block * sizeof (struct file_info));
	fss = malloc (block * fss_count_block * sizeof (struct file_info));
	fffs = malloc (block * fffs_count_block * sizeof (struct file_info));
	fts = malloc (block * fts_count_block * sizeof (struct file_info));
	fs_cur_size_block = fs_count_block * block;
	fss_cur_size_block = fss_count_block * block;
	fffs_cur_size_block = fffs_count_block * block;
	fts_cur_size_block = fts_count_block * block;

	DIR *dir = opendir (".");
	uint32_t count = 0;

	FILE *data_list = fopen ("data.list", "r");
	if (!data_list) {
		fprintf (stderr, "should been a file data.list\n");
		return -1;
	}

	char line[512];
	while (fgets (line, 512, data_list)) {
		uint32_t num;
		char name[256];
		char defined_name[256] = {0, };
		uint32_t type;
		sscanf (line, "%d %s %d", &num, name, &type);

		make_defined_name (defined_name, name);

		switch (type) {
			case 0:
				fs[fs_count_file_info].num = num;
				strncpy (fs[fs_count_file_info].name, name, strlen (name));
				strncpy (fs[fs_count_file_info].define_name, defined_name, strlen (defined_name));
				fs_count_file_info++;
				if (fs_count_file_info >= fs_cur_size_block)
					fs_restructure_blocks ();
				break;
			case 1:
				fffs[fffs_count_file_info].num = num;
				strncpy (fffs[fffs_count_file_info].name, name, strlen (name));
				strncpy (fffs[fffs_count_file_info].define_name, defined_name, strlen (defined_name));
				fffs_count_file_info++;
				if (fffs_count_file_info >= fffs_cur_size_block)
					fffs_restructure_blocks ();
				break;
			case 2:
				fts[fts_count_file_info].num = num;
				strncpy (fts[fts_count_file_info].name, name, strlen (name));
				strncpy (fts[fts_count_file_info].define_name, defined_name, strlen (defined_name));
				fts_count_file_info++;
				if (fts_count_file_info >= fts_cur_size_block)
					fts_restructure_blocks ();
				break;
			case 3:
				fss[fss_count_file_info].num = num;
				strncpy (fss[fss_count_file_info].name, name, strlen (name));
				strncpy (fss[fss_count_file_info].define_name, defined_name, strlen (defined_name));
				fss_count_file_info++;
				if (fss_count_file_info >= fss_cur_size_block)
					fss_restructure_blocks ();
				break;
		}



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

	uint64_t group_pos_shaders = 0L;
	uint64_t group_pos_fonts = 0L;
	uint64_t group_pos_textures = 0L;
	uint64_t group_pos_sounds = 0L;
	uint64_t off = 0L;
	/*
	 * write shaders positions and tell group where it location
	 */
	group_pos_shaders = ftell (dt);
	fseek (dt, sizeof (uint64_t) + POS_SHADERS, SEEK_SET);
	fwrite (&group_pos_shaders, sizeof (uint64_t), 1, dt);
	fseek (dt, group_pos_shaders, SEEK_SET);

	for (int i = 0; i < fs_count_file_info; i++) {
		fwrite (&off, sizeof (uint64_t), 1, dt);
	}

	group_pos_fonts = ftell (dt);
	fseek (dt, sizeof (uint64_t) + POS_FONTS, SEEK_SET);
	fwrite (&group_pos_fonts, sizeof (uint64_t), 1, dt);
	fseek (dt, group_pos_fonts, SEEK_SET);

	for (int i = 0; i < fffs_count_file_info; i++) {
		fwrite (&off, sizeof (uint64_t), 1, dt);
	}

	group_pos_textures = ftell (dt);
	fseek (dt, sizeof (uint64_t) + POS_TEXTURES, SEEK_SET);
	fwrite (&group_pos_textures, sizeof (uint64_t), 1, dt);
	fseek (dt, group_pos_textures, SEEK_SET);

	for (int i = 0; i < fts_count_file_info; i++) {
		fwrite (&off, sizeof (uint64_t), 1, dt);
	}

	group_pos_sounds = ftell (dt);
	fseek (dt, sizeof (uint64_t) + POS_SOUND, SEEK_SET);
	fwrite (&group_pos_sounds, sizeof (uint64_t), 1, dt);
	fseek (dt, group_pos_sounds, SEEK_SET);

	for (int i = 0; i < fss_count_file_info; i++) {
		fwrite (&off, sizeof (uint64_t), 1, dt);
	}

	/* 
	 * work with shaders
	 */
	uint64_t i = 0;
	for (; i < fs_count_file_info; i++) {
		FILE *file = fopen (fs[i].name, "r");
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

		fseek (dt, group_pos_shaders + sizeof (uint64_t) * i, SEEK_SET);
		fwrite (&off, sizeof (uint64_t), 1, dt);

		fseek (dt, pos_continue, SEEK_SET);

		fprintf (log, "%s = %lx\n", fs[i].name, off);

		free (data);
	}

	/* 
	 * work with fonts
	 */
	i = 0;
	for (; i < fffs_count_file_info; i++) {
		FILE *file = fopen (fffs[i].name, "r");
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

		fseek (dt, group_pos_fonts + sizeof (uint64_t) * i, SEEK_SET);
		fwrite (&off, sizeof (uint64_t), 1, dt);

		fseek (dt, pos_continue, SEEK_SET);

		fprintf (log, "%s = %lx\n", fffs[i].name, off);

		free (data);
	}

	/* 
	 * work with textures
	 */
	i = 0;
	for (; i < fts_count_file_info; i++) {
		FILE *file = fopen (fts[i].name, "r");
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

		fseek (dt, group_pos_textures + sizeof (uint64_t) * i, SEEK_SET);
		fwrite (&off, sizeof (uint64_t), 1, dt);

		fseek (dt, pos_continue, SEEK_SET);

		fprintf (log, "%s = %lx\n", fts[i].name, off);

		free (data);
	}

	/* 
	 * work with sounds
	 */
	i = 0;
	for (; i < fss_count_file_info; i++) {
		FILE *file = fopen (fss[i].name, "r");
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

		fseek (dt, group_pos_sounds + sizeof (uint64_t) * i, SEEK_SET);
		fwrite (&off, sizeof (uint64_t), 1, dt);

		fseek (dt, pos_continue, SEEK_SET);

		fprintf (log, "%s = %lx\n", fss[i].name, off);

		free (data);
	}

	fclose (log);
	fclose (dt);
}
