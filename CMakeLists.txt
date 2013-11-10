cmake_minimum_required(VERSION 2.6)
# additional cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(liblxqt-mount)

set (MAJOR_VERSION 0)
set (MINOR_VERSION 5)
set (PATCH_VERSION 0)
set (LXQT_MOUNT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

find_package(X11 REQUIRED QUIET)

find_package(LXQT REQUIRED)
include(${LXQT_USE_FILE})

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDES}
	${X11_INCLUDE_DIR}
	${LXQT_INCLUDE_DIRS}
)

set(lxqtmount_HDRS
    lxqtmount.h
    rzmountproviders.h
)

set(lxqtmount_SRCS
    lxqtmount.cpp
    rzmountproviders.cpp
    rzmountproviders_udisks2.cpp
)

set(lxqtmount_MOCS
    lxqtmount.h
    rzmountproviders.h
)

QT4_WRAP_CPP(razourmount_MOCS ${lxqtmount_MOCS})

# use gcc visibility feature to decrease unnecessary exported symbols
if (CMAKE_COMPILER_IS_GNUCXX)
  # set visibility to hidden to hide symbols, unlesss they're exporeted manually in the code
  set(CMAKE_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -fno-exceptions")
endif()
add_definitions(-DCOMPILE_LIBLXQT_MOUNT)

add_library ( lxqtmount SHARED ${lxqtmount_SRCS} ${lxqtmount_MOCS} )

target_link_libraries ( lxqtmount ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
                                 ${QT_QTDBUS_LIBRARY}
				 # ${UDEV_LIBRARIES} 
				 ${LXQT_LIBRARIES}
                      )

set_target_properties(lxqtmount PROPERTIES
  VERSION ${LXQT_VERSION}
  SOVERSION ${LXQT_MAJOR_VERSION}
)

install(TARGETS lxqtmount DESTINATION lib${LIB_SUFFIX})
install(FILES ${lxqtmount_HDRS} DESTINATION include/lxqtmount)

include(create_pkgconfig_file)
create_pkgconfig_file(lxqtmount "lxde-qt library for removable media/devices")

# cmake config file
set(LXQTMOUNT_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/lxqtmount)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/lxqtmount-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/lxqtmount-config.cmake
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lxqtmount-config.cmake DESTINATION share/cmake/lxqtmount)
install(FILES cmake/lxqtmount_use.cmake DESTINATION share/cmake/lxqtmount)
