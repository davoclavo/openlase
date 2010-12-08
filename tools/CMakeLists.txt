#         OpenLase - a realtime laser graphics toolkit
#
# Copyright (C) 2009-2010 Hector Martin "marcan" <hector@marcansoft.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 or version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

include_directories (${CMAKE_SOURCE_DIR}/include)
link_directories (${CMAKE_BINARY_DIR}/libol)

add_executable(playilda playilda.c)
target_link_libraries(playilda ${JACK_LIBRARIES})

add_executable(playvid playvid.c trace.c)
target_link_libraries(playvid openlase avformat avcodec)