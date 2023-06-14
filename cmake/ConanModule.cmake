# This is a CMake file that configures Conan and external dependencies
# This is the place to add any additional external packages in the future.


# Download conan.cmake file for future use
# https://github.com/conan-io/cmake-conan is the go-to website if you have questions.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.0/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

# Includes the downloaded file
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Uses conan.cmake functionality to list the needed packages.
conan_cmake_configure(REQUIRES boost/1.81.0
                    GENERATORS cmake_find_package_multi)

# This detects system settings such as platform, compiler, etc.
# Those can be overridden, see conan.cmake github for documentation.
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    REMOTE conancenter
                    BUILD missing
                    INSTALL_FOLDER ${CMAKE_BINARY_DIR}/Conan
                    SETTINGS ${settings}
                    ENV "F77=no"
                    ENV "FCs=no")

# Append the folder with
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Conan)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# List of find_packages for each external library that we use.

find_package(Boost REQUIRED)
