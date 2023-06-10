#include <stdint.h>
#include <Engine/Core/ShaderManager.h>
#include <Engine/Core/ScreenManager.h>
#include <Game/Core/GameConfig.h>
#include <Game/Core/ShaderList.h>
#include <Engine/Core/Text.h>
#include <Game/Core/ResEnum.h>
#include <clocale>
#include <asgl.h>
#ifdef __ANDROID__
#include "SDL.h"
#include <jni.h>
#include "../Trasnport/Transport.h"
JNIEnv* global_env;
#else
#include <SDL2/SDL.h>
#endif


static GameConfig* game_config;
static bool is_resume = false;
static SDL_Window *window;
static SDL_GLContext ctx;
static ScreenManager* screen_manager;
static ShaderManager* shader_manager;
static Text* text;

void
handle_event (SDL_Event& event)
{
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			{
				SDL_MouseButtonEvent *ev = (SDL_MouseButtonEvent *) &event;
				game_config->cur_level->mouse_click_down (ev->x, ev->y);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			{
				SDL_MouseButtonEvent *ev = (SDL_MouseButtonEvent *) &event;
				game_config->cur_level->mouse_click_up (ev->x, ev->y);
			}
			break;
		case SDL_MOUSEMOTION:
			{
				SDL_MouseMotionEvent *ev = (SDL_MouseMotionEvent *) &event;
				game_config->cur_level->mouse_motion (ev->x, ev->y);
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

void pc_respawn_gl_context ()
{
    shader_manager->set_shader_size (N_SHADERS);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport (0, 0, screen_manager->width, screen_manager->height);

    game_config->load_res(0, true);

    text->init(N_TYPE_FONTS);
    text->init_index (0, RES_TERMINUS_TTF, 48);
    text->init_shaders();
    text->build_text();
}

#ifdef __ANDROID__
void android_respawn_gl_context ()
{
    if (transport_is_stop()) {
        if (ctx) {

            game_config->unload_res ();

            text->uninit(N_TYPE_FONTS);
            game_config->cur_level->unload_assets();

            SDL_GL_DeleteContext(ctx);

            ctx = nullptr;

            return;
        }
    }

    if (transport_is_pause()) {
        is_resume = true;
    }

    if (is_resume && transport_is_new_context()) {

        ctx = SDL_GL_CreateContext (window);
        SDL_GL_MakeCurrent (window, ctx);

        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport (0, 0, screen_manager->width, screen_manager->height);

        game_config->load_res(0, true);

        text->init(N_TYPE_FONTS);
        text->init_index (0, RES_TERMINUS_TTF, 48);
        text->build_text();
        text->init_shaders();

        game_config->cur_level->load_assets();

        is_resume = false;
    }
}
#endif

int
main(int argc, char **argv)
{
#ifdef __ANDROID__
	//global_env = static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv());
#endif

	std::setlocale (LC_ALL, "en_US.UTF-8");

	SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	//SDL_GL_LoadLibrary (nullptr);

	SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

#ifdef __ANDROID__
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

	screen_manager = ScreenManager::get_instance ();

	game_config = GameConfig::get_instance ();
	game_config->init_screen ();

	uint32_t flags_window = SDL_WINDOW_OPENGL | (screen_manager->is_fullscreen ? SDL_WINDOW_FULLSCREEN: 0);

	window = SDL_CreateWindow ("HexFrontier",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_manager->width,
			screen_manager->height,
			flags_window
			);


    shader_manager = ShaderManager::get_instance ();

    text = Text::getInstance ();

#ifdef __ANDROID__
	int ret = 1;
    	android_respawn_gl_context();
#else
    	ctx = SDL_GL_CreateContext (window);
	int ret = gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_MakeCurrent (window, ctx);
    	pc_respawn_gl_context();
#endif

	game_config->list_of_levels ();
	game_config->entry_point ();

#ifndef __ANDROID__
	game_config->cur_level->load_assets ();
#endif

	SDL_CreateThread (event_linux, "thread", game_config);

#ifdef WIN32
	SDL_Event event;
	while (1) {
		SDL_PollEvent(&event);
		handle_event(event);
#else
	while (1) {
#endif
#ifdef __ANDROID__
        android_respawn_gl_context();
#endif

		game_config->cur_level->clear_screen ();

		game_config->cur_level->tick ();

		game_config->cur_level->render ();

		SDL_GL_SwapWindow (window);

		SDL_Delay (16);
	}

	return 0;
}
