project(HTTPExamples LANGUAGES CXX)

set(TARGET HTTPExamples)
set(FOLDER HTTPExamples)

add_subdirectory(Examples.Exec)
add_subdirectory(Examples.Static)

add_library(${TARGET} INTERFACE)
