include (FindPkgConfig)

pkg_check_modules (GLES glesv2)
pkg_check_modules (SDL sdl2)
pkg_check_modules (FREETYPE freetype2)

set (SOURCES_ENGINE
	Engine/Core/Main.cpp
	Engine/Core/ShaderManager.cpp
	Engine/Core/ScreenManager.cpp
	Engine/Utils/File.cpp
	Engine/Core/UI.cpp
	Engine/Core/Object.cpp
	Engine/UI/Rect.cpp
	)

set (SOURCES_GAME
	Game/Core/GameConfig.cpp
	Game/Shaders/ShaderRect.cpp
	)

add_executable (${PROJECT_NAME} ${SOURCES_GAME} ${SOURCES_ENGINE})

target_include_directories (${PROJECT_NAME}
	PRIVATE
	Include
	${GLES_INCLUDE_DIRS}
	${SDL_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIRS}
	)

target_link_libraries (${PROJECT_NAME}
	${GLES_LIBRARIES}
	${SDL_LIBRARIES}
	${FREETYPE_LIBRARIES}
	"-lm"
	)

set (EXECUTABLE_OUTPUT_PATH ..)
