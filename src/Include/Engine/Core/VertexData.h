#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H
#include <stdint.h>

struct VertexData {
	~VertexData ();
	float **f = {nullptr};
	uint32_t size_f;
	uint32_t max_vertex;
	uint32_t count_v;
	uint32_t tex_count;
	uint32_t tex_width;
	uint32_t tex_height;
	uint32_t *tex_sampler;
	uint8_t **data = {nullptr};
};

#endif
