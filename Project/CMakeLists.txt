cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

project ("CppGameKit")

if (WIN32)
	execute_process(COMMAND py build.py
   	 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
elseif(APPLE)
	execute_process(COMMAND python3 build.py
  	  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()

add_library(SoLoud::SoLoud STATIC IMPORTED)
if (WIN32)
    set_target_properties(SoLoud::SoLoud PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/soloud_static_x64_d.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/soloud_static_x64.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(SoLoud::SoLoud PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug/libsoloud_static.a"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release/libsoloud_static.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_library(SDL3::SDL3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(SDL3::SDL3 PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/SDL3.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/SDL3.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(SDL3::SDL3 PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug/libSDL3.0.dylib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release/libSDL3.0.dylib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_library(Assimp::Assimp STATIC IMPORTED)
if (WIN32)
    set_target_properties(Assimp::Assimp PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/assimp-vc143-mtd.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/assimp-vc143-mt.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(Assimp::Assimp PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug_m3dc/libassimpd.6.0.4.dylib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release_m3dc/libassimp.6.0.4.dylib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_library(bgfx::bgfx STATIC IMPORTED)
if (WIN32)
    set_target_properties(bgfx::bgfx PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/bgfxDebug.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/bgfxRelease.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(bgfx::bgfx PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug/libbgfxDebug.a"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release/libbgfxRelease.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_library(bx::bx STATIC IMPORTED)
if (WIN32)
    set_target_properties(bx::bx PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/bxDebug.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/bxRelease.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(bx::bx PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug/libbxDebug.a"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release/libbxRelease.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_library(bimg::bimg STATIC IMPORTED)
if (WIN32)
    set_target_properties(bimg::bimg PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Debug/bimgDebug.lib"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/x64/Release/bimgRelease.lib"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc/"
	)
else()
	set_target_properties(bimg::bimg PROPERTIES
		IMPORTED_LOCATION_DEBUG   "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug/libbimgDebug.a"
		IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release/libbimgRelease.a"
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Thirdparty/Inc"
	)
endif()

add_subdirectory ("GameEngine")
add_subdirectory ("Game")
add_subdirectory ("M3DC")
