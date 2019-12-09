include_directories(include)

file(GLOB_RECURSE KraFight_SRC
    "include/*.h"
    "source/*.c"
    "include/*.hpp"
    "source/*.cpp"
)

add_library(
    KraFight STATIC
    ${KraFight_SRC}
)