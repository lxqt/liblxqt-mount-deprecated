cmake_minimum_required(VERSION 2.8.11)

project(liblxqt-mount)

# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(MAJOR_VERSION 0)
set(MINOR_VERSION 9)
set(PATCH_VERSION 0)
set(LXQTMOUNT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

# Set default installation paths
set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}" CACHE PATH "Installation directory for libraries")
set(INCLUDE_INSTALL_DIR include CACHE PATH "Installation directory for includes")

find_package(Qt5Core REQUIRED QUIET)
find_package(Qt5Gui REQUIRED QUIET)
find_package(Qt5DBus REQUIRED QUIET)
find_package(Qt5LinguistTools REQUIRED QUIET)
set(LXQTMOUNT_LIBRARY_NAME "lxqtmount-qt5")
message(STATUS "Building with Qt${Qt5Core_VERSION_STRING}")

set(LXQTMOUNT_HEADER_NAMESPACE "LXQtMount")

include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${QT_INCLUDES}
)

set(lxqtmount_HDRS
    mount.h
    providers.h
)

set(lxqtmount_SRCS
    mount.cpp
    providers.cpp
    providers_udisks2.cpp
)

set(lxqtmount_MOCS
    mount.h
    providers.h
)

set(LXQTMOUNT_PUBLIC_CLASSES
    Mount
    Providers
)

QT5_WRAP_CPP(lxqtmount_MOCS ${lxqtmount_MOCS})

# use gcc visibility feature to decrease unnecessary exported symbols
if(CMAKE_COMPILER_IS_GNUCXX)
    # set visibility to hidden to hide symbols, unlesss they're exporeted manually in the code
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -fno-exceptions -Wl,-no-undefined -Wall")
endif()
add_definitions(-DCOMPILE_LIBLXQT_MOUNT)

add_library(${LXQTMOUNT_LIBRARY_NAME} SHARED ${lxqtmount_SRCS} ${lxqtmount_MOCS})

target_link_libraries(
    ${LXQTMOUNT_LIBRARY_NAME} Qt5::Core Qt5::Gui Qt5::DBus
)

set_target_properties(${LXQTMOUNT_LIBRARY_NAME} PROPERTIES
    VERSION ${LXQTMOUNT_VERSION}
    SOVERSION ${MAJOR_VERSION}
)
include(cmake/create_portable_headers.cmake)

create_portable_headers(LXQTMOUNT_NAMESPACE_HEADERS
    NAMESPACE "${LXQTMOUNT_HEADER_NAMESPACE}/"
    FILENAMES ${LXQTMOUNT_PUBLIC_CLASSES}
)

install(TARGETS ${LXQTMOUNT_LIBRARY_NAME} DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${lxqtmount_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/${LXQTMOUNT_LIBRARY_NAME})
install(FILES ${LXQTMOUNT_NAMESPACE_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/${LXQTMOUNT_LIBRARY_NAME}/${LXQTMOUNT_HEADER_NAMESPACE})

include(create_pkgconfig_file)
create_pkgconfig_file(${LXQTMOUNT_LIBRARY_NAME} "LXQt library for removable media/devices")

# cmake config file
set(LXQTMOUNT_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}/${LXQTMOUNT_LIBRARY_NAME}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/lxqtmount-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LXQTMOUNT_LIBRARY_NAME}-config.cmake
    @ONLY
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LXQTMOUNT_LIBRARY_NAME}-config.cmake" DESTINATION share/cmake/${LXQTMOUNT_LIBRARY_NAME})
install(FILES cmake/lxqtmount_use.cmake DESTINATION share/cmake/${LXQTMOUNT_LIBRARY_NAME})

# building tarball with CPack -------------------------------------------------
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VERSION_MAJOR ${MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${PATCH_VERSION})
set(CPACK_GENERATOR TBZ2)
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
include(CPack)
