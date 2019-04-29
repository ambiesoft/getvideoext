# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(LSMISC_DIR ../../lsMisc)

include_directories(${LSMISC_DIR})
include_directories(.)

# Add source to this project's executable.
add_executable (getvideoext 
"getvideoext.cpp" "getvideoext.h" 
"${LSMISC_DIR}/CommandLineParser.cpp" "${LSMISC_DIR}/CommandLineParser.h"
"${LSMISC_DIR}/UTF16toUTF8.cpp" "${LSMISC_DIR}/UTF16toUTF8.h"
"${LSMISC_DIR}/UrlEncode.cpp" "${LSMISC_DIR}/UrlEncode.h"
"${LSMISC_DIR}/RunCommandGetResult.cpp" "${LSMISC_DIR}/RunCommandGetResult.h"
"${LSMISC_DIR}/stdosd/stdosd.cpp" "${LSMISC_DIR}/stdosd/stdosd_win.cpp" "${LSMISC_DIR}/stdosd/stdosd.h"
)

# TODO: Add tests and install targets if needed.
