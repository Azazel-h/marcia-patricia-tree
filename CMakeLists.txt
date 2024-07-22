cmake_minimum_required(VERSION 3.28)
project(merkle-patricia-tree)

if (NOT DEFINED version)
    set(version "1.0.0")
endif ()

if (NOT DEFINED namespace)
    set(namespace "mpt")
endif ()

add_subdirectory(external)
#target_link_libraries(${PROJECT_NAME} PRIVATE evmc intx)
# here you can see how instead of writing "SomeLibrary"
# we can just use the PROJECT_NAME variable
add_library(${PROJECT_NAME} STATIC)

# no need to add headers here, only sources are required

file(
    GLOB_RECURSE
    SILKWORM_CORE_SRC
    CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
    "*.c"
    "*.h"
)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SILKWORM_CORE_SRC}
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        evmc
)

# note that it is not CMAKE_INSTALL_PREFIX we are checking here
if (DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
            STATUS
            "CMAKE_INSTALL_PREFIX is not set\n"
            "Default value: ${CMAKE_INSTALL_PREFIX}\n"
            "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
            "${CMAKE_SOURCE_DIR}/install"
            CACHE PATH "Where the library will be installed to" FORCE
    )
else ()
    message(
            STATUS
            "CMAKE_INSTALL_PREFIX was already set\n"
            "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif ()

# without it public headers won't get installed
set(public_headers
        include/some.h
)
# note that ${public_headers} has to be in quotes
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

# for CMAKE_INSTALL_INCLUDEDIR definition
include(GNUInstallDirs)

# the variant with PUBLIC_HEADER property unfortunately does not preserve the folder structure
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
# so instead we iterate through public headers and install them "manually"
foreach (header ${public_headers})
    get_filename_component(header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${header}" ABSOLUTE)
    get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
    file(RELATIVE_PATH header_install_path "${CMAKE_CURRENT_SOURCE_DIR}/include" "${header_directory_path}")
    install(FILES ${header_file_path}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_install_path}"
    )
endforeach ()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

# if you haven't included it already
# definitions of CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR and others
include(GNUInstallDirs)

# install the target and create export-set
install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        # these get default values from GNUInstallDirs, no need to set them
        #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # bin
        #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
        #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib
        # except for public headers, as we want them to be inside a library folder
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} # include/SomeLibrary
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
)

# generate and install export file
install(EXPORT "${PROJECT_NAME}Targets"
        FILE "${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${namespace}::
        DESTINATION cmake
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion
)
# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION cmake
)
# install config files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION cmake
)
# generate the export targets for the build tree
# (can't say what this one is for, but so far it has been only causing me problems,
# so I stopped adding it to projects)
# export(EXPORT "${PROJECT_NAME}Targets"
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
#     NAMESPACE ${namespace}::
#)