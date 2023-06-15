project(HTTPExamples LANGUAGES CXX)

set(TARGET HTTPExamples)
set(FOLDER HTTPExamples)

add_subdirectory(HTTPExamples.Exec)
add_subdirectory(HTTPExamples.Static)

add_library(${TARGET} INTERFACE)
