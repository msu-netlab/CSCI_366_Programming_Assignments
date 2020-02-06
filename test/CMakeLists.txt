#copy the board files into runtime directory
configure_file(${PROJECT_SOURCE_DIR}/player_1.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_1.setup_board.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/player_2.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_2.setup_board.txt COPYONLY)

# convert player setup boards to unix line endings
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_1.setup_board.txt)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_2.setup_board.txt)

add_executable(battleship_tests tests.cpp)
include_directories(${PROJECT_SOURCE_DIR}/dtl/dtl ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/cereal/include)
target_link_libraries(battleship_tests SERVER CLIENT gtest gmock_main)