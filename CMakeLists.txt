
### Initialization ###

cmake_minimum_required ( VERSION 2.6 FATAL_ERROR )

project ( ZNC )

find_package ( PkgConfig )

set ( ZNC_COMPILE_FLAGS "" )
set ( ZNC_LINK_FLAGS "" )

### Define changeable options ###

set ( _DEBUG 0 CACHE BOOL "Create debug binary?" )
set ( RUN_FROM_SOURCE 0 CACHE BOOL "Allow ZNC to be run without make install?" )
set ( CSOCK_USE_POLL 1 CACHE BOOL "Use poll() instead of select()?" )
set ( HAVE_IPV6 1 CACHE BOOL "Enable IPv6 support?" )

option ( ENABLE_EXTRA "Enable some extra modules" )

### Look for basic functions ###

include ( CheckSymbolExists )

check_symbol_exists ( lstat sys/stat.h HAVE_LSTAT )

check_symbol_exists ( getopt_long getopt.h HAVE_GETOPT_LONG )
if ( NOT HAVE_GETOPT_LONG )
	message ( FATAL_ERROR "Missing getopt_long() function." )
endif ()

set ( CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${CMAKE_DL_LIBS} )
check_symbol_exists ( dlopen dlfcn.h HAVE_DLOPEN )
if ( NOT HAVE_DLOPEN )
	message ( FATAL_ERROR "Missing dlopen() function." )
endif ()

### Look for OpenSSL ###

find_package ( OpenSSL )

if ( OPENSSL_FOUND )
	include_directories ( ${OPENSSL_INCLUDE_DIR} )
	# for zncconfig.h
	set ( HAVE_LIBSSL 1 )
endif ( OPENSSL_FOUND )

### Look for c-ares ###

if ( PKG_CONFIG_FOUND )
	pkg_check_modules ( CARES libcares>=1.5.0 )
	if ( CARES_FOUND )
		set ( HAVE_C_ARES 1 )
		include_directories ( ${CARES_INCLUDE_DIRS} )
		link_directories ( ${CARES_LIBRARY_DIRS} )
		set ( ZNC_COMPILE_FLAGS "${ZNC_COMPILE_FLAGS} ${CARES_CFLAGS}" )
		set ( ZNC_LINK_FLAGS "${ZNC_LINK_FLAGS} ${CARES_LDFLAGS}" )
	endif ()
endif ()

### Create zncconfig.h ###

configure_file (
	"${ZNC_SOURCE_DIR}/zncconfig.h.cmake.in"
	"${ZNC_BINARY_DIR}/zncconfig.h"
)

include_directories ( ${ZNC_BINARY_DIR} )

### Collect core files ###

set ( CORE_SRC_FILES
	Buffer.cpp
	DCCBounce.cpp
	Listener.cpp
	Server.cpp
	Utils.cpp
	Chan.cpp
	DCCSock.cpp
	main.cpp
	SHA256.cpp
	WebModules.cpp
	ClientCommand.cpp
	FileUtils.cpp
	MD5.cpp
	Socket.cpp
	znc.cpp
	Client.cpp
	HTTPSock.cpp
	Modules.cpp
	Template.cpp
	ZNCString.cpp
	Csocket.cpp
	IRCSock.cpp
	Nick.cpp
	User.cpp
)

### Define main binary ###

add_executable ( znc ${CORE_SRC_FILES} )

target_link_libraries ( znc ${OPENSSL_LIBRARIES} ${CARES_LIBRARIES} ${CMAKE_DL_LIBS} )

### Compiler setup ###

add_definitions ( -D_FORTIFY_SOURCE=2 )

if ( CMAKE_COMPILER_IS_GNUCXX )
	set ( ZNC_COMPILE_FLAGS "${ZNC_COMPILE_FLAGS} -Wall -W -Wno-unused-parameter -Woverloaded-virtual -Wshadow" )
endif ()

### Configure debug build ###

if ( _DEBUG )
	add_definitions ( -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_CONCEPT_CHECKS )
endif ( _DEBUG)

### Final steps ###

set_target_properties ( znc PROPERTIES COMPILE_FLAGS "${ZNC_COMPILE_FLAGS}" LINK_FLAGS "${ZNC_LINK_FLAGS}" )

### Modules, anyone? ###

add_subdirectory ( modules )

### Output useful information ###

message ( STATUS "" )
message ( STATUS "ZNC configured:" )
message ( STATUS "  Prefix:   ${CMAKE_INSTALL_PREFIX}" )
message ( STATUS "  Debug:    ${_DEBUG}" )
message ( STATUS "  IPv6:     ${HAVE_IPV6}" )
message ( STATUS "  OpenSSL:  ${HAVE_LIBSSL}" )
message ( STATUS "  c-ares:   ${HAVE_C_ARES}" )
message ( STATUS "  From src: ${RUN_FROM_SOURCE}" )
message ( STATUS "" )
