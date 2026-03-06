file(GLOB_RECURSE SOURCES "Source/*.cpp" "../GameEngine/Source/FileSystem.cpp")

if (WIN32)
    add_executable (M3DC ${SOURCES})
	set_target_properties(M3DC PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../GameDir)
else()
    add_executable(M3DC MACOSX_BUNDLE ${SOURCES})
endif()

# Thirdparty
target_link_libraries(M3DC PRIVATE Assimp::Assimp)

if(APPLE)
    set(GAME_APP "$<TARGET_BUNDLE_DIR:M3DC>")
    set(GAME_FRAMEWORKS_DIR "${GAME_APP}/Contents/Frameworks")

    set_target_properties(M3DC PROPERTIES
        BUILD_RPATH "@executable_path/../Frameworks"
        INSTALL_RPATH "@executable_path/../Frameworks"
    )

    add_custom_command(TARGET M3DC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GAME_FRAMEWORKS_DIR}"
    )
	
	add_custom_command(TARGET M3DC POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug_m3dc,${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release_m3dc>"
			"${GAME_FRAMEWORKS_DIR}"
		COMMAND ${CMAKE_COMMAND} -E remove
			"${GAME_FRAMEWORKS_DIR}/*.a")

    add_custom_command(TARGET M3DC POST_BUILD
        COMMAND codesign --force --deep --sign - "${GAME_APP}"
    )
endif()

if (WIN32)
	set_target_properties(M3DC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)
	if(MSVC)
		target_compile_options(M3DC PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
else()
	set_target_properties(M3DC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/arm64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/arm64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/arm64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/arm64/MinSizeRel
	)

	find_library(COREFRAMEWORK CoreFoundation)
	find_library(IOKIT_FRAMEWORK IOKit)
	target_link_libraries(M3DC PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET M3DC PROPERTY CXX_STANDARD 20)
endif()
