find_package(Doxygen REQUIRED)

set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generation de la documentation Doxygen"
    VERBATIM
)