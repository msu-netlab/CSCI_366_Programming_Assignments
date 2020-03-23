#copy the board files into runtime directory
configure_file(${PROJECT_SOURCE_DIR}/player_1.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/player_1.setup_board.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/player_2.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/player_2.setup_board.txt COPYONLY)

# convert player setup boards to unix line endings
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/player_1.setup_board.txt)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/src/player_2.setup_board.txt)

include_directories(${PROJECT_SOURCE_DIR}/cereal/include)

set_source_files_properties(BitArray2D.asm PROPERTIES COMPILE_FLAGS "-g -Fdwarf")

add_library(SERVER Server.cpp Server.hpp BitArray2D.cpp BitArray2D.hpp BitArray2D.asm)

add_library(CLIENT Client.cpp Client.hpp)

add_executable(run_server server_main.cpp Server.cpp)

add_executable(run_client client_main.cpp Client.cpp)
