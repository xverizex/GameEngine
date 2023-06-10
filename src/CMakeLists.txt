
if (LINUX)
	include(FindPkgConfig)
	pkg_check_modules (SDL sdl2)
	pkg_check_modules (FREETYPE freetype2)
	pkg_check_modules(SDL sdl2)

	set(SGL_INCLUDES ${GL_INCLUDE_DIRS})
	set(SGL_LIBS ${GL_LIBRARIES})

	set(SSDL_INCLUDES ${SDL_INCLUDE_DIRS})
	set(SSDL_LIBS ${SDL_LIBRARIES})
endif()
if (MSVC)
	set(SGL_INCLUDES "")
	set(SGL_LIBS OpenGL32)

	set(SSDL_INCLUDES ../SDL2/)
	set(SSDL_LIBS 
		SDL2
		SDL2main
		)

	set(FREETYPE_LIBRARIES
		freetype)

	link_directories(..)
	set(SGLM_INCLUDES ../glm)
endif()

set (SOURCES_ENGINE
	Engine/Core/Main.cpp
	Engine/Core/ShaderManager.cpp
	Engine/Core/ScreenManager.cpp
	Engine/Utils/File.cpp
	Engine/Core/UI.cpp
	Engine/Core/Object.cpp
	Engine/UI/Rect.cpp
	Engine/Core/Downloader.cpp
	Engine/Core/Sprite.cpp
	Engine/Core/ILevel.cpp
	Engine/Core/Timer.cpp
	Engine/Core/Animation.cpp
	Engine/Core/Camera.cpp
	Engine/Core/Text.cpp
	Engine/Core/VertexData.cpp
	Engine/Core/IShader.cpp
	glad/glad.c
	)

set (SOURCES_GAME
	Game/Core/GameConfig.cpp
	Game/Shaders/ShaderRect.cpp
	Game/Shaders/Shader2DSprite.cpp
	Game/Shaders/Shader2DSpriteWithCamera.cpp
	)

add_executable (${PROJECT_NAME} ${SOURCES_GAME} ${SOURCES_ENGINE})

target_include_directories (${PROJECT_NAME}
	PRIVATE
	Include
	#${GLES_INCLUDE_DIRS}
	${SSDL_INCLUDES}
	${SGLM_INCLUDES}
	)

target_link_libraries (${PROJECT_NAME}
	#${GLES_LIBRARIES}
	${SSDL_LIBS}
	${FREETYPE_LIBRARIES}
	${SGL_LIBS}
	"-lm"
	)

set (EXECUTABLE_OUTPUT_PATH ..)
