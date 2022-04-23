cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "./externals/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_BUILD_TYPE Debug)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(SimpleGameEngine)

find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(EABase CONFIG REQUIRED)
find_package(EASTL CONFIG REQUIRED)

function(sge_source_group src_path src_files)
	foreach(FILE ${src_files}) 
		get_filename_component(PARENT_DIR "${FILE}" PATH)
		file(RELATIVE_PATH PARENT_DIR ${src_path} ${PARENT_DIR})
		string(REPLACE "/" "\\" GROUP "${PARENT_DIR}")
		set(GROUP "${GROUP}")
		source_group("${GROUP}" FILES "${FILE}")	
	endforeach()
endfunction()

function(sge_set_target_warning_level target_name)
	if(MSVC)
	  target_compile_options(${target_name} PRIVATE /W4 /WX)
	  target_compile_options(${target_name} PRIVATE /wd4100) #warning C4100: unreferenced formal parameter in function
	  target_compile_options(${target_name} PRIVATE /wd4201) #warning C4201: nonstandard extension used: nameless struct/union
	  target_compile_options(${target_name} PRIVATE /wd4127) #warning C4127: conditional expression is constant
	else()
	  target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	endif()
endfunction()

function(sge_add_library target_name src_path)
	file(GLOB_RECURSE src_files
		"${src_path}/src/*.*"
	)	
	sge_source_group(${src_path} "${src_files}")
	
	add_library(${target_name} ${src_files})	
	target_precompile_headers(${target_name} PUBLIC src/${target_name}/${target_name}-pch.h)
	target_include_directories(${target_name} PUBLIC src)
	sge_set_target_warning_level(${target_name})
endfunction()

function(sge_add_executable target_name src_path)
	file(GLOB_RECURSE src_files
		"${src_path}/src/*.*"
	)	
	sge_source_group(${src_path} "${src_files}")
	
	add_executable(${target_name} ${src_files})	
	target_precompile_headers(${target_name} INTERFACE src/${target_name}/${target_name}-pch.h)
	sge_set_target_warning_level(${target_name})
endfunction()

#-- core ---
add_subdirectory(src/core)
add_subdirectory(src/core/test)

#-- render --
add_subdirectory(src/render)

#-- engine
add_subdirectory(src/engine)

#-- editor
add_subdirectory(src/editor)
add_subdirectory(src/editor/app)

set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT sge_editor_app)