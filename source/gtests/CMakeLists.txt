project(tests_prj)

set(TEST_SRC_PATH ${CMAKE_SOURCE_DIR}/gtests)
set(CLIENT_SRC_PATH ${CMAKE_SOURCE_DIR}/client)
set(SERVER_SRC_PATH ${CMAKE_SOURCE_DIR}/server)

include_directories(
	${CMAKE_BINARY_DIR}
	${COMMON_PATH}
	${CLIENT_SRC_PATH}
	${SERVER_SRC_PATH}
)

set(EXE_NAME tests)

file(GLOB CLIENT_FILES "${CLIENT_SRC_PATH}/*")
list(REMOVE_ITEM CLIENT_FILES "${CLIENT_SRC_PATH}/main.cpp" "${CLIENT_SRC_PATH}/main.h")

file(GLOB SERVER_FILES "${SERVER_SRC_PATH}/*")
list(REMOVE_ITEM SERVER_FILES "${SERVER_SRC_PATH}/main.cpp" "${SERVER_SRC_PATH}/main.h")

set(TEST_FILES
	tests_main.h
	tests_main.cpp
	client_tests.cpp
	server_tests.cpp
)

add_executable(${EXE_NAME}
	${TEST_FILES}
    ${CLIENT_FILES} 
	${SERVER_FILES}
	${COMMON_FILES}
)

add_dependencies(${EXE_NAME} generate_protobuf_files)

if(MSVC)
    set_property(DIRECTORY ${TEST_SRC_PATH} PROPERTY VS_STARTUP_PROJECT ${EXE_NAME})
	source_group("Main" FILES ${TEST_FILES})	
    source_group("Client" FILES ${CLIENT_FILES})
	source_group("Server" FILES ${SERVER_FILES}) 
	source_group("Common" FILES ${COMMON_FILES})  
endif ()

set_common_target_properties(${EXE_NAME})

add_custom_command(TARGET tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/client_files/files ${CMAKE_BINARY_DIR}/gtests/${CMAKE_CFG_INTDIR}/files )