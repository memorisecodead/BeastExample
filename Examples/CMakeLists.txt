project(Examples LANGUAGES CXX)

set(TARGET Examples)
set(FOLDER Examples)

add_subdirectory(ASIO)
add_subdirectory(HTTPRequests)
add_subdirectory(REST)
add_subdirectory(HTTPServer)
add_subdirectory(HTTPClient)

add_library(${TARGET} INTERFACE)
