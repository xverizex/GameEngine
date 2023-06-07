#include <Engine/Core/Text.h>
#include <cstdio>
#include <cstdlib>
#include <asgl.h>
#include <Engine/Core/ScreenManager.h>
#include <Engine/Core/ShaderManager.h>
#include <Game/Core/ShaderList.h>
#include <Game/Core/Defines.h>
#include <Engine/Utils/File.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Game/Core/ResEnum.h>

static Text *global;

#define DELIMITER_SPACE                     30

Text *Text::getInstance()
{
	if (!global) global = new Text(N_TYPE_FONTS);

	return global;
}

void Text::resizeMatrix()
{
	ScreenManager* screen_manager = ScreenManager::get_instance();

	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	float aspect = w / h;

	projection = glm::ortho (0.f, w, 0.f, h, -1.0f, 10.f);
}

Text::Text(uint32_t size)
{


	face = new FT_Face[size];
	list_face.resize(size);

	if (FT_Init_FreeType(&ft)) {
		printf ("error freetype init\n");
		exit (1);
	}

	ShaderManager* shader_manager = ShaderManager::get_instance ();
        program = shader_manager->get_program(SHADER_TEXT);

	glUseProgram(program);

	uniform_projection = glGetUniformLocation(program, "projection");
	uniform_sampler = glGetUniformLocation(program, "text");
	uniform_text_color = glGetUniformLocation(program, "textColor");

	ScreenManager* screen_manager = ScreenManager::get_instance();

	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	float aspect = w / h;

	projection = glm::ortho (0.f, w, 0.f, h, -1.0f, 10.f);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::init(uint32_t index, uint64_t enum_pos, uint32_t pixelSize)
{
	uint64_t size = 0L;
	uint8_t* data = Utils::file_get_game_data (enum_pos, size, R_FONTS);

	FT_New_Memory_Face (ft, data, size, 0, &face[index]);
#if 0
	if (FT_New_Face(ft, path.c_str(), 0, &face[index])) {
		printf ("error new face by path: %s\n", path.c_str());
		exit (1);
	}
#endif

	FT_Set_Pixel_Sizes (face[index], 0, pixelSize);
}

void Text::render(uint32_t index, const wchar_t *text, int xx, int yy, float scale, glm::vec3 color, bool onlyKnow, uint32_t &_width, uint32_t &_height)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	_width = _height = 0;

	ScreenManager* screen_manager = ScreenManager::get_instance();

	float w = screen_manager->width_float;
	float h = screen_manager->height_float;

	float aspect = w / h;

	float x = static_cast<float>(xx);
	float y = static_cast<float>(yy);

	projection = glm::ortho (0.f, w, 0.f, h, -1.0f, 10.f);


	uint32_t len = wcslen (text);

	
		for (uint32_t i = 0; i < len; i++) {
	 		if (list_face[index][text[i]].builded == true) continue;

			if (FT_Load_Char(face[index], text[i], FT_LOAD_RENDER)) {
				printf ("failed to load glyph\n");
				exit (1);
			}

			uint32_t texture;
			glGenTextures (1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face[index]->glyph->bitmap.width,
				face[index]->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face[index]->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character = {
				texture,
				glm::ivec2(face[index]->glyph->bitmap.width, face[index]->glyph->bitmap.rows),
				glm::ivec2(face[index]->glyph->bitmap_left, face[index]->glyph->bitmap_top),
				face[index]->glyph->advance.x,
				true
			};

			list_face[index][text[i]] = character;
		}

	if (onlyKnow == false) {

		glUseProgram(program);

		glUniform3f(uniform_text_color, color.x, color.y, color.z);
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &projection[0][0]);


		glBindVertexArray(vao);

		glUniform1i (uniform_sampler, 0);
		glActiveTexture(GL_TEXTURE0);

		for (uint32_t i = 0; i < len; i++) {
		

			Character ch = list_face[index][text[i]];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
			_height = ch.Size.y > _height? ch.Size.y: _height;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[6 * 4] = {
				xpos,		ypos + h,	0.f, 0.f,
				xpos,		ypos, 		0.f, 1.f,
				xpos + w,	ypos,		1.f, 1.f,
				xpos,		ypos + h,	0.f, 0.f,
				xpos + w,	ypos, 		1.f, 1.f,
				xpos + w, ypos + h, 		1.f, 0.f
			};

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.Advance >> 6) * scale;

			_width += static_cast<uint32_t>((ch.Advance >> 6) * scale);
		}


		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	} else {
		for (uint32_t i = 0; i < len; i++) {
		

			Character ch = list_face[index][text[i]];
			_height = ch.Size.y * scale > _height? ch.Size.y * scale: _height;
			x += (ch.Advance >> 6) * scale;
			_width += static_cast<uint32_t>((ch.Advance >> 6) * scale);
		}
		
	}
}
