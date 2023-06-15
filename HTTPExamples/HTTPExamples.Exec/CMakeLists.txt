set(TARGET HTTPExamples.Exec)
set(OUTPUT_NAME ${PROJECT}.HTTPExamples.Exec)

set(SOURCE_FILES
    main.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} 
                                        Boost::boost)

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:Beast.Static,SOURCE_DIR>/..)
target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:HTTPExamples.Static,SOURCE_DIR>/..)


install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin/)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin/)
