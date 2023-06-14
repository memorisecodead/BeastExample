project(HTTPClient.Core LANGUAGES CXX)

set(TARGET HTTPClient.Core)
set(FOLDER HTTPClient.Core)

add_subdirectory(HTTPClient.Exec)
add_subdirectory(HTTPClient.Static)

add_library(${TARGET} INTERFACE)
