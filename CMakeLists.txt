# This is used to generate the `compile_commands.json` file required by ccls/clangd
# If bear is installed, run `make fclean && bear -- make` instead.
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
PROJECT(minishell LANGUAGES C)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")

FILE(GLOB_RECURSE SOURCES
	"src/*.c")

INCLUDE_DIRECTORIES(src)
INCLUDE_DIRECTORIES(./libs/ft_printf/includes/)
INCLUDE_DIRECTORIES(./libs/ft_gnl/includes/)
INCLUDE_DIRECTORIES(./libs/libopts/includes/)
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCES})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ./libs/ft_printf/libftprintf.a)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ./libs/ft_gnl/libgnl.a)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ./libs/libopts/libopts.a)
