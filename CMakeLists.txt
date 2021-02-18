cmake_minimum_required(VERSION 3.16)
project(synchronization
        VERSION "0.0.1"
        DESCRIPTION "Sample implementation of C++20 atomic_wait/notify, forked from https://github.com/ogiroux/atomic_wait"
        HOMEPAGE_URL "https://github.com/beached/atomic_wait"
        LANGUAGES C CXX
        )

set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads QUIET REQUIRED)

# ---- Warning guard ----

# Protect dependents from this project's warnings if the guard isn't disabled
set(Synchronization_warning_guard SYSTEM)
if(Synchronization_INCLUDE_WITHOUT_SYSTEM)
  set(Synchronization_warning_guard "")
endif()

option(BUILD_SHARED_LIBS "Build shared library" ON)


set(SOURCE_FILES
        lib/source.cpp
)

set(HEADER_FILES 
        include/atomic_wait
        include/barrier
        include/latch
        include/semaphore
)

include(GenerateExportHeader)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library( ${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME "include/${PROJECT_NAME}_export.h"
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
)


target_include_directories( ${PROJECT_NAME} ${Synchronization_warning_guard}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_17 )

# Install
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(
    DIRECTORY
    "${PROJECT_SOURCE_DIR}/include/"
    "${PROJECT_BINARY_DIR}/include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ${PROJECT_NAME}_Development
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME #
    COMPONENT ${PROJECT_NAME}_Runtime
    LIBRARY #
    COMPONENT ${PROJECT_NAME}_Runtime
    NAMELINK_COMPONENT ${PROJECT_NAME}_Development
    ARCHIVE #
    COMPONENT ${PROJECT_NAME}_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

set(
    ${PROJECT_NAME}_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING "CMake package config location relative to the install prefix"
)

mark_as_advanced(${PROJECT_NAME}_INSTALL_CMAKEDIR)

install(
    FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ${PROJECT_NAME}_Development
)

install(
    EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ${PROJECT_NAME}_Development
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  include(CPack)
endif()
