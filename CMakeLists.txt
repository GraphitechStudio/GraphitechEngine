cmake_minimum_required(VERSION 3.15)
project(GraphitechEngine VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "Configuring Graphitech Engine...")

# Configure CPM cache (if not already set by parent)
if(NOT DEFINED CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm-cache" CACHE PATH "CPM source cache directory")
endif()
set(CPM_USE_LOCAL_PACKAGES ON)

# Bootstrap CPM for dependencies
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
    message(STATUS "Downloading CPM.cmake")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/cmake")
    file(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake"
        "${CMAKE_BINARY_DIR}/cmake/CPM.cmake"
        TLS_VERIFY ON
        SHOW_PROGRESS
    )
endif()
include(${CMAKE_BINARY_DIR}/cmake/CPM.cmake)

# Dependencies
# GLM is added by parent Graphitech/CMakeLists.txt

CPMAddPackage(
    NAME stb
    GITHUB_REPOSITORY nothings/stb
    GIT_TAG master
)

# GLAD generation
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    CPMAddPackage(
        NAME glad
        GITHUB_REPOSITORY Dav1dde/glad
        GIT_TAG v2.0.8
    )

    if(glad_ADDED)
        set(GLAD_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/glad_generated" CACHE PATH "GLAD generated files directory" FORCE)
        set(GLAD_HEADER "${GLAD_OUT_DIR}/include/glad/glad.h")
        set(GLAD_SOURCE "${GLAD_OUT_DIR}/src/gl.c")
        
        # Only generate GLAD if files don't already exist
        if(NOT EXISTS "${GLAD_HEADER}" OR NOT EXISTS "${GLAD_SOURCE}")
            message(STATUS "Generating GLAD files...")
            file(MAKE_DIRECTORY ${GLAD_OUT_DIR})
            execute_process(
                COMMAND ${Python3_EXECUTABLE} -m glad
                    --api=gl:compatibility=3.3
                    --reproducible
                    --out-path=${GLAD_OUT_DIR}
                    c
                    --loader
                WORKING_DIRECTORY ${glad_SOURCE_DIR}
                RESULT_VARIABLE GLAD_RESULT
            )

            if(GLAD_RESULT EQUAL 0)
                file(COPY_FILE "${GLAD_OUT_DIR}/include/glad/gl.h" "${GLAD_HEADER}")
            else()
                message(WARNING "GLAD generation failed - OpenGL modern functions may not be available")
            endif()
        else()
            message(STATUS "Using cached GLAD files")
        endif()
        
        if(EXISTS "${GLAD_SOURCE}")
            add_library(glad_lib STATIC)
            target_sources(glad_lib PRIVATE "${GLAD_SOURCE}")
            target_include_directories(glad_lib PUBLIC "${GLAD_OUT_DIR}/include")
            set(GLAD_LIBRARIES glad_lib)
        endif()
    endif()
else()
    message(WARNING "Python3 not found - GLAD cannot be generated")
endif()

# Collect all engine source files
file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Network/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Render/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Application/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/*.cpp
)

# Create the Graphitech Engine library
add_library(GraphitechEngine STATIC ${ENGINE_SOURCES})

# Link dependencies
# GLFW via CPM instead of PkgConfig
CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.3.8
    OPTIONS "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_DOCS OFF" "GLFW_INSTALL ON"
)

if(glfw_ADDED)
    target_link_libraries(GraphitechEngine PUBLIC glfw)
    # GLFW adds include directories automatically
else()
    # Fallback to system if CPM failed or was skipped
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_include_directories(GraphitechEngine PUBLIC ${GLFW_INCLUDE_DIRS})
    target_link_libraries(GraphitechEngine PUBLIC ${GLFW_LIBRARIES})
endif()

target_include_directories(GraphitechEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Graphitech/Graphics/OpenGL
)

if(TARGET glm)
    target_link_libraries(GraphitechEngine PUBLIC glm)
    message(STATUS "Using GLM from CPM")
else()
    message(FATAL_ERROR "GLM target not available - cannot continue")
endif()

if(TARGET glad_lib)
    target_include_directories(GraphitechEngine PUBLIC "${GLAD_OUT_DIR}/include")
    target_link_libraries(GraphitechEngine PUBLIC glad_lib)
endif()

if(TARGET glm)
    target_link_libraries(GraphitechEngine PUBLIC glm)
endif()

if(stb_ADDED AND DEFINED stb_SOURCE_DIR)
    target_include_directories(GraphitechEngine PUBLIC ${stb_SOURCE_DIR})
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(GraphitechEngine PUBLIC
    ${GLFW_LIBRARIES}
    OpenGL::GL
    ${CMAKE_DL_LIBS}
    pthread
)

# Expose ECS interfaces to the engine
if(TARGET GraphitechECS)
    target_link_libraries(GraphitechEngine PUBLIC GraphitechECS)
endif()

# If a Graphitech graphics backend target exists, link it to the engine
if(TARGET GraphitechGraphicsOpenGL)
    target_link_libraries(GraphitechEngine PUBLIC GraphitechGraphicsOpenGL)
endif()

# Compiler flags
target_compile_options(GraphitechEngine PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Release>:-O3>
)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ECS and Shared have been migrated under Graphitech/
# They are added by the Graphitech meta CMake and should not be added here to avoid duplicate targets.
# add_subdirectory(ECS)
# add_subdirectory(Shared)

# Installation rules (optional)
install(TARGETS GraphitechEngine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/Graphitech
    FILES_MATCHING PATTERN "*.hpp"
)

message(STATUS "Graphitech Engine configured successfully")
