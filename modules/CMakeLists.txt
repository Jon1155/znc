
include_directories ( ${ZNC_SOURCE_DIR} )

set ( DISABLED_MODULES modpython modperl saslauth )

function ( znc_build_modules FOLDER )
	message ( STATUS "Scanning ${FOLDER}..." )

	file ( GLOB MODULE_FILES "${FOLDER}/*.cpp" )
	list ( SORT MODULE_FILES )

	foreach ( MODULE_FILE ${MODULE_FILES} )
		string ( REGEX REPLACE "^.+/([^/]+)\\.cpp$" "\\1" MODULE_NAME ${MODULE_FILE} )

		list ( FIND DISABLED_MODULES ${MODULE_NAME} IS_DISABLED )
		if ( IS_DISABLED EQUAL -1 )
			message ( STATUS "Enabling module: ${MODULE_NAME}" )
			add_library ( ${MODULE_NAME} MODULE ${MODULE_FILE} )
			set_target_properties ( ${MODULE_NAME} PROPERTIES PREFIX "" )
			set_target_properties ( ${MODULE_NAME} PROPERTIES COMPILE_FLAGS ${ZNC_COMPILE_FLAGS} )

			install ( TARGETS ${MODULE_NAME} LIBRARY DESTINATION lib/znc )
			if ( EXISTS "${FOLDER}/${MODULE_NAME}" AND IS_DIRECTORY "${FOLDER}/${MODULE_NAME}" )
				install ( DIRECTORY "${FOLDER}/${MODULE_NAME}" DESTINATION "share/znc" )
			endif ()
		endif ()
	endforeach ()
endfunction ()

### find tcl ###
if ( ENABLE_MODTCL )
	# suppress TK not found messages:
	set ( TCL_FIND_QUIETLY 1 )
	find_package ( TCL 8.4 )

	if ( NOT TCL_FOUND )
		message ( FATAL_ERROR "Modtcl has been enabled, but TCL doesn't seem to be installed!" )
	endif ()
else ()
	list ( APPEND DISABLED_MODULES modtcl )
endif ()

### Configure core/main modules ###
znc_build_modules ( "${ZNC_SOURCE_DIR}/modules" )

### extra options for module "modtcl" ###
if ( ENABLE_MODTCL AND TCL_FOUND )
	target_link_libraries ( "modtcl" ${TCL_LIBRARY} )
	set_target_properties ( "modtcl" PROPERTIES COMPILE_FLAGS "-I${TCL_INCLUDE_PATH}" )
endif ()

if ( ENABLE_EXTRA )

	### find (lib)iconv ###
	find_package (Iconv)
	if ( NOT ICONV_FOUND )
		list ( APPEND DISABLED_MODULES charset )
		message ( STATUS "iconv not found, disabling charset module." )
	endif ()

	### find & configure extra modules ###
	znc_build_modules ( "${ZNC_SOURCE_DIR}/modules/extra" )

	### extra options for module "charset" ###
	if ( ICONV_FOUND )
		if ( ICONV_SECOND_ARGUMENT_IS_CONST )
			set_target_properties ( "charset" PROPERTIES COMPILE_DEFINITIONS "ICONV_CONST=const" ) 
		elseif ()
			set_target_properties ( "charset" PROPERTIES COMPILE_DEFINITIONS "ICONV_CONST=" )
		endif ()

		if ( ICONV_LIBRARIES )
			TARGET_LINK_LIBRARIES ( "charset" ${ICONV_LIBRARIES} )
		endif ()

		if ( ICONV_INCLUDE_DIR )
			set_target_properties ( "charset" PROPERTIES COMPILE_FLAGS "-I${ICONV_INCLUDE_DIR}" )
		endif ()
	endif ()

endif ()
