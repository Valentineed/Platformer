#APP
cmake_minimum_required(VERSION 3.16.3)


include(CMake/CMakeLists_app.cmake)
set(APP_TARGET app)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(IMGUI_SOURCE ${VENDOR_DIR}/imgui)

file(GLOB IMGUI_FILES "${IMGUI_SOURCE}/*.cpp" "${IMGUI_SOURCE}/*.h")
add_executable(${APP_TARGET} ${app_SOURCES} ${IMGUI_FILES})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${app_SOURCES})

if(MSVC)
target_compile_options(${APP_TARGET} PRIVATE /W4 /WX)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT output)
if(ipo_supported)
    message("--APP: IPO is supported!")
    set_property(TARGET ${APP_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()


set(APP_DIR ${PROJECT_SOURCE_DIR}/app/source)
set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(GLFW3_ROOT ${VENDOR_DIR}/glfw3/)
set(GLFW_DLL ${PROJECT_SOURCE_DIR}/vendor/glfw3/lib/glfw3.dll)
set(IMGUI_SOURCE ${VENDOR_DIR}/imgui)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake/")
find_package(GLFW3 REQUIRED)
							
target_include_directories(${APP_TARGET}
							PRIVATE 
							${IMGUI_SOURCE}
							${APP_DIR}
							${VENDOR_DIR}/glfw3/include
							${GLFW3_INCLUDE_DIR}
							)
							
target_link_libraries(${APP_TARGET} PRIVATE ${GLFW3_LIBRARY} lib physicsLib)

#copie the dll and deplace
add_custom_command(TARGET ${APP_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_DLL} $<TARGET_FILE_DIR:${APP_TARGET}>)


#include auto-source regeneration script for this target if there if there is one
set(AUTO_REGEN_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/_scripts/generate_cmake_app.sh")
if(EXISTS ${AUTO_REGEN_SCRIPT})
target_sources(${APP_TARGET} PRIVATE ${AUTO_REGEN_SCRIPT})
endif()


#start the project in my target
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_TARGET})

set_property(TARGET ${APP_TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")