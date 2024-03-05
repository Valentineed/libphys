#LIBPHYS
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CMakeLists_sources_lib.cmake)
set(PHYSIC_LIB libphys)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(PHYSX_DIR ${VENDOR_DIR}/PhysX)

set(PHYSX_LIB_DEBUG ${PHYSX_DIR}/lib/debug)
set(PHYSX_LIB_RELEASE ${PHYSX_DIR}/lib/release)

add_library(${PHYSIC_LIB} SHARED ${libphys_SOURCES})


set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${PHYSIC_LIB}_SOURCES})

include(GenerateExportHeader)
generate_export_header(${PHYSIC_LIB}
		BASE_NAME ${PHYSIC_LIB}
		EXPORT_MACRO_NAME ${PHYSIC_LIB}_API
		EXPORT_FILE_NAME ${PHYSIC_LIB}_Export.h
		STATIC_DEFINE ${PHYSIC_LIB}_STATIC)

if(MSVC)
target_compile_options(${PHYSIC_LIB} PRIVATE /W4 /WX)
endif()

target_include_directories(${PHYSIC_LIB} PUBLIC ${LIB_DIR}
							${PROJECT_SOURCE_DIR}/libMaths/source
							${PROJECT_SOURCE_DIR}/build/libphys
							${PROJECT_SOURCE_DIR}/build/libMaths
							${PROJECT_SOURCE_DIR}/build/ToolsEngine
							${PHYSX_DIR}/include)


target_link_libraries(${PHYSIC_LIB} debug ${PHYSX_LIB_DEBUG}/*.lib)
target_link_libraries(${PHYSIC_LIB} optimized ${PHYSX_LIB_RELEASE}/*.lib)
target_link_libraries(${PHYSIC_LIB}  ${MATHS_LIB})
target_link_libraries(${PHYSIC_LIB}  ${TOOLS_ENGINE})

if(MSVC)
target_compile_definitions(${PHYSIC_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

set(NOT_RELEASE $<NOT:$<CONFIG:Release>>)
set(NOT_DEBUG $<NOT:$<CONFIG:Debug>>)
set(NOT_SHIPPING $<NOT:$<CONFIG:Shipping>>)

add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    # do nothing for non-Release build
    $<${NOT_RELEASE}:echo>
    # output text to make clear that the copy command is not actually issued
    $<${NOT_RELEASE}:"copy omitted for non-release build">
    # the actual copy command, which is overridden by the "echo" above
    # in the case of a non-release build
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysX_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_RELEASE}:echo> $<${NOT_RELEASE}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXCommon_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_RELEASE}:echo> $<${NOT_RELEASE}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXCooking_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_RELEASE}:echo> $<${NOT_RELEASE}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXFoundation_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)

add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_DEBUG}:echo> $<${NOT_DEBUG}:"copy omitted for non-DEBUG build">
    copy_if_different ${PHYSX_LIB_DEBUG}/PhysX_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_DEBUG}:echo> $<${NOT_DEBUG}:"copy omitted for non-DEBUG build">
    copy_if_different ${PHYSX_LIB_DEBUG}/PhysXCommon_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_DEBUG}:echo> $<${NOT_DEBUG}:"copy omitted for non-DEBUG build">
    copy_if_different ${PHYSX_LIB_DEBUG}/PhysXCooking_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_DEBUG}:echo> $<${NOT_DEBUG}:"copy omitted for non-DEBUG build">
    copy_if_different ${PHYSX_LIB_DEBUG}/PhysXFoundation_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
	
	add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_SHIPPING}:echo>
    $<${NOT_SHIPPING}:"copy omitted for non-release build">
	copy_if_different ${PHYSX_LIB_RELEASE}/PhysX_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_SHIPPING}:echo> $<${NOT_SHIPPING}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXCommon_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_SHIPPING}:echo> $<${NOT_SHIPPING}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXCooking_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
add_custom_command(TARGET ${PHYSIC_LIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    $<${NOT_SHIPPING}:echo> $<${NOT_SHIPPING}:"copy omitted for non-release build">
    copy_if_different ${PHYSX_LIB_RELEASE}/PhysXFoundation_64.dll $<TARGET_FILE_DIR:${PHYSIC_LIB}>)
