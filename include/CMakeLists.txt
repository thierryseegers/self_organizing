# This is somewhat of a "dummy" target to put all the headers of the self_organizing library in one place rather than have all projects hold references to them.

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

source_group(headers FILES ${headers})

add_custom_target(self_organizing
				  COMMAND cmake -E echo ""
				  SOURCES ${headers} ${samples})
