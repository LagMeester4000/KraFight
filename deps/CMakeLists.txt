cmake_minimum_required(VERSION 3.6.0)
set(BUILD_TESTING OFF)

# SFML
set(SFML_STATIC_LIBRARIES ON)
add_subdirectory(SFML)
#set_property(TARGET SFML PROPERTY FOLDER SFML)