INCLUDE_DIRECTORIES(.)

SET(LIDAR_SRC lidar.cc lidar.hpp ReadParsePackage.cc ReadParsePackage.hpp rOc_serial.cc rOc_serial.hpp)
ADD_LIBRARY(lidar ${LIDAR_SRC})
IF (WIN32)
ELSE()
    TARGET_LINK_LIBRARIES(lidar pthread)
ENDIF()
