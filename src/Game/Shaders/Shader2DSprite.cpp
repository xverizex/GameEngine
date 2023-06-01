#include <ShaderClasses/Shader2DSprite.h>
#include <Core/Shaders.h>
#include <glad/glad.h>
#include <Core/Sprite.h>
#include <stdio.h>
#include <time.h>


void Shader2DSprite::init()
{
	program = getProgram(SHADER_2D_SPRITE);

	glUseProgram(program);

	uniform_model = glGetUniformLocation(program, "model");
	uniform_projection = glGetUniformLocation(program, "ortho");
	uniform_translation = glGetUniformLocation(program, "pos");
	uniform_scale = glGetUniformLocation(program, "scale");
	uniform_sampler = glGetUniformLocation(program, "s_texture");
}

void Shader2DSprite::render(void *_data)
{
	Sprite *obj = static_cast<Sprite *>(_data);

	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, obj->vertexData->tex_sampler[obj->cur_tex]);
	
	glUniform1i(uniform_sampler, 0);

	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &obj->model()[0][0]);
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &obj->projection()[0][0]);
	glUniformMatrix4fv(uniform_translation, 1, GL_FALSE, &obj->position()[0][0]);
	glUniformMatrix4fv(uniform_scale, 1, GL_FALSE, &obj->scale()[0][0]);
	
	
	glBindVertexArray(obj->vao[0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}


