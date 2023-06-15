set(TARGET HTTPExamples.Static)
set(OUTPUT_NAME HTTPExamples.Static)

set(SOURCE_FILES
	HTTPExamples.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										Boost::boost)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})


target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast.Static,SOURCE_DIR>/..)
									 