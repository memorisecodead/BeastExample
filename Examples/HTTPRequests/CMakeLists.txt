set(TARGET HTTPExample)
set(OUTPUT_NAME HTTPExample)

set(SOURCE_FILES
	HTTPExample.hpp
	main.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										Boost::boost)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})
								
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast,SOURCE_DIR>/..)
									 