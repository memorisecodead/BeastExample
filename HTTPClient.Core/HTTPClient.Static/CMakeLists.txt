set(TARGET HTTPClient.Static)
set(OUTPUT_NAME HTTPClient.Static)

set(SOURCE_FILES
	HTTPClient.hpp
	HTTPClientInfo.hpp
	Session.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										Boost::boost)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast.Static,SOURCE_DIR>/..)
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Utility.Public,SOURCE_DIR>/..)
