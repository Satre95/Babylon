set(ASSIMP_BUILD_STATIC_LIB ON)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)

add_subdirectory(glm)
add_subdirectory(assimp)
add_subdirectory(SDL2)
