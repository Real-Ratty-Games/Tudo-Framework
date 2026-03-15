file(GLOB_RECURSE SOURCES "Core/Source/*.cpp" "Core/Include/*.hpp"
"Draw/Source/*.cpp" "Draw/Include/*.hpp"
"Math/Source/*.cpp" "Math/Include/*.hpp"
"Net/Source/*.cpp" "Net/Include/*.hpp"
"OS/Source/*.cpp" "OS/Include/*.hpp"
"Res/Source/*.cpp" "Res/Include/*.hpp")

add_library(Tudo STATIC ${SOURCES})

file(GLOB COREFILES_HEADER "Core/Include/*.hpp")
source_group("Core/Inc/" FILES ${COREFILES_HEADER})

file(GLOB COREFILES_SOURCE "Core/Source/*.cpp")
source_group("Core/Src" FILES ${COREFILES_SOURCE})

file(GLOB DRAWFILES_HEADER "Draw/Include/*.hpp")
source_group("Draw/Inc/" FILES ${DRAWFILES_HEADER})

file(GLOB DRAWFILES_SOURCE "Draw/Source/*.cpp")
source_group("Draw/Src/" FILES ${DRAWFILES_SOURCE})

file(GLOB MATHFILES_HEADER "Math/Include/*.hpp")
source_group("Math/Inc/" FILES ${MATHFILES_HEADER})

file(GLOB MATHFILES_SOURCE "Math/Source/*.cpp")
source_group("Math/Src/" FILES ${MATHFILES_SOURCE})

file(GLOB NETFILES_HEADER "Net/Include/*.hpp")
source_group("Net/Inc/" FILES ${NETFILES_HEADER})

file(GLOB NETFILES_SOURCE "Net/Source/*.cpp")
source_group("Net/Src/" FILES ${NETFILES_SOURCE})

file(GLOB OSFILES_HEADER "OS/Include/*.hpp")
source_group("OS/Inc/" FILES ${OSFILES_HEADER})

file(GLOB OSFILES_SOURCE "OS/Source/*.cpp")
source_group("OS/Src/" FILES ${OSFILES_SOURCE})

file(GLOB RESFILES_HEADER "Res/Include/*.hpp")
source_group("Res/Inc/" FILES ${RESFILES_HEADER})

file(GLOB RESFILES_SOURCE "Res/Source/*.cpp")
source_group("Res/Src/" FILES ${RESFILES_SOURCE})

target_include_directories(Tudo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core/Include> $<INSTALL_INTERFACE:Core>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Draw/Include> $<INSTALL_INTERFACE:Draw>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Math/Include> $<INSTALL_INTERFACE:Math>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Net/Include> $<INSTALL_INTERFACE:Net>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/OS/Include> $<INSTALL_INTERFACE:OS>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Res/Include> $<INSTALL_INTERFACE:Res>
)

# Thirdparty
target_link_libraries(Tudo PRIVATE SoLoud::SoLoud)
target_link_libraries(Tudo PRIVATE SDL3::SDL3)
target_link_libraries(Tudo PRIVATE bgfx::bgfx)
target_link_libraries(Tudo PRIVATE bx::bx)
target_link_libraries(Tudo PRIVATE bimg::bimg)

if (WIN32)
	set_target_properties(Tudo PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)

	if(MSVC)
		target_compile_options(Tudo PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
elseif(APPLE)
	set_target_properties(Tudo PROPERTIES
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
	target_link_libraries(Tudo PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK} ${METAL_FRAMEWORK} ${COCOA_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} ${COREAUDIO_FRAMEWORK} ${AUDIOTOOLBOX_FRAMEWORK})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tudo PROPERTY CXX_STANDARD 20)
endif()
