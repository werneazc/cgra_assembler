cmake_minimum_required(VERSION 2.6)
project(cgra_assembler)

#Bring the headers from project include folder and systemc include dir to project
include_directories(header "$./header")

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES LIST_DIRECTORIES FALSE "src/*.cpp")

#Add SystemC library to dependencies of project
find_library(PO boost_program_options)
find_library(FS boost_filesystem)
find_library(SY boost_system)
find_library(RE boost_regex)

find_package(Doxygen)
set(DOXFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/cgra-assembler)
set(DOXFILE ${CMAKE_CURRENT_BINARY_DIR}/cgra_assembler)
configure_file(${DOXFILE_IN} ${DOXFILE} @ONLY)

add_custom_target(
doc ALL
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXFILE_IN}
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
COMMENT "Generating doxygen documentation"
VERBATIM
)

add_executable(cgra_assembler ${SOURCES})
target_link_libraries(cgra_assembler ${PO} ${FS} ${SY} ${RE})

#install(TARGETS cgra_assembler RUNTIME DESTINATION bin)
