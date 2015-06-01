cmake_minimum_required(VERSION 2.8)
project(Sub3)

# Set default build configuration
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_definitions(-DDEBUG_MESSAGES)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Get the git commit hash
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)

# Set version information in a config.h file
set(Sub3_VERSION_MAJOR 0)
set(Sub3_VERSION_MINOR 1)
configure_file(
    "${PROJECT_SOURCE_DIR}/config.cpp.in"
    "${PROJECT_SOURCE_DIR}/config.cpp"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/debug")
add_subdirectory("${PROJECT_SOURCE_DIR}/graphics")
add_subdirectory("${PROJECT_SOURCE_DIR}/graphics/widgets")
add_subdirectory("${PROJECT_SOURCE_DIR}/physics")
add_subdirectory("${PROJECT_SOURCE_DIR}/network")
add_subdirectory("${PROJECT_SOURCE_DIR}/server")
add_subdirectory("${PROJECT_SOURCE_DIR}/simulation")
add_subdirectory("${PROJECT_SOURCE_DIR}/simulation/vessels")

set(HEADERS ${HEADERS} ${PROJECT_SOURCE_DIR}/config.hpp)
set(SOURCE ${SOURCE} ${PROJECT_SOURCE_DIR}/config.cpp)

set(GAME_SOURCE ${SOURCE} ${PROJECT_SOURCE_DIR}/main.cpp)
set(SERVER_SOURCE ${SOURCE} ${PROJECT_SOURCE_DIR}/server.cpp)

add_library(Sub3SharedObjects OBJECT ${SOURCE} ${HEADERS})

set(GAME_NAME "sub3")
add_executable(${GAME_NAME} ${PROJECT_SOURCE_DIR}/main.cpp $<TARGET_OBJECTS:Sub3SharedObjects>)

set(SERVER_NAME "sub3_server")
add_executable(${SERVER_NAME} ${PROJECT_SOURCE_DIR}/server.cpp $<TARGET_OBJECTS:Sub3SharedObjects>)

# Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find threads
find_package (Threads)
target_link_libraries(${GAME_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${SERVER_NAME} ${CMAKE_THREAD_LIBS_INIT})

# Find Boost
find_package(Boost COMPONENTS serialization system)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${GAME_NAME} ${Boost_LIBRARIES})
    target_link_libraries(${SERVER_NAME} ${Boost_LIBRARIES})
endif()

# Find SFML
find_package(SFML 2.3 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${GAME_NAME} ${SFML_LIBRARIES})
    target_link_libraries(${SERVER_NAME} ${SFML_LIBRARIES})
endif()

# Find SFGUI
find_package(SFGUI REQUIRED)
if(SFGUI_FOUND)
    include_directories(${SFGUI_INCLUDE_DIR})
    target_link_libraries(${GAME_NAME} ${SFGUI_LIBRARY})
    target_link_libraries(${SERVER_NAME} ${SFGUI_LIBRARY})
endif()

# Find USML
find_package(USML REQUIRED)
if(USML_FOUND)
    message("Copying oceanographic data. This could take a while.")

    file(COPY "${USML_INCLUDE_DIRS}/usml/data/woa09/temperature_seasonal_1deg.nc" DESTINATION "${PROJECT_BINARY_DIR}/usml/woa09/")
    file(COPY "${USML_INCLUDE_DIRS}/usml/data/woa09/temperature_monthly_1deg.nc" DESTINATION "${PROJECT_BINARY_DIR}/usml/woa09/")

    file(COPY "${USML_INCLUDE_DIRS}/usml/data/woa09/salinity_seasonal_1deg.nc" DESTINATION "${PROJECT_BINARY_DIR}/usml/woa09/")
    file(COPY "${USML_INCLUDE_DIRS}/usml/data/woa09/salinity_monthly_1deg.nc" DESTINATION "${PROJECT_BINARY_DIR}/usml/woa09/")

    file(COPY "${USML_INCLUDE_DIRS}/usml/data/bathymetry/ETOPO1_Ice_g_gmt4.grd" DESTINATION "${PROJECT_BINARY_DIR}/usml/bathymetry/")

    include_directories(${USML_INCLUDE_DIRS})
    target_link_libraries(${GAME_NAME} ${USML_LIBRARIES})
    target_link_libraries(${SERVER_NAME} ${USML_LIBRARIES})
endif()

# Find NetCDF
find_package(NetCDF REQUIRED)
if(NETCDF_FOUND)
    include_directories(${NETCDF_INCLUDES})
    target_link_libraries(${GAME_NAME} ${NETCDF_LIBRARIES})
    target_link_libraries(${SERVER_NAME} ${NETCDF_LIBRARIES})
endif()

# Allow includes without "../" all over the place
include_directories(${PROJECT_SOURCE_DIR})

# Install target
install(TARGETS ${GAME_NAME} DESTINATION bin)
install(TARGETS ${SERVER_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${SUB3_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${SUB3_VERSION_MINOR}")
include(CPack)
