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

enum TYPE_FONTS {
	MAIN_FONT_28,
	N_TYPE_FONTS
};

struct Character {
	uint32_t TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	int64_t Advance;
	bool builded = {false};
};

class Text {
	public:
		static Text *getInstance ();

		void resizeMatrix();
		void init (uint32_t index, uint64_t pos, uint32_t pixelSize);
		void render (uint32_t index, const wchar_t *text, int x, int y, float scale, glm::vec3 color, bool only_know, uint32_t &_width, uint32_t &height);

		FT_Library ft;
		FT_Face *face;
		std::vector<std::map<wchar_t, Character>> list_face;
		glm::mat4 projection;

		uint32_t program;
		uint32_t uniform_projection;
		uint32_t uniform_sampler;
		uint32_t uniform_text_color;
		uint32_t vao;
		uint32_t vbo;
	private:
		Text(uint32_t size);

};
#endif
