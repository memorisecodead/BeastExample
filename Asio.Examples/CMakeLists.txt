project(Asio.Examples LANGUAGES CXX)

set(TARGET Asio.Examples)
set(FOLDER Asio.Examples)

add_subdirectory(Asio.Examples.Exec)
add_subdirectory(Asio.Examples.Static)

add_library(${TARGET} INTERFACE)
