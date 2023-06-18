#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <asgl.h>
#include <Engine/Core/Downloader.h>
#include <Game/Core/Defines.h>
#include <Engine/Utils/File.h>
#include <Game/Core/ResEnum.h>

static uint32_t count_sprite_array;

static VertexData **vdata;
static VertexData **vsprites;
static VertexData **vsprites_center;

static void load_model (const char *filename, uint32_t asset);
static void load_textures (const char *filename, uint32_t asset);
static void load_sprite (uint64_t);

void downloader_clear_memory_of_sprites ()
{
	for (uint32_t i = 0; i < count_sprite_array; i++) {
		delete vsprites[i];
	}

	delete[] vsprites;
}

void downloader_init_sprite_array (uint32_t count)
{
	count_sprite_array = count;
	vsprites = new VertexData*[count];
}

void downloader_download_all_sprites_without_progressbar_by_range (uint32_t min, uint32_t max)
{
    for (uint32_t i = min; i < max; i++) {
        load_sprite (i);
    }
}

void downloader_download_all_sprites_without_progressbar (std::vector<InfoSprite>& list)
{
	for (uint32_t i = 0; i < list.size (); i++) {
		downloader_download_each_sprite (list[i]);
	}
}

void downloader_download_each_sprite (InfoSprite& item)
{
#if 0
	std::string path = "assets/" + item.path + ".texture";
	load_sprite (path.c_str(), item.index);
#endif
	load_sprite (item.enum_pos);
}


VertexData *downloader_load_sprite (uint32_t asset)
{
	return vsprites[asset];
}

VertexData *downloader_object_load (uint32_t asset)
{
#if 0
	switch (asset) {
		case MODEL_TILE_GROUND:
			if (vdata[MODEL_TILE_GROUND]) break;
			load_model ("assets/tile_ground.object", MODEL_TILE_GROUND);
			load_textures ("assets/tile_ground.texture", MODEL_TILE_GROUND);
			break;
		case MODEL_TILE_GRASS:
			if (vdata[MODEL_TILE_GRASS]) break;
			load_model ("assets/tile_grass.object", MODEL_TILE_GRASS);
			load_textures ("assets/tile_grass.texture", MODEL_TILE_GRASS);
			break;
		case MODEL_TILE_BUSHES:
			if (vdata[MODEL_TILE_BUSHES]) break;
			load_model ("assets/tile_bushes.object", MODEL_TILE_BUSHES);
			load_textures ("assets/tile_bushes.texture", MODEL_TILE_BUSHES);
			break;
		case MODEL_TILE_RIVER:
			if (vdata[MODEL_TILE_RIVER]) break;
			load_model ("assets/tile_river.object", MODEL_TILE_RIVER);
			load_textures ("assets/tile_river.texture", MODEL_TILE_RIVER);
			break;
		case MODEL_TILE_TREE:
			if (vdata[MODEL_TILE_TREE]) break;
			load_model ("assets/tile_tree.object", MODEL_TILE_TREE);
			load_textures ("assets/tile_tree.texture", MODEL_TILE_TREE);
			break;
		case MODEL_HUMAN:
			if (vdata[MODEL_HUMAN]) break;
			load_model ("assets/human.object", MODEL_HUMAN);
			load_textures ("assets/human.texture", MODEL_HUMAN);
			break;
		case MODEL_BERRY:
			if (vdata[MODEL_BERRY]) break;
			load_model ("assets/berry.object", MODEL_BERRY);
			load_textures ("assets/berry.texture", MODEL_BERRY);
			break;
		case MODEL_CARD:
			if (vdata[MODEL_CARD]) break;
			load_model ("assets/card.object", MODEL_CARD);
			load_textures ("assets/card.texture", MODEL_CARD);
			break;
		case MODEL_ENEMY_WOLF:
			if (vdata[MODEL_ENEMY_WOLF]) break;
			load_model ("assets/wolf.object", MODEL_ENEMY_WOLF);
			load_textures ("assets/wolf.texture", MODEL_ENEMY_WOLF);
			break;
	}
#endif
	return vdata[asset];
}


static void load_model (const char *filename, uint32_t asset)
{
	FILE *fp = fopen (filename, "r");
	int lb = 0;
	int count = 0;
	int size;
	fread (&lb, sizeof (int), 1, fp);
	fread (&count, sizeof (int), 1, fp);

	vdata[asset] = new VertexData();
	vdata[asset]->f = new float*[count];
	vdata[asset]->size_f = count;
	uint32_t indexf = 0;

	for (int i = 0; i < count; i++) {
		fread (&size, sizeof (int), 1, fp);
		int totl = sizeof (float) * size * 24;
		float *f = new float[sizeof (float) * size * 24];
		int ret = fread (f, 1, totl, fp);
		vdata[asset]->f[indexf++] = f;
	}

	fclose (fp);
		
	vdata[asset]->count_v = size * 24;
}



static void load_sprite (uint64_t enum_pos)
{
	uint32_t asset = enum_pos;
	vsprites[enum_pos] = new VertexData();

	uint64_t size_file;
	uint8_t *file = Utils::file_get_game_data (enum_pos, size_file, R_TEXTURES);

	const int LTBE = 0;
        const int COUNT = 1;
        const int WIDTH = 2;
        const int HEIGHT = 3;
        int *pack[4];

        for (int i = 0; i < 4; i++) {
                pack[i] = (int *) &file[i * 4];
        }

        if (*pack[LTBE] != 1) {
                for (int i = 1; i < 4; i++) {
                        *pack[i] = Utils::swap_little_big_engian (*pack[i]);
                }
        }

        int size_tex = *pack[COUNT];
        int width = *pack[WIDTH];
        int height = *pack[HEIGHT];

        uint8_t **data = new uint8_t *[size_tex];

        int pos = 4 * sizeof (int);

        int total = width * height * 4;

        for (int i = 0; i < size_tex; i++) {
                data[i] = new uint8_t[total];
                memcpy (data[i], &file[pos], total);
                pos += total;
        }

	vsprites[asset]->tex_count = size_tex;
	vsprites[asset]->tex_width = width;
	vsprites[asset]->tex_height = height;

	vsprites[asset]->tex_sampler = new uint32_t[size_tex];
	glGenTextures (size_tex, vsprites[asset]->tex_sampler);
	

	for (int i = 0; i < size_tex; i++) {
		glBindTexture (GL_TEXTURE_2D, vsprites[asset]->tex_sampler[i]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture (GL_TEXTURE_2D, 0);
                //delete[] data[i];
	}

	//delete[] data;
	
	vsprites[asset]->data = data;

	float ww = static_cast<float>(width);
	float hh = static_cast<float>(height);


	static float v[30] = {
		0.f, 0.f, 0.0f, 1.0f, 1.0f,
		0.f, hh, 0.0f, 1.0f, 0.0f,
		ww, 0.f, 0.0f, 0.0f, 1.0f,
		ww, 0.f, 0.0f, 0.0f, 1.0f,
		ww, hh, 0.0f, 0.0f, 0.0f,
		0.f, hh, 0.0f, 1.0f, 0.0f
	};

	uint32_t count = 30;
	vsprites[asset]->f = new float*[1];
	vsprites[asset]->f[0] = new float[30];
	vsprites[asset]->size_f = 1;
	vsprites[asset]->count_v = 30;
	memcpy (vsprites[asset]->f[0], v, sizeof(float) * count);
}

static void load_sprite_center (const char *filename, uint32_t asset)
{
	vsprites_center[asset] = new VertexData();

	FILE *fp = fopen (filename, "r");

	uint8_t *file;

	fseek (fp, 0, SEEK_END);
	uint64_t size_file = ftell (fp);
	fseek (fp, 0, SEEK_SET);

	file = new uint8_t[size_file];
	fread (file, size_file, 1, fp);
	fclose (fp);



	const int LTBE = 0;
        const int COUNT = 1;
        const int WIDTH = 2;
        const int HEIGHT = 3;
        int *pack[4];

        for (int i = 0; i < 4; i++) {
                pack[i] = (int *) &file[i * 4];
        }

        if (*pack[LTBE] != 1) {
                for (int i = 1; i < 4; i++) {
                        *pack[i] = Utils::swap_little_big_engian (*pack[i]);
                }
        }

        int size_tex = *pack[COUNT];
        int width = *pack[WIDTH];
        int height = *pack[HEIGHT];

        uint8_t **data = new uint8_t *[size_tex];

        int pos = 4 * sizeof (int);

        int total = width * height * 4;

        for (int i = 0; i < size_tex; i++) {
                data[i] = new uint8_t[total];
                memcpy (data[i], &file[pos], total);
                pos += total;
        }

	vsprites_center[asset]->tex_count = size_tex;
	vsprites_center[asset]->tex_width = width;
	vsprites_center[asset]->tex_height = height;

	vsprites_center[asset]->tex_sampler = new uint32_t[size_tex];
	glGenTextures (size_tex, vsprites_center[asset]->tex_sampler);
	

	for (int i = 0; i < size_tex; i++) {
		glBindTexture (GL_TEXTURE_2D, vsprites_center[asset]->tex_sampler[i]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture (GL_TEXTURE_2D, 0);
                delete[] data[i];
	}

	delete[] data;

	float w, h;
	if (width > height) {
		w = static_cast<float>(1.f);
		float aspect = static_cast<float>(width) / static_cast<float>(height);
		h = w / aspect;
	} else if (width < height) {
		h = static_cast<float>(1.f);
		float aspect = static_cast<float>(height) / static_cast<float>(width);
		w = h / aspect;
	} else {
		h = w = 1.f;
	}

	float ww = static_cast<float>(w);
	float hh = static_cast<float>(h);

#if 0
	static float v[30] = {
		-ww, -hh, 0.0f, 1.0f, 1.0f,
		-ww, hh, 0.0f, 1.0f, 0.0f,
		ww, -hh, 0.0f, 0.0f, 1.0f,
		ww, -hh, 0.0f, 0.0f, 1.0f,
		ww, hh, 0.0f, 0.0f, 0.0f,
		-ww, hh, 0.0f, 1.0f, 0.0f
	};
#else
	static float v[30] = {
		0.f, 0.f, 0.0f, 1.0f, 1.0f,
		0.f, hh, 0.0f, 1.0f, 0.0f,
		ww, 0.f, 0.0f, 0.0f, 1.0f,
		ww, 0.f, 0.0f, 0.0f, 1.0f,
		ww, hh, 0.0f, 0.0f, 0.0f,
		0.f, hh, 0.0f, 1.0f, 0.0f
	};
#endif

	uint32_t count = 30;
	vsprites_center[asset]->f = new float*[1];
	vsprites_center[asset]->f[0] = new float[30];
	vsprites_center[asset]->size_f = 1;
	vsprites_center[asset]->count_v = 30;
	memcpy (vsprites_center[asset]->f[0], v, sizeof(float) * 30);
}

static void load_textures (const char *filename, uint32_t asset)
{
	FILE *fp = fopen (filename, "r");

	uint8_t *file;

	fseek (fp, 0, SEEK_END);
	uint64_t size_file = ftell (fp);
	fseek (fp, 0, SEEK_SET);

	file = new uint8_t[size_file];
	fread (file, size_file, 1, fp);
	fclose (fp);

	const int LTBE = 0;
        const int COUNT = 1;
        const int WIDTH = 2;
        const int HEIGHT = 3;
        int *pack[4];

        for (int i = 0; i < 4; i++) {
                pack[i] = (int *) &file[i * 4];
        }

        if (*pack[LTBE] != 1) {
                for (int i = 1; i < 4; i++) {
                        *pack[i] = Utils::swap_little_big_engian (*pack[i]);
                }
        }

        int size_tex = *pack[COUNT];
        int width = *pack[WIDTH];
        int height = *pack[HEIGHT];


        uint8_t **data = new uint8_t *[size_tex];

        int pos = 4 * sizeof (int);

        int total = width * height * 4;

        for (int i = 0; i < size_tex; i++) {
                data[i] = new uint8_t[total];
                memcpy (data[i], &file[pos], total);
                pos += total;
        }

	vdata[asset]->tex_width = width;
	vdata[asset]->tex_height = height;

	vdata[asset]->tex_count = size_tex;

	vdata[asset]->tex_sampler = new uint32_t[size_tex];
	glGenTextures (size_tex, vdata[asset]->tex_sampler);

	for (int i = 0; i < size_tex; i++) {
		glBindTexture (GL_TEXTURE_2D, vdata[asset]->tex_sampler[i]);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture (GL_TEXTURE_2D, 0);
                delete[] data[i];
	}

	delete[] data;
}

static int get_count_objects_in_level (char *file)
{
	FILE *fp = fopen (file, "r");
	fseek (fp, 0 - 4, SEEK_END);

	int count = 0;
	fread (&count, sizeof (int), 1, fp);
	fclose (fp);

	return count;
}
