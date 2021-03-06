link_libraries(Babylon Threads::Threads)
include_directories( ${INCLUDES} )
add_executable(Basic WIN32 basic.cpp ${ICON})
add_executable(AshikminTest WIN32 ashikminTest.cpp ${ICON} )
set_target_properties(Basic PROPERTIES COMPILE_FLAGS "-mavx2")
set_target_properties(AshikminTest PROPERTIES COMPILE_FLAGS "-mavx2")

# Windows config stuff
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if (WIN32)
    set(ICON example.rc)
elseif (APPLE)
    set(ICON example.icns)
    set_source_files_properties(example.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(WIN32)
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(Basic PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    set_target_properties(AshikminTest PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
	endif()
endif()

set(WINDOWS_BINARIES Basic AshikminTest)
set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                      FOLDER "Babylon-Examples")
# Mac Configs
if (APPLE)
   set_target_properties(Basic PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Basic Example")
   set_target_properties(AshikminTest PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Basic Example")

   set_target_properties(${WINDOWS_BINARIES} PROPERTIES
                         RESOURCE example.icns
                         MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
                         MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0.0
                         MACOSX_BUNDLE_ICON_FILE example.icns
                         MACOSX_BUNDLE_INFO_PLIST "${Babylon_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in")
endif()
