#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "protobuf::libprotobuf-lite" for configuration ""
set_property(TARGET protobuf::libprotobuf-lite APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(protobuf::libprotobuf-lite PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libprotobuf-lite.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS protobuf::libprotobuf-lite )
list(APPEND _IMPORT_CHECK_FILES_FOR_protobuf::libprotobuf-lite "${_IMPORT_PREFIX}/lib/libprotobuf-lite.a" )

# Import target "protobuf::libprotobuf" for configuration ""
set_property(TARGET protobuf::libprotobuf APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(protobuf::libprotobuf PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libprotobuf.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS protobuf::libprotobuf )
list(APPEND _IMPORT_CHECK_FILES_FOR_protobuf::libprotobuf "${_IMPORT_PREFIX}/lib/libprotobuf.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
