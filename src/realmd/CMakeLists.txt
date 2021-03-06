#
# This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

set(EXECUTABLE_NAME realmd)

cmake_policy(SET CMP0043 NEW)

set(EXECUTABLE_SRCS
    AuthCodes.h
    AuthSocket.cpp
    AuthSocket.h
    Main.cpp
    PatchHandler.cpp
    PatchHandler.h
    RealmList.cpp
    RealmList.h
	SessionManager.cpp
    SessionManager.h
   )

if(WIN32)
  # add resource file to windows build
  set(EXECUTABLE_SRCS ${EXECUTABLE_SRCS} realmd.rc)
endif()

source_group("" FILES ${EXECUTABLE_SRCS})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Thread
  ${CMAKE_SOURCE_DIR}/src/shared/Util
  ${CMAKE_SOURCE_DIR}/src/shared/Log
  ${CMAKE_SOURCE_DIR}/src/shared/Platform
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/src/shared
  ${MYSQL_INCLUDE_DIR}
)

add_executable(${EXECUTABLE_NAME}
  ${EXECUTABLE_SRCS}
)

target_link_libraries(${EXECUTABLE_NAME}
  shared
  framework
  backtrace
  ${Boost_LIBRARIES}
  zlib
)

if(NOT (NEDMALLOC_USE_EXTERNAL OR USE_STD_MALLOC))
  target_link_libraries(${EXECUTABLE_NAME} nedmalloc)
endif()

if(WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  
  target_link_libraries(${EXECUTABLE_NAME}
    optimized ${MYSQL_LIBRARY}
    optimized ${OPENSSL_LIBRARIES}
    debug ${MYSQL_DEBUG_LIBRARY}
    debug ${OPENSSL_DEBUG_LIBRARIES}
	dbghelp
  )
  if(PLATFORM MATCHES X86)
    target_link_libraries(${EXECUTABLE_NAME})
  endif()
endif()

if(UNIX)
  target_link_libraries(${EXECUTABLE_NAME}
    ${MYSQL_LIBRARY}
    ${OPENSSL_LIBRARIES}
    ${OPENSSL_EXTRA_LIBRARIES}
  )
endif()

set(EXECUTABLE_LINK_FLAGS "")

if(UNIX)
  set(EXECUTABLE_LINK_FLAGS "-pthread ${EXECUTABLE_LINK_FLAGS}")
endif()

if(APPLE)
  set(EXECUTABLE_LINK_FLAGS "-framework Carbon ${EXECUTABLE_LINK_FLAGS}")
endif()

set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS
  "${EXECUTABLE_LINK_FLAGS}"
)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${BIN_DIR})
install(FILES realmd.conf.dist.in DESTINATION ${CONF_DIR} RENAME realmd.conf.dist)

if(WIN32 AND MSVC)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/\${BUILD_TYPE}/${EXECUTABLE_NAME}.pdb DESTINATION ${BIN_DIR} CONFIGURATIONS Debug)
endif()
