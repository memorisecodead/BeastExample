set(TARGET HTTPServer.Static)
set(OUTPUT_NAME HTTPServer.Static)

set(SOURCE_FILES
	Session.hpp
	Listener.hpp
	HTTPServer.hpp
	HTTPServerInfo.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										Boost::boost)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})


target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast.Static,SOURCE_DIR>/..)
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Utility.Public,SOURCE_DIR>/..)
									 