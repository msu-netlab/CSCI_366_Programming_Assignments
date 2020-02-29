#copy the board files into runtime directory
configure_file(${PROJECT_SOURCE_DIR}/player_1.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_1.setup_board.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/player_2.setup_board.txt ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_2.setup_board.txt COPYONLY)

# copy test action boards into runtime directory
configure_file(${PROJECT_SOURCE_DIR}/test/correct_start_action_board.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_start_action_board.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_fire_message.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_fire_message.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_hit_action_board.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_hit_action_board.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_miss_action_board.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_miss_action_board.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_hit_result.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_hit_result.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_miss_result.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_miss_result.json
        COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/test/correct_out_of_bounds_result.json
        ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_out_of_bounds_result.json
        COPYONLY)

# convert boards to unix line endings
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_1.setup_board.txt)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/player_2.setup_board.txt)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_start_action_board.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_fire_message.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_hit_action_board.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_miss_action_board.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_hit_result.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_miss_result.json)
execute_process(COMMAND dos2unix ${PROJECT_SOURCE_DIR}/cmake-build-debug/test/correct_out_of_bounds_result.json)

add_executable(battleship_tests tests.cpp)
include_directories(${PROJECT_SOURCE_DIR}/dtl/dtl ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/cereal/include)
target_link_libraries(battleship_tests SERVER CLIENT gtest gmock_main)