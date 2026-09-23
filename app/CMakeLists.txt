# ============================================================================
# app — Dear ImGui desktop dashboard (GLFW + OpenGL 3)
# ----------------------------------------------------------------------------
# Dear ImGui is fetched from GitHub at configure time. GLFW is taken from the
# system when installed (apt: libglfw3-dev, brew: glfw, vcpkg: glfw3),
# otherwise fetched and built from source.
# ============================================================================
include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.9b
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(imgui)

find_package(glfw3 3.3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found on the system; fetching and building it")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(glfw)
endif()

find_package(OpenGL REQUIRED)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}/misc/cpp
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_executable(pokertab WIN32 main.cpp)
target_link_libraries(pokertab PRIVATE pokertab_core imgui)
if(MSVC)
    # WIN32 above hides the console window; keep main() as the entry point.
    set_target_properties(pokertab PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif(WIN32)
    target_link_options(pokertab PRIVATE -mwindows)
endif()
