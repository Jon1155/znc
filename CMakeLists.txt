
### Initialization ###

cmake_minimum_required ( VERSION 2.6 FATAL_ERROR )

project ( ZNC )

### Define changeable options ###

set ( _DEBUG 0 CACHE BOOL "Create debug binary?" )
set ( RUN_FROM_SOURCE 0 CACHE BOOL "Allow ZNC to be run without make install?" )
set ( CSOCK_USE_POLL 1 CACHE BOOL "Use poll() instead of select()?" )
set ( HAVE_IPV6 1 CACHE BOOL "Enable IPv6 support?" )
set ( OPTIMIZE 1 CACHE BOOL "Let compiler optimize?" )

option ( ENABLE_EXTRA "Enable some extra modules" )

### Look for OpenSSL ###

include ( FindOpenSSL )

if ( OPENSSL_FOUND )
	include_directories ( ${OPENSSL_INCLUDE_DIR} )
	# for zncconfig.h
	set ( HAVE_LIBSSL 1 )
endif ( OPENSSL_FOUND )

### Create zncconfig.h ###

configure_file (
	"${ZNC_SOURCE_DIR}/zncconfig.h.cmake.in"
	"${ZNC_BINARY_DIR}/zncconfig.h"
)

include_directories ( ${ZNC_BINARY_DIR} )

### Basic compiler setup ###

add_definitions ( -D_FORTIFY_SOURCE=2 )

### Configure debug build ###

if ( _DEBUG )
	add_definitions ( -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_CONCEPT_CHECKS )
endif ( _DEBUG)

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

### Make main binary ###

ADD_EXECUTABLE ( znc ${CORE_SRC_FILES} )
TARGET_LINK_LIBRARIES ( znc ${OPENSSL_LIBRARIES} )

### Modules, anyone? ###

add_subdirectory ( modules )

### Output useful information ###

message ( STATUS "" )
message ( STATUS "ZNC configured:" )
message ( STATUS "  Prefix:   ${CMAKE_INSTALL_PREFIX}" )
message ( STATUS "  Debug:    ${_DEBUG}" )
message ( STATUS "  IPv6:     ${HAVE_IPV6}" )
message ( STATUS "  OpenSSL:  ${HAVE_LIBSSL}" )
message ( STATUS "  From src: ${RUN_FROM_SOURCE}" )
message ( STATUS "" )
