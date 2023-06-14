project(Utility LANGUAGES CXX)

set(TARGET Utility)
set(FOLDER Utility)

add_subdirectory(Public)

add_library(${TARGET} INTERFACE)
