# - Find NetCDF
# Find the native NetCDF includes and library
#
#  NETCDF_INCLUDES    - where to find netcdf.h, etc
#  NETCDF_LIBRARIES   - Link these libraries when using NetCDF
#  NETCDF_FOUND       - True if NetCDF found including required interfaces (see below)
#
# The following are not for general use and are included in
# NETCDF_LIBRARIES if the corresponding option above is set.
#
#  NETCDF_LIBRARIES_C    - Just the C interface
#  NETCDF_LIBRARIES_CXX  - C++ interface, if available
#
# Normal usage would be:
#  set (NETCDF_F90 "YES")
#  find_package (NetCDF REQUIRED)
#  target_link_libraries (uses_f90_interface ${NETCDF_LIBRARIES})
#  target_link_libraries (only_uses_c_interface ${NETCDF_LIBRARIES_C})

if (NETCDF_INCLUDES AND NETCDF_LIBRARIES)
  # Already in cache, be silent
  set (NETCDF_FIND_QUIETLY TRUE)
endif (NETCDF_INCLUDES AND NETCDF_LIBRARIES)

find_path (NETCDF_INCLUDES_C netcdf.h
  HINTS ../netcdf/include ../netcdf-4.1.3-with-cmake/include ${NETCDF_DIR}/include )

find_path (NETCDF_INCLUDES_CXX netcdfcpp.h
  HINTS ../netcdf/cxx ../netcdf-4.1.3-with-cmake/cxx ${NETCDF_DIR}/cxx )

find_library (NETCDF_LIBRARIES_C NAMES netcdf
  HINTS ../netcdf/libsrc ../netcdf-4.1.3-with-cmake/libsrc ${NETCDF_DIR}/libsrc )
mark_as_advanced(NETCDF_LIBRARIES_C)

find_library (NETCDF_LIBRARIES_CXX NAMES netcdf_c++
  HINTS ../netcdf/cxx ../netcdf-4.1.3-with-cmake/cxx ${NETCDF_DIR}/cxx )
mark_as_advanced(NETCDF_LIBRARIES_CXX)

set (NETCDF_INCLUDES ${NETCDF_INCLUDES_C} ${NETCDF_INCLUDES_CXX})
set (NETCDF_LIBRARIES ${NETCDF_LIBRARIES_C} ${NETCDF_LIBRARIES_CXX})

find_package_handle_standard_args(NETCDF  DEFAULT_MSG
                                  NETCDF_LIBRARIES NETCDF_INCLUDES)

mark_as_advanced (NETCDF_LIBRARIES NETCDF_INCLUDES)
