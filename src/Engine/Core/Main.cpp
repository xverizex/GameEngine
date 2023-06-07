#include <SDL2/SDL.h>
#include <stdint.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/GameConfig.h>
#include <Game/Core/ShaderList.h>
#include <Engine/Core/Text.h>
#include <Game/Core/ResEnum.h>
#include <clocale>
#include <glad/glad.h>

static GameConfig* game_config;
void
handle_event (SDL_Event& event)
{
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			{
				SDL_MouseButtonEvent *ev = (SDL_MouseButtonEvent *) &event;
				game_config->cur_level->mouse_click (ev->x, ev->y);
			}
			break;
		case SDL_KEYDOWN:
			{
				SDL_KeyboardEvent *ev = (SDL_KeyboardEvent *) &event;
				game_config->cur_level->key_click (ev->keysym);
			}
	}
}
int
event_linux (void* _data)
{
	SDL_Event event;

	while (SDL_WaitEvent (&event)) {
		handle_event (event);
	}

	return 0;
}

int
main(int argc, char **argv)
{
	std::setlocale (LC_ALL, "en_US.UTF-8");

	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	//SDL_GL_LoadLibrary (nullptr);

#if 1
	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);

#endif

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	ScreenManager* screen_manager = ScreenManager::get_instance ();

	game_config = GameConfig::get_instance ();
	game_config->init_screen ();

	uint32_t flags_window = SDL_WINDOW_OPENGL | (screen_manager->is_fullscreen ? SDL_WINDOW_FULLSCREEN: 0);

	SDL_Window *window = SDL_CreateWindow ("HexFrontier",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_manager->width,
			screen_manager->height,
			flags_window
			);


	SDL_GLContext ctx = SDL_GL_CreateContext (window);

	SDL_GL_MakeCurrent (window, ctx);

	int ret = gladLoadGLLoader(SDL_GL_GetProcAddress);

	ShaderManager* shader_manager = ShaderManager::get_instance ();
	shader_manager->set_shader_size (N_SHADERS);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport (0, 0, screen_manager->width, screen_manager->height);

	game_config->init_shaders ();

	Text* text = Text::getInstance ();
	text->init (0, RES_TERMINUS_TTF, 48);


	game_config->list_of_levels ();
	game_config->entry_point ();

	SDL_CreateThread (event_linux, "thread", game_config);

#ifdef WIN32
	SDL_Event event;
	while (1) {
		SDL_PollEvent(&event);
		handle_event(event);
#else
	while (1) {
#endif	

		game_config->cur_level->clear_screen ();

		game_config->cur_level->tick ();

		game_config->cur_level->render ();

		SDL_GL_SwapWindow (window);

		SDL_Delay (16);
	}

	return 0;
}
