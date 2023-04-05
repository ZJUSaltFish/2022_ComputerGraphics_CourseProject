cmake_minimum_required(VERSION 3.15)

project(finalProject)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/projects)
set(THIRD_PARTY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/external)

#copy media files
#file(COPY "media/" DESTINATION "media")

#compile libs to subdirctories
add_subdirectory(${THIRD_PARTY_LIBRARY_PATH}/glfw)
add_subdirectory(${THIRD_PARTY_LIBRARY_PATH}/glad)
add_subdirectory(${THIRD_PARTY_LIBRARY_PATH}/glm)
add_subdirectory(${THIRD_PARTY_LIBRARY_PATH}/imgui)
add_subdirectory(${THIRD_PARTY_LIBRARY_PATH}/stb)

#get our source codes
file(GLOB BASE_HDR ${SOURCE_PATH}/base/*.h)
file(GLOB BASE_SRC ${SOURCE_PATH}/base/*.cpp)
file(GLOB PROJECT_HDR ${SOURCE_PATH}/finalProject/*.h)
file(GLOB PROJECT_SRC ${SOURCE_PATH}/finalProject/*.cpp)

#generate .exe
add_executable(finalProject ${BASE_HDR} ${BASE_SRC} ${PROJECT_HDR} ${PROJECT_SRC})

if(WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif(UNIX)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release")
endif()
endif()

#include libs
target_include_directories(
	finalProject PUBLIC
	${THIRD_PARTY_LIBRARY_PATH}/glfw/include
	${THIRD_PARTY_LIBRARY_PATH}/glad/include
	${THIRD_PARTY_LIBRARY_PATH}/glm
	${THIRD_PARTY_LIBRARY_PATH}/imgui
	${THIRD_PARTY_LIBRARY_PATH}/stb
)

#link libs
target_link_libraries(finalProject PUBLIC glfw glad glm imgui stb)