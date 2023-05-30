#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <stdint.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/Global.h>
#include <Game/Core/GameConfig.h>

uint32_t screen_width;
uint32_t screen_height;

int
main(int argc, char **argv)
{
	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	screen_width = 600;
	screen_height = 1024;

	uint32_t flags_window = SDL_WINDOW_OPENGL;

	SDL_Window *window = SDL_CreateWindow ("TileNinjaHackers",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			flags_window
			);


	SDL_GLContext ctx = SDL_GL_CreateContext (window);

	ShaderManager* shader_manager = new ShaderManager (N_SHADERS);

	Global::set_singleton (shader_manager);

	GameConfig* game_config = new GameConfig ();

	game_config->init_shaders ();

	SDL_Event event;

	while (1) {
		const uint8_t *key = SDL_GetKeyboardState (NULL);
		if (key[SDL_SCANCODE_Q])
			exit (0);

		SDL_PollEvent (&event);

		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				break;
		}

		SDL_GL_SwapWindow (window);

		SDL_Delay (16);
	}
}
