cmake_minimum_required(VERSION 3.28)
project(merkle-patricia-tree VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED namespace)
    set(namespace "mpt")
endif ()


# Create the main library
add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE SILKWORM_CORE_SRC
        CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.hpp"
        "src/*.c"
        "src/*.h"
)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SILKWORM_CORE_SRC}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/intx/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ethash/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/evmone/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/evmone/evmc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/expected/include
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#add_subdirectory(external/ethash)
#add_subdirectory(external/intx)
#add_subdirectory(external/evmone)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        evmone
        evmc
)

include(GNUInstallDirs)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
            "${CMAKE_SOURCE_DIR}/install"
            CACHE PATH "Where the library will be installed to" FORCE
    )
endif ()

set(public_headers
        include/some.h
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "${public_headers}"
        DEBUG_POSTFIX "d"
)

foreach (header ${public_headers})
    get_filename_component(header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${header}" ABSOLUTE)
    get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
    file(RELATIVE_PATH header_install_path "${CMAKE_CURRENT_SOURCE_DIR}/include" "${header_directory_path}")
    install(FILES ${header_file_path}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_install_path}"
    )
endforeach ()

install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT "${PROJECT_NAME}Targets"
        FILE "${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${namespace}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)