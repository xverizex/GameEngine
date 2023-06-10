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
	if (!global) global = new Text(N_FONTS_RES);

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

void Text::init_shaders ()
{
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

Text::Text(uint32_t _size)
{
    size = _size;

    if (FT_Init_FreeType(&ft)) {
        printf ("error freetype init\n");
        exit (1);
    }
}

static wchar_t t[] = {L"qwertyuiop[{]}|\\asdfghjkl;:'\"zxcvbnm,<.>/?}QWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*()-_=+ "};

void Text::uninit (uint32_t size)
{
    if (init_list_face) {
        for (int i = 0; i < SIZE_COUNT_FONTS; i++) {
            for (int sym = 0; sym < SIZE_LIST_FACE; sym++) {
                if (list_face[i][sym] && !list_face[i][sym]->is_not_destroy) {
                    glDeleteTextures(1, &list_face[i][sym]->TextureID);
                    delete list_face[i][sym];
                }
            }

        }

        for (int i = 0; i < size; i++) {
            FT_Done_Face(face[i]);
        }

        delete[] face;
    }

    glDeleteVertexArrays (1, &vao);
    glDeleteBuffers (1, &vbo);

    FT_Done_FreeType(ft);

    init_list_face = false;
}

void Text::build_text ()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    for (int index_face = 0; index_face < SIZE_COUNT_FONTS; index_face++) {
        for (int i = 0; i < SIZE_LIST_FACE; i++) {
            list_face[index_face][i] = nullptr;
        }
    }

    uint32_t st = sizeof (t) / sizeof (wchar_t);

    for (int index_face = 0; index_face < size; index_face++) {
        for (int i = 0; i < st; i++) {

            if (FT_Load_Char(face[index_face], t[i], FT_LOAD_RENDER)) {
                printf ("failed to load glyph\n");
                exit (1);
            }

            uint32_t texture = 0;
            bool is_not_destroy = false;

            if (t[i] != ' ') {
                glGenTextures (1, &texture);

                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face[index_face]->glyph->bitmap.width,
                        face[index_face]->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face[index_face]->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                is_not_destroy = true;
            }



            Character *character = new Character {
                    is_not_destroy,
                    texture,
                    glm::ivec2(face[index_face]->glyph->bitmap.width, face[index_face]->glyph->bitmap.rows),
                    glm::ivec2(face[index_face]->glyph->bitmap_left, face[index_face]->glyph->bitmap_top),
                    face[index_face]->glyph->advance.x,
                    true
            };


            list_face[index_face][t[i]] = character;
        }
    }
}

void Text::init (uint32_t size)
{
    face = new FT_Face[size];

    if (FT_Init_FreeType(&ft)) {
        printf ("error freetype init\n");
        exit (1);
    }





    init_list_face = true;
}

void Text::init_index(uint32_t index, uint64_t enum_pos, uint32_t pixelSize)
{
	uint64_t size = 0L;
	uint8_t* data = Utils::file_get_game_data (enum_pos, size, R_FONTS);

	FT_New_Memory_Face (ft, data, size, 0, &face[index]);

    delete[] data;

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

	if (onlyKnow == false) {

		glUseProgram(program);

		glUniform3f(uniform_text_color, color.x, color.y, color.z);
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &projection[0][0]);


		glBindVertexArray(vao);

		glUniform1i (uniform_sampler, 0);
		glActiveTexture(GL_TEXTURE0);


		for (uint32_t i = 0; i < len; i++) {
		

			Character* ch = list_face[index][text[i]];
            if (!ch) {
                exit (0);
            }

			float xpos = x + (float)ch->Bearing.x * scale;
			float ypos = y - (float)(ch->Size.y - ch->Bearing.y) * scale;
			_height = ch->Size.y > _height? ch->Size.y: _height;

			float www = (float) ch->Size.x * scale;
			float hhh = (float) ch->Size.y * scale;

			float vertices[6 * 4] = {
				xpos,		ypos + hhh,	0.f, 0.f,
				xpos,		ypos, 		0.f, 1.f,
				xpos + www,	ypos,		1.f, 1.f,
				xpos,		ypos + hhh,	0.f, 0.f,
				xpos + www,	ypos, 		1.f, 1.f,
				xpos + www, ypos + hhh, 	1.f, 0.f
			};

            if (!ch->is_not_destroy) {
                glBindTexture(GL_TEXTURE_2D, ch->TextureID);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }


			x += (float)(ch->Advance >> 6) * scale;

			_width += static_cast<uint32_t>((float)(ch->Advance >> 6) * scale);
		}

		glBindVertexArray(0);
	} else {
		for (uint32_t i = 0; i < len; i++) {
		

			Character* ch = list_face[index][text[i]];
			_height = (uint32_t)((float) ch->Size.y * scale) > _height? (uint32_t)(((float) ch->Size.y) * scale): _height;
			x += (float)(ch->Advance >> 6) * scale;
			_width += static_cast<uint32_t>((float)(ch->Advance >> 6) * scale);
		}
		
	}
}
