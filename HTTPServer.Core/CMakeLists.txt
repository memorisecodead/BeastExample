project(HTTPServer.Core LANGUAGES CXX)

set(TARGET HTTPServer.Core)
set(FOLDER HTTPServer.Core)

add_subdirectory(HTTPServer.Exec)
add_subdirectory(HTTPServer.Static)

add_library(${TARGET} INTERFACE)
