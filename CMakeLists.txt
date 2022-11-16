cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VERSION})

option(WITH_GTEST "Whether to build Gtest test" OFF)

add_library(async SHARED
            async.cpp
            async.h                 
            command_handler.h
            command_handler.cpp
            command.h
            command_block.h
            command_block.cpp
            static_command_block.h
            dynamic_command_block.h)

target_link_libraries(async PRIVATE -pthread)

set_target_properties(async
                        PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
                main.cpp
                server/bulk_server.cpp
                server/bulk_server.h
                server/bulk_session.cpp
                server/bulk_session.h
                async.h
                )

set_target_properties(${PROJECT_NAME}
                        PROPERTIES
                        CXX_STANDARD 17
                        CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME} async ${Boost_LIBRARIES} -pthread)


if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
    )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
