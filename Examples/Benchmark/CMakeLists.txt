set(TARGET Benchmark)
set(OUTPUT_NAME Benchmark)

set(SOURCE_FILES
	main.cpp
	PocoBench.hpp
	CurlBench.hpp
	BeastBench.hpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										Boost::boost
										benchmark::benchmark
										CURL::libcurl
										Poco::Foundation Poco::Net Poco::Util)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})
																
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast,SOURCE_DIR>/..)
