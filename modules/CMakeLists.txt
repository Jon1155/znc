
include_directories ( ${ZNC_SOURCE_DIR} )

set ( DISABLED_MODULES modpython modtcl modperl saslauth )

function ( znc_build_modules FOLDER )
	file ( GLOB MODULE_FILES "${FOLDER}/*.cpp" )
	list ( SORT MODULE_FILES )

	foreach ( MODULE_FILE ${MODULE_FILES} )
		string ( REGEX REPLACE "^.+/([^/]+)\\.cpp$" "\\1" MODULE_NAME ${MODULE_FILE} )

		list ( FIND DISABLED_MODULES ${MODULE_NAME} IS_DISABLED )
		if ( IS_DISABLED EQUAL -1 )
			message ( STATUS "Enabling module: ${MODULE_NAME}" )
			add_library ( ${MODULE_NAME} MODULE ${MODULE_FILE} )
			set_target_properties ( ${MODULE_NAME} PROPERTIES PREFIX "" )
		endif ()
	endforeach ()
endfunction ()

znc_build_modules ( "${ZNC_SOURCE_DIR}/modules" )

if ( ENABLE_EXTRA )

	### find (lib)iconv ###
	include ( "${ZNC_SOURCE_DIR}/modules/FindIconv.cmake" )	
	if ( NOT ICONV_FOUND )
		list ( APPEND DISABLED_MODULES charset )
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

