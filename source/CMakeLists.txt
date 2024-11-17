cmake_minimum_required(VERSION 3.26)

project(general_prj)

set(COMMON_PATH ${CMAKE_SOURCE_DIR}/common)
option(BUILD_TESTS "Build tests using Google Test" OFF)

############################################# BOOST #############################################

if(${CMAKE_VERSION} VERSION_LESS "3.30")
    message(STATUS "CMake version is ${CMAKE_VERSION}. Used CMP0167 OLD")    
else()
    message(STATUS "CMake version >= 3.30. Used CMP0167 NEW. Need to set BOOST_ROOT and Boost_DIR")
    cmake_policy(SET CMP0167 NEW)
    cmake_policy(SET CMP0144 NEW)
    set (BOOST_ROOT "C:/Program Files/boost/boost_1_85_0")
    set (Boost_DIR "${BOOST_ROOT}/bin/x64/lib/cmake/Boost-1.85.0")
endif()

set(Boost_DEBUG OFF)

find_package (Boost 1.75.0 COMPONENTS 
    system    
    log
    log_setup
    program_options
    REQUIRED
) 

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

include_directories (SYSTEM ${Boost_INCLUDE_DIR})


############################################# PROTOBUF #############################################
set(PROTO_DIR "${COMMON_PATH}/proto")
set(PROTO_PATH "${PROTO_DIR}/messages.proto")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIR})

protobuf_generate_cpp(messages_SRCS messages_HDRS ${PROTO_PATH})

add_custom_command(
    OUTPUT ${messages_SRCS} ${messages_HDRS}
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
    ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR} -I ${PROTO_DIR} ${PROTO_PATH}
    DEPENDS ${PROTO_PATH}
)

add_custom_target(
    generate_protobuf_files ALL
    DEPENDS ${messages_SRCS} ${messages_HDRS}
)

# Export
set(PROTO_SRCS ${messages_SRCS}) #PARENT_SCOPE
set(PROTO_HDRS ${messages_HDRS})

############################################# SQLiteCpp ############################################

find_package(SQLiteCpp REQUIRED)

############################################# GTest ############################################

if(BUILD_TESTS)

    find_package(GTest)
    include_directories(${GTEST_INCLUDE_DIRS})
    
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

endif()


####################################################################################################

set(COMMON_FILES
    ${PROTO_SRCS} ${PROTO_HDRS} 
    ${COMMON_PATH}/game_elements.h
    ${COMMON_PATH}/avatar.h     ${COMMON_PATH}/avatar.cpp
    ${COMMON_PATH}/location.h   ${COMMON_PATH}/location.cpp     ${COMMON_PATH}/location.ipp
    ${COMMON_PATH}/npc.h        ${COMMON_PATH}/npc.cpp
    ${COMMON_PATH}/settings.h
)

if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00) # Windows_10 and Windows_11 
endif()

function(set_common_target_properties target)
    target_link_libraries(${target} PRIVATE
        ${Boost_LIBRARIES}
        Boost::log
        Boost::log_setup
        Boost::program_options
        protobuf::libprotobuf
        SQLiteCpp
    )

    if (BUILD_TESTS) 
        target_link_libraries(${target} PRIVATE 
            gtest gtest_main 
            gmock gmock_main 
        ) 
    endif()

    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4> #/WX 
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )    
endfunction()


add_subdirectory(client)
add_subdirectory(server)

if(BUILD_TESTS)
    add_subdirectory(gtests)
endif()