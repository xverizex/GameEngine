#ifndef TEXT_H
#define TEXT_H
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <stdint.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <wchar.h>
#include <string>



struct Character {
    bool is_not_destroy;
	uint32_t TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	int64_t Advance;
	bool builded = {false};
};

#define SIZE_LIST_FACE                          255
#define SIZE_COUNT_FONTS                         10

class Text {
	public:
		static Text *getInstance ();

		void resizeMatrix();
		void init_index (uint32_t index, uint64_t pos, uint32_t pixelSize);
        void uninit (uint32_t);
        void init (uint32_t);
        void init_shaders ();
        void build_text ();
		void render (uint32_t index, const wchar_t *text, int x, int y, float scale, glm::vec3 color, bool only_know, uint32_t &_width, uint32_t &height);

		FT_Library ft;
		FT_Face *face;
		Character *list_face[SIZE_COUNT_FONTS][SIZE_LIST_FACE];
        bool init_list_face = {false};
		glm::mat4 projection;

		uint32_t program;
		uint32_t uniform_projection;
		uint32_t uniform_sampler;
		uint32_t uniform_text_color;
		uint32_t vao;
		uint32_t vbo;
        uint32_t size;
	private:
		Text(uint32_t size);

};
#endif
