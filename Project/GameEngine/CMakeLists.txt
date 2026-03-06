file(GLOB_RECURSE SOURCES "Source/*.cpp" "Include/*.hpp")

add_library(GameEngine STATIC ${SOURCES})

set(COREFILES_HEADER "Include/Alarm.hpp"
"Include/API.hpp"
"Include/BigError.hpp"
"Include/Clock.hpp"
"Include/DebugHelp.hpp"
"Include/FileSystem.hpp"
"Include/Globals.hpp"
"Include/IniFile.hpp"
"Include/Program.hpp"
"Include/SystemTypes.hpp"
"Include/WADFile.hpp"
"Include/WADMaker.hpp"
)
source_group("Core/Inc/" FILES ${COREFILES_HEADER})

set(COREFILES_SOURCE "Source/Alarm.cpp"
"Source/BigError.cpp"
"Source/Clock.cpp"
"Source/FileSystem.cpp"
"Source/IniFile.cpp"
"Source/Program.cpp"
"Source/WADFile.cpp"
"Source/WADMaker.cpp"
)
source_group("Core/Src/" FILES ${COREFILES_SOURCE})

set(DRAWFILES_HEADER "Include/DrawData.hpp"
"Include/DrawSurface.hpp"
"Include/DrawSurface2D.hpp"
"Include/DrawSurface3D.hpp"
"Include/Renderer.hpp"
"Include/Shader.hpp"
"Include/Sprite.hpp"
"Include/SpriteAnimation.hpp"
"Include/Viewport3D.hpp"
)
source_group("Draw/Inc/" FILES ${DRAWFILES_HEADER})

set(DRAWFILES_SOURCE "Source/DrawSurface.cpp"
"Source/DrawSurface2D.cpp"
"Source/DrawSurface3D.cpp"
"Source/Renderer.cpp"
"Source/Shader.cpp"
"Source/Sprite.cpp"
"Source/SpriteAnimation.cpp"
"Source/Viewport3D.cpp"
)
source_group("Draw/Src/" FILES ${DRAWFILES_SOURCE})

set(MATHFILES_HEADER "Include/Collider.hpp"
"Include/Collision.hpp"
"Include/DrawMath.hpp"
"Include/EulerRotation.hpp"
"Include/Math.hpp"
"Include/Matrix4.hpp"
"Include/Multiply.hpp"
"Include/Quaternion.hpp"
"Include/Transformation.hpp"
"Include/Vector2.hpp"
"Include/Vector3.hpp"
"Include/Vector4.hpp"
)
source_group("Math/Inc/" FILES ${MATHFILES_HEADER})

set(MATHFILES_SOURCE "Source/Collision.cpp"
"Source/DrawMath.cpp"
"Source/Transformation.cpp"
)
source_group("Math/Src/" FILES ${MATHFILES_SOURCE})

set(OSFILES_HEADER "Include/Gamepad.hpp"
"Include/Input.hpp"
"Include/InputTypes.hpp"
"Include/Sound.hpp"
"Include/Window.hpp"
)
source_group("OS/Inc/" FILES ${OSFILES_HEADER})

set(OSFILES_SOURCE "Source/Gamepad.cpp"
"Source/Input.cpp"
"Source/Sound.cpp"
"Source/Window.cpp"
)
source_group("OS/Src/" FILES ${OSFILES_SOURCE})

# Thirdparty
target_link_libraries(GameEngine PRIVATE SoLoud::SoLoud)
target_link_libraries(GameEngine PRIVATE SDL3::SDL3)
target_link_libraries(GameEngine PRIVATE bgfx::bgfx)
target_link_libraries(GameEngine PRIVATE bx::bx)
target_link_libraries(GameEngine PRIVATE bimg::bimg)

if (WIN32)
	set_target_properties(GameEngine PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)

	if(MSVC)
		target_compile_options(GameEngine PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
elseif(APPLE)
	set_target_properties(GameEngine PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/arm64/Debug
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/arm64/Release
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/arm64/RelWithDebInfo
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/arm64/MinSizeRel
	)

	find_library(COREAUDIO_FRAMEWORK CoreAudio)
	find_library(AUDIOTOOLBOX_FRAMEWORK AudioToolbox)

	find_library(METAL_FRAMEWORK Metal)
	find_library(COCOA_FRAMEWORK Cocoa)
	find_library(FOUNDATION_FRAMEWORK Foundation)
	find_library(QUARTZCORE_FRAMEWORK QuartzCore)

	find_library(COREFRAMEWORK CoreFoundation)
	find_library(IOKIT_FRAMEWORK IOKit)
	target_link_libraries(GameEngine PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK} ${METAL_FRAMEWORK} ${COCOA_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} ${COREAUDIO_FRAMEWORK} ${AUDIOTOOLBOX_FRAMEWORK})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)
endif()
