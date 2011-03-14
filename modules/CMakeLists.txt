
include_directories ( ${ZNC_SOURCE_DIR} )

set ( DISABLED_MODULES modpython modtcl charset modperl saslauth )

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
	znc_build_modules ( "${ZNC_SOURCE_DIR}/modules/extra" )
endif ()
