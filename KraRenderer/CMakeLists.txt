include_directories(include)
include_directories(../KraFight/include)
include_directories(../KraNet/include)
include_directories(include/imgui)
include_directories(imgui)
#include_directories(../deps/SFML/include)

file(GLOB_RECURSE KraRenderer_SRC
    "include/*.h"
    "source/*.c"
    "include/*.hpp"
    "source/*.cpp"
)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

add_executable(
    KraRenderer
    ${KraRenderer_SRC}
)

target_link_libraries(KraRenderer KraFight)
target_link_libraries(KraRenderer KraNet)
target_link_libraries(KraRenderer opengl32)
#target_link_libraries(KraRenderer winmm)
#target_link_libraries(KraRenderer gdi32)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

#find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(KraRenderer sfml-graphics)