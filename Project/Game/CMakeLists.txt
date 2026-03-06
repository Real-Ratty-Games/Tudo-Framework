file(GLOB_RECURSE SOURCES "Source/*.cpp" "Include/*.hpp")

if (WIN32)
    add_executable (Game ${SOURCES})
	set_target_properties(Game PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../GameDir)
else()
    add_executable(Game MACOSX_BUNDLE ${SOURCES})
endif()

# Thirdparty
target_link_libraries(Game PRIVATE SoLoud::SoLoud)
target_link_libraries(Game PRIVATE SDL3::SDL3)
target_link_libraries(Game PRIVATE bgfx::bgfx)
target_link_libraries(Game PRIVATE bx::bx)
target_link_libraries(Game PRIVATE bimg::bimg)

if(APPLE)
    set(GAME_APP "$<TARGET_BUNDLE_DIR:Game>")
    set(GAME_FRAMEWORKS_DIR "${GAME_APP}/Contents/Frameworks")

    set_target_properties(Game PROPERTIES
        BUILD_RPATH "@executable_path/../Frameworks"
        INSTALL_RPATH "@executable_path/../Frameworks"
    )

    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GAME_FRAMEWORKS_DIR}"
    )
	
	add_custom_command(TARGET Game POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Debug,${CMAKE_SOURCE_DIR}/Thirdparty/arm64/Release>"
			"${GAME_FRAMEWORKS_DIR}"
		COMMAND ${CMAKE_COMMAND} -E remove
			"${GAME_FRAMEWORKS_DIR}/*.a")

    add_custom_command(TARGET Game POST_BUILD
        COMMAND codesign --force --deep --sign - "${GAME_APP}"
    )
endif()

target_link_libraries(Game PRIVATE GameEngine)
if (WIN32)
	target_link_directories(Game PRIVATE
			$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/../x64/Debug>
			$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/../x64/Release>
	)
elseif(APPLE)
	target_link_directories(Game PRIVATE
			$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/../arm64/Debug>
			$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/../arm64/Release>
	)
endif()
target_include_directories(Game PRIVATE "../GameEngine/Include")

if (WIN32)
	set_target_properties(Game PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)
	if(MSVC)
		target_compile_options(Game PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
else()
	set_target_properties(Game PROPERTIES
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
	target_link_libraries(Game PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK} ${METAL_FRAMEWORK} ${COCOA_FRAMEWORK} ${FOUNDATION_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} ${COREAUDIO_FRAMEWORK} ${AUDIOTOOLBOX_FRAMEWORK})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Game PROPERTY CXX_STANDARD 20)
endif()
