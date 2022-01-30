target_sources(
	cacus
	PRIVATE
	cacus.cpp)

target_include_directories(cacus
	PUBLIC include
	PRIVATE .
)

add_subdirectory(core)
add_subdirectory(wrappers)