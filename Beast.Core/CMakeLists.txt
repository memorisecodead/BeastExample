project(Beast.Core LANGUAGES CXX)

set(TARGET Beast.Core)
set(FOLDER Beast.Core)

add_subdirectory(Beast.Static)

add_library(${TARGET} INTERFACE)
