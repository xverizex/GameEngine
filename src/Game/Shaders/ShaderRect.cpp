#include <Game/Shaders/ShaderRect.h>
#include <Engine/Core/ShaderManager.h>
#include <Game/Core/ShaderList.h>
#include <asgl.h>
#include <cstdio>

void ShaderRect::init()
{
	ShaderManager* shader_manager = ShaderManager::get_instance ();

	program = shader_manager->get_program(SHADER_RECT);

	glUseProgram(program);

	uniform_color = glGetUniformLocation(program, "color");
	uniform_top_left_radius = glGetUniformLocation(program, "top_left_radius");
	uniform_top_right_radius = glGetUniformLocation(program, "top_right_radius");
	uniform_bottom_left_radius = glGetUniformLocation(program, "bottom_left_radius");
	uniform_bottom_right_radius = glGetUniformLocation(program, "bottom_right_radius");
	uniform_position = glGetUniformLocation(program, "position");
	uniform_projection = glGetUniformLocation(program, "projection");
	uniform_model = glGetUniformLocation(program, "model");
	uniform_rotate = glGetUniformLocation(program, "rotate");
	uniform_pos = glGetUniformLocation(program, "pos");
	uniform_size_quad = glGetUniformLocation(program, "dimension");
}

void ShaderRect::render(void *_data)
{
	Rect *r = static_cast<Rect *>(_data);

	glUseProgram(program);

	glUniform2f(uniform_size_quad, r->width, r->height);
	glUniform1f (uniform_top_left_radius, r->top_left_radius);
	glUniform1f (uniform_top_right_radius, r->top_right_radius);
	glUniform1f (uniform_bottom_left_radius, r->bottom_left_radius);
	glUniform1f (uniform_bottom_right_radius, r->bottom_right_radius);
	glUniform4fv(uniform_color, 1, &r->getColor()[0]);
	glUniform3fv(uniform_pos, 1, &r->getPosVector()[0]);
	glUniformMatrix4fv(uniform_position, 1, GL_FALSE, &r->position()[0][0]);
	glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &r->projection()[0][0]);
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &r->model()[0][0]);
	glUniformMatrix4fv(uniform_rotate, 1, GL_FALSE, &r->rotate()[0][0]);


	glBindVertexArray(r->vao[0]);
		
	glDrawArrays(GL_TRIANGLES, 0, r->vertexData->max_vertex);

}
