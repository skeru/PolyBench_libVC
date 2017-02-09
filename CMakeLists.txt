cmake_minimum_required(VERSION 3.0.2)

set (EXE_NAME "runPolyBenchSuite")

# Load additional CMake modules
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(LibVersioningCompiler REQUIRED)
include_directories(${LIBVC_INCLUDES})
link_directories(${LIBVC_LIB_DIR})
set (DEPS_LIBS ${DEPS_LIBS} ${LIBVC_LIBRARIES})

set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")

if (HAVE_CLANG_LIB)
	set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-DHAVE_CLANG_LIB")
endif(HAVE_CLANG_LIB)

set (source_files
	"tester/main.cpp"
	)

add_executable(${EXE_NAME} ${source_files})
target_link_libraries(${EXE_NAME} ${DEPS_LIBS})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "default install path" FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
