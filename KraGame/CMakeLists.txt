include_directories(include)
include_directories(../KraFight/include)
include_directories(../KraNet/include)
include_directories(include/imgui)
include_directories(imgui)
#include_directories(../deps/SFML/include)

file(GLOB_RECURSE KraGame_SRC
    "include/*.h"
    "source/*.c"
    "include/*.hpp"
    "source/*.cpp"
)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

add_executable(
    KraGame
    ${KraGame_SRC}
)

target_link_libraries(KraGame KraFight)
target_link_libraries(KraGame KraNet)
target_link_libraries(KraGame opengl32)
#target_link_libraries(KraGame winmm)
#target_link_libraries(KraGame gdi32)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

#find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(KraGame sfml-graphics)