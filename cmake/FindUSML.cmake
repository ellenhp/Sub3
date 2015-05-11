# - Try to find USML
# Once done this will define
#  USML_FOUND
#  USML_HOME
#  USML_LIBRARIES

find_path(USML_INCLUDE_DIR usml_config.h
          HINTS ${USML_HOME} ../usml ENV USML_HOME)

find_library(USML_LIBRARY NAMES usml
             HINTS ${USML_HOME} ../usml ENV USML_HOME )

set(USML_LIBRARIES ${USML_LIBRARY} )
set(USML_INCLUDE_DIRS ${USML_INCLUDE_DIR}/../ )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(USML  DEFAULT_MSG
                                  USML_LIBRARY USML_INCLUDE_DIR)

mark_as_advanced(USML_INCLUDE_DIR USML_LIBRARY )
