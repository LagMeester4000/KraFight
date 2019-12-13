include_directories(include)
include_directories(../deps/SFML/include)

file(GLOB_RECURSE KraNet_SRC
    "include/*.h"
    "source/*.c"
    "include/*.hpp"
    "source/*.cpp"
)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

add_library(
    KraNet STATIC
    ${KraNet_SRC}
)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

#find_package(SFML 2.5 COMPONENTS network REQUIRED)
target_link_libraries(KraNet sfml-network)