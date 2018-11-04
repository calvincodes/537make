cmake_minimum_required(VERSION 3.12)
project(537make C)

set(CMAKE_C_STANDARD 99)

add_executable(537make driver.c entities/linked_list.c entities/linked_list.h entities/graph.c entities/graph.h utils/constants.c utils/constants.h target_executor/traversal.c target_executor/traversal.h input_parser/reader.c input_parser/reader.h target_executor/command_executor.c target_executor/command_executor.h utils/validator.c utils/validator.h)
