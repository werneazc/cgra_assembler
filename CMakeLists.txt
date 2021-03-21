cmake_minimum_required(VERSION 3.12)
project(cgra_assembler
    VERSION 0.1.0
    DESCRIPTION "Small Assembler for a VCGRA instance"
    LANGUAGES CXX
    )
    
#Find necessary boost packages
find_package(Boost 
    1.66.0
    REQUIRED
    COMPONENTS program_options filesystem regex
    )

#Create library for exceptions
add_library(myexceptions 
    INTERFACE)
target_include_directories(myexceptions
    INTERFACE
        header/
    )
target_compile_features(myexceptions
    INTERFACE
        cxx_std_11
    )
    
#Create library for parse objects
add_library(parseobjects 
    OBJECT
        src/parseobjbase.cpp src/parseobjectconst.cpp src/parseobjectvariable.cpp
        src/level.cpp src/loop.cpp
        src/iarithmetic.cpp src/addinteger.cpp src/add.cpp src/sub.cpp src/subinteger.cpp
        src/mul.cpp src/mulinteger.cpp
        src/nooperand.cpp src/oneoperand.cpp src/twooperand.cpp src/threeoperand.cpp
        src/resetvariable.cpp
    )
target_include_directories(parseobjects
    PUBLIC
        header/
    )
target_compile_features(parseobjects
    PUBLIC
        cxx_auto_type
        cxx_delegating_constructors
        cxx_deleted_functions
        cxx_final
        cxx_override
        cxx_nullptr
        cxx_rvalue_references
    )
target_link_libraries(parseobjects
    PUBLIC
        myexceptions)

#Create assembler library
add_library(assembler 
    OBJECT
    src/assembler.cpp
    )
target_include_directories(assembler
    PUBLIC
        header/
    )
target_compile_features(assembler
    PUBLIC
        cxx_auto_type
        cxx_delegating_constructors
        cxx_deleted_functions
        cxx_final
        cxx_override
        cxx_nullptr
        cxx_rvalue_references
    )
target_link_libraries(assembler
    PUBLIC
        parseobjects myexceptions
        Boost::regex
    )

#Create assembler executable
add_executable(cgra_assembler 
    src/main.cpp)
target_include_directories( cgra_assembler
    PUBLIC
        header/
    )
target_compile_features(cgra_assembler
    PUBLIC
        cxx_std_11
    )
target_link_libraries(cgra_assembler
    PUBLIC
        assembler parseobjects myexceptions
        Boost::program_options Boost::filesystem Boost::regex
    )


#Create documentation with doxygen
find_package(Doxygen REQUIRED dot)
set(DOXYGEN_CREATE_SUBDIRS YES)
set(DOXYGEN_MULTILINE_CPP_IS_BRIEF YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_EXTRACT_PRIVATE YES)
set(DOXYGEN_EXTRACT_PACKAGE YES)
set(DOXYGEN_EXTRACT_STATIC YES)
set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE Readme.md)
set(DOXYGEN_EXTRACT_LOCAL_CLASSES YES)
set(DOXYGEN_EXTRACT_LOCAL_METHODS YES)
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_INLINE_SOURCES YES)
set(DOXYGEN_UML_LOOK YES)
set(DOXYGEN_SHOW_FILES NO)
set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc/)
doxygen_add_docs(doc
    ${PROJECT_SOURCE_DIR}/src/ ${PROJECT_SOURCE_DIR}/header/ ${PROJECT_SOURCE_DIR}/doc/Readme.md
    COMMENT "Create doxygen documentation"
    )
#Type make doc in build directory to create documentation

#install(TARGETS cgra_assembler RUNTIME DESTINATION bin)
