#
# Copyright (C) 2005-2018 MaNGOS project <http://getmangos.com/>
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

set(LIBRARY_NAME mangostool)
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

set(CURL_DISABLE_LDAP ON CACHE INTERNAL "")
set(CURL_DISABLE_LDAPS ON CACHE INTERNAL "")
set(CURL_DISABLE_TELNET ON CACHE INTERNAL "")
set(CURL_DISABLE_TFTP ON CACHE INTERNAL "")
set(CURL_DISABLE_DICT ON CACHE INTERNAL "")
set(CURL_DISABLE_FILE ON CACHE INTERNAL "")
set(CURL_DISABLE_GOPHER ON CACHE INTERNAL "")
set(CURL_DISABLE_IMAP ON CACHE INTERNAL "")
set(CURL_DISABLE_MQTT ON CACHE INTERNAL "")
set(CURL_DISABLE_POP3 ON CACHE INTERNAL "")
set(CURL_DISABLE_RTSP ON CACHE INTERNAL "")
set(CURL_DISABLE_SMB ON CACHE INTERNAL "")
set(CURL_DISABLE_SMTP ON CACHE INTERNAL "")
set(CURL_DISABLE_SRP ON CACHE INTERNAL "")
set(CURL_DISABLE_FTP ON CACHE INTERNAL "")
set(CURL_DISABLE_SCP ON CACHE INTERNAL "")
set(CURL_DISABLE_SFTP ON CACHE INTERNAL "")
set(CURL_DISABLE_LDAP ON CACHE INTERNAL "")

set(CURL_DISABLE_LIBSSH2 ON CACHE INTERNAL "")
set(CURL_USE_LIBSSH2 OFF CACHE INTERNAL "")

set(CURL_DISABLE_PROXY ON CACHE INTERNAL "")
#Base files
file(GLOB mangostool_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
  source_group("mangostool" FILES ${mangostool_source})

#Define base source library
set(LIBRARY_SRCS ${mangostool_source})

# Define ENABLE_PLAYERBOTS if need
if (BUILD_PLAYERBOTS)
  include_directories(${CMAKE_SOURCE_DIR}/src/modules/PlayerBots)
  add_definitions(-DENABLE_PLAYERBOTS)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/modules/modules/src)
add_definitions(-DENABLE_MODULES)
add_definitions(-DENABLE_DUALSPEC)

# Define Expansion
if ( ${CMAKE_PROJECT_NAME} MATCHES "Classic")
  add_definitions(-DEXPANSION=0)
endif()
if ( ${CMAKE_PROJECT_NAME} MATCHES "TBC")
  add_definitions(-DEXPANSION=1)
endif()
if ( ${CMAKE_PROJECT_NAME} MATCHES "WoTLK")
  add_definitions(-DEXPANSION=2)
endif()

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SRCS})

if(WIN32)
    include(FetchContent)
    FetchContent_Declare(
      curl
      URL https://curl.se/download/curl-8.5.0.tar.gz
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      OVERRIDE_FIND_PACKAGE
    )

    FetchContent_MakeAvailable(curl)

    find_package(CURL REQUIRED)

    target_link_libraries(${LIBRARY_NAME}
        PRIVATE
            modules
            shared
            RecastNavigation::Detour
            CURL::libcurl wldap32 ws2_32 Crypt32.lib Wldap32 Normaliz
    )
elseif(UNIX)
    find_package(CURL REQUIRED)
    target_link_libraries(${LIBRARY_NAME}
        modules
        shared
        RecastNavigation::Detour
        CURL::libcurl
    )
endif()


if(UNIX)
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(${LIBRARY_NAME} dl)
  endif()
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "MangosTool")

# Install config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/mangostool.conf.dist.in ${CMAKE_CURRENT_BINARY_DIR}/mangostool.conf.dist)
if (NOT CONF_INSTALL_DIR)
  set(CONF_INSTALL_DIR ${CONF_DIR})
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mangostool.conf.dist DESTINATION ${CONF_INSTALL_DIR})