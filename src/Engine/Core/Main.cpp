#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <stdint.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/Global.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/GameConfig.h>

int
main(int argc, char **argv)
{
	uint32_t screen_width = 100;
       	uint32_t screen_height = 100;
	bool is_fullscreen = false;

	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	ScreenManager* screen_manager = new ScreenManager ();
	Global::set_singleton (screen_manager);

	GameConfig* game_config = new GameConfig ();
	game_config->init_screen ();

	screen_manager->get_screen_size (screen_width, screen_height, is_fullscreen);

	uint32_t flags_window = SDL_WINDOW_OPENGL | (is_fullscreen ? SDL_WINDOW_FULLSCREEN: 0);

	SDL_Window *window = SDL_CreateWindow ("TileNinjaHackers",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			flags_window
			);


	SDL_GLContext ctx = SDL_GL_CreateContext (window);

	ShaderManager* shader_manager = new ShaderManager (N_SHADERS);

	Global::set_singleton (shader_manager);


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
