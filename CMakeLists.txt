cmake_minimum_required(VERSION 3.12)
project(2D-Physics-Engine)

# Inclure le module FetchContent
include(FetchContent)

# Configuration de SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.0
)

# Télécharger SFML si ce n'est pas déjà présent
FetchContent_GetProperties(SFML)
if(NOT SFML_POPULATED)
    FetchContent_Populate(SFML)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

# Définir les fichiers source et d'en-tête
set(SOURCE_FILES
    src/2D-Physics-Engine.cpp
    src/button.cpp
    src/functions.cpp
    src/physics.cpp
    src/scene.cpp
    src/struct.cpp
)

set(HEADER_FILES
    include/button.h
    include/functions.h
    include/physics.h
    include/scene.h
    include/struct.h
)

# Indiquer où trouver les fichiers d'en-tête
include_directories(include)

# Ajouter les fichiers source à l'exécutable
add_executable(2D-Physics-Engine ${SOURCE_FILES} ${HEADER_FILES})

# Copier les ressources vers le répertoire de sortie lors de la construction
configure_file(
    ${CMAKE_SOURCE_DIR}/resources/fonts/arial.ttf
    ${CMAKE_CURRENT_BINARY_DIR}/resources/fonts/arial.ttf
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/circle.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/circle.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/circleMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/circleMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/circleRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/circleRelease.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravity.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravity.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravityClick.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravityClick.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravityClickRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravityClickRelease.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravityMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravityMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravityMouseRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravityMouseRelease.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/gravityRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/gravityRelease.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/pause.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/pause.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/pauseClick.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/pauseClick.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/pauseMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/pauseMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/play.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/play.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/playClick.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/playClick.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/playMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/playMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/rectangle.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/rectangle.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/rectangleMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/rectangleMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/rectangleRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/rectangleRelease.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/refresh.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/refresh.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/refreshClick.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/refreshClick.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/refreshMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/refreshMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/textBox.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/textBox.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/textBoxClick.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/textBoxClick.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/textBoxMouse.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/textBoxMouse.png
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/sprites/textBoxRelease.png
    ${CMAKE_CURRENT_BINARY_DIR}/resources/sprites/textBoxRelease.png
    COPYONLY
)

# Liens avec les bibliothèques SFML
target_link_libraries(2D-Physics-Engine PRIVATE sfml-graphics sfml-window sfml-audio sfml-network sfml-system)