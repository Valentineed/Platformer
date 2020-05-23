#PHYSICSLIB
cmake_minimum_required(VERSION 3.16.3)

include(CMake/CMakeLists_physicsLib.cmake)
set(PHYSICS_TARGET physicsLib)
 
add_library(${PHYSICS_TARGET} STATIC ${physicsLib_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${physicsLib_SOURCES})

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

# Load bullet library
set(BULLET_ROOT  ${VENDOR_DIR}/bullet)
include(FindBullet)
message(STATUS "Found bullet include dir: ${BULLET_INCLUDE_DIR}")
message(STATUS "Found bullet library 1 : ${BULLET_DYNAMICS_LIBRARY}")
message(STATUS "Found bullet library DEBUG 1 : ${BULLET_DYNAMICS_LIBRARY_DEBUG}")
message(STATUS "Found bullet library 2 : ${BULLET_COLLISION_LIBRARY}")
message(STATUS "Found bullet library DEBUG 2 : ${BULLET_COLLISION_LIBRARY_DEBUG}")
message(STATUS "Found bullet library 3 : ${BULLET_MATH_LIBRARY}")
message(STATUS "Found bullet library DEBUG 3 : ${BULLET_MATH_LIBRARY_DEBUG}")
message(STATUS "Found bullet library 4 : ${BULLET_SOFTBODY_LIBRARY}")
message(STATUS "Found bullet library DEBUG 4 : ${BULLET_SOFTBODY_LIBRARY_DEBUG}")

# Set Warning Level and Warning as error
if(MSVC)
	target_compile_options(${PHYSICS_TARGET} PRIVATE /W4 /WX)
endif()

# Interprocedural optimization in release mode
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT output)
if(ipo_supported)
    message("--LIBGFX: IPO is supported!")
    set_property(TARGET ${PHYSICS_TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
	message(WARNING "IPO is not supported: ${output}")
endif()

set(PHYSICS_DIR ${PROJECT_SOURCE_DIR}/PhysicsLib/source)

target_include_directories(${PHYSICS_TARGET} 	PUBLIC ${PHYSICS_DIR}
												PUBLIC ${BULLET_INCLUDE_DIR})

target_link_libraries(${PHYSICS_TARGET} PRIVATE optimized ${BULLET_DYNAMICS_LIBRARY}
												optimized ${BULLET_COLLISION_LIBRARY}
												optimized ${BULLET_MATH_LIBRARY}
												optimized ${BULLET_SOFTBODY_LIBRARY})

target_link_libraries(${PHYSICS_TARGET}	PRIVATE	debug ${BULLET_DYNAMICS_LIBRARY_DEBUG}
												debug ${BULLET_COLLISION_LIBRARY_DEBUG}
												debug ${BULLET_MATH_LIBRARY_DEBUG}
												debug ${BULLET_SOFTBODY_LIBRARY_DEBUG})


target_sources(${PHYSICS_TARGET} PRIVATE ${BULLET_INCLUDE_DIR})


#include auto-source regeneration script for this target if there if there is one
set(AUTO_REGEN_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/_scripts/generate_cmake_physics.sh")
if(EXISTS ${AUTO_REGEN_SCRIPT})
	target_sources(${PHYSICS_TARGET} PRIVATE ${AUTO_REGEN_SCRIPT})
endif()

set_property(TARGET ${PHYSICS_TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")