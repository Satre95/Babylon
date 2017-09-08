link_libraries(Babylon)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (WIN32)
    set(ICON example.rc)
elseif (APPLE)
    set(ICON example.icns)
    set_source_files_properties(example.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

add_executable(basic WIN32 MACOSX_BUNDLE basic.cpp ${ICON})

if(WIN32)
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(basic PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	endif()
endif()
set(WINDOWS_BINARIES basic)

set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                      FOLDER "StarForge-Examples")
					  

if (APPLE)
    set_target_properties(basic PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Basic Example")

    set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                          RESOURCE example.icns
                          MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
                          MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0.0
                          MACOSX_BUNDLE_ICON_FILE example.icns
                          MACOSX_BUNDLE_INFO_PLIST "${StarForge_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in")
endif()					  