file(GLOB_RECURSE SOURCES "Source/*.cpp" "Include/*.hpp" "../Tudo/Core/Source/FileSystem.cpp")

if (WIN32)
    add_executable (SHDC ${SOURCES})
	set_target_properties(SHDC PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../GameDir)
else()
    add_executable(SHDC MACROS ${SOURCES})
endif()

# Thirdparty
target_link_libraries(SHDC PRIVATE bgfx::bgfx)
target_link_libraries(SHDC PRIVATE bx::bx)
target_link_libraries(SHDC PRIVATE bimg::bimg)

target_include_directories(SHDC PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tudo/Core/Include> $<INSTALL_INTERFACE:Core>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../Tudo/Math/Include> $<INSTALL_INTERFACE:Math>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include> $<INSTALL_INTERFACE:TInc>
)

if (WIN32)
	set_target_properties(SHDC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/x64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/x64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/x64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/x64/MinSizeRel
	)
	if(MSVC)
		target_compile_options(SHDC PRIVATE /Zc:__cplusplus /Zc:preprocessor)
	endif()
else()
	set_target_properties(SHDC PROPERTIES
   	 	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/arm64/Debug
    		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/arm64/Release
    		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/arm64/RelWithDebInfo
    		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_SOURCE_DIR}/arm64/MinSizeRel
	)

	find_library(COREFRAMEWORK CoreFoundation)
	find_library(IOKIT_FRAMEWORK IOKit)
	target_link_libraries(SHDC PRIVATE ${COREFRAMEWORK} ${IOKIT_FRAMEWORK})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SHDC PROPERTY CXX_STANDARD 20)
endif()
