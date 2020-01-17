//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include <string>
#include <fstream>
#include <streambuf>

#include "common.hpp"
#include "Server.hpp"
#include "Client.hpp"



/**
 * Compute the diff distance between a string and the contents of the file using the dtl library
 * @param str - string to be compared
 * @param fname - against the contents of the file
 * @return distance between s and contents of f
 */
#include <dtl.hpp> // a library for diffing strings
long get_diff_dist(string str, string fname){
    // load the file contents
    ifstream file(fname);
    string fctnt((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

    // compute the distance between correct and constructed boards
    dtl::Diff<char, string> d(str, fctnt);
    d.onOnlyEditDistance();
    d.compose();

    // clean up and return
    file.close();
    return d.getEditDistance();
}



class ServerInitialize : public ::testing::Test{
protected:
    Server srv;
};

TEST_F(ServerInitialize, Correct_Board_Size){
    ASSERT_NO_THROW(srv.initialize(BOARD_SIZE, "player_1.setup_board.txt", "player_2.setup_board.txt"));
}

TEST_F(ServerInitialize, Wrong_Board_Size){
    ASSERT_ANY_THROW(srv.initialize(0, "player_1.setup_board.txt", "player_2.setup_board.txt"));
}

TEST_F(ServerInitialize, Bad_File_Name){
    ASSERT_ANY_THROW(srv.initialize(BOARD_SIZE, "", ""));
}


class ServerEvaluateShot : public ::testing::Test{
protected:
    Server srv;
    void SetUp() override{
        srv.initialize(BOARD_SIZE, "player_1.setup_board.txt", "player_2.setup_board.txt");
    }
};

TEST_F(ServerEvaluateShot, Hit_Detected){
    ASSERT_EQ(HIT, srv.evaluate_shot(2,1,0));
}

TEST_F(ServerEvaluateShot, Miss_Detected){
    ASSERT_EQ(MISS, srv.evaluate_shot(2,0,0));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_X){
    ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(2,srv.board_size+1,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_Y){
    ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(2,1,srv.board_size+1));
}

TEST_F(ServerEvaluateShot, Max_In_Bounds){
    ASSERT_NO_THROW(srv.evaluate_shot(2,srv.board_size-1,srv.board_size-1));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_Low){
    ASSERT_ANY_THROW(srv.evaluate_shot(0,0,0));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_Low_High){
    ASSERT_ANY_THROW(srv.evaluate_shot(MAX_PLAYERS+1,0,0));
}


class ServerProcessShot : public ::testing::Test{
protected:
    Server srv;

    void set_up_shot(unsigned int x, unsigned int y){
        string coords = "{\"x\": "+to_string(x)+",\"y\": "+to_string(y)+"}";
        ofstream shot_file("player_1.shot.json");
        shot_file << coords;
        shot_file.close();
    }

    void SetUp() override{
        srv.initialize(BOARD_SIZE, "player_1.setup_board.txt", "player_2.setup_board.txt");
    }

    void TearDown() override{
        remove("player_1.shot.json");
        remove("player_1.result.json");
    }
};


TEST_F(ServerProcessShot, Hit_Detected){
    set_up_shot(1, 1);
    srv.process_shot(1);

    string correct_result = "{\n"
                            "    \"result\": "+to_string(HIT)+"\n"
                            "}";

    ASSERT_EQ(0, get_diff_dist(correct_result, "player_1.result.json"));
}

TEST_F(ServerProcessShot, Miss_Detected){
    set_up_shot(0, 0);
    srv.process_shot(1);

    string correct_result = "{\n"
                            "    \"result\": "+to_string(MISS)+"\n"
                            "}";

    ASSERT_EQ(0, get_diff_dist(correct_result, "player_1.result.json"));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_X){
    set_up_shot(srv.board_size, 0);
    srv.process_shot(1);

    string correct_result = "{\n"
                            "    \"result\": "+to_string(OUT_OF_BOUNDS)+"\n"
                                                               "}";

    ASSERT_EQ(0, get_diff_dist(correct_result, "player_1.result.json"));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_Y){
    set_up_shot(0, srv.board_size);
    srv.process_shot(1);

    string correct_result = "{\n"
                            "    \"result\": "+to_string(OUT_OF_BOUNDS)+"\n"
                                                               "}";

    ASSERT_EQ(0, get_diff_dist(correct_result, "player_1.result.json"));
}

TEST_F(ServerProcessShot, Max_In_Bounds){
    set_up_shot(srv.board_size-1, srv.board_size-1);
    ASSERT_NO_THROW(srv.process_shot(1));
}

TEST_F(ServerProcessShot, Bad_Player_Number_Low){
    set_up_shot(0, 0);
    ASSERT_ANY_THROW(srv.process_shot(0));
}

TEST_F(ServerProcessShot, Bad_Player_Number_Low_High){
    set_up_shot(0, 0);
    ASSERT_ANY_THROW(srv.process_shot(MAX_PLAYERS+1));
}

TEST_F(ServerProcessShot, Cleanup){
    set_up_shot(0,0);
    srv.process_shot(1);
    ifstream f("player_1.shot.json");
    ASSERT_FALSE(f.good());
}


class ClientInitialize : public ::testing::Test{
protected:
    Client client;

    void SetUp() override{
        client.initialize(1, BOARD_SIZE);
    }

    void TearDown() override{
        remove("player_1.action_board.json");
    }
};

TEST_F(ClientInitialize, Creates_Action_Board){
    string correct_board = "{\n"
                           "    \"board\": [\n"
                           "        [\n"
                           "            0,\n"
                           "            0\n"
                           "        ],\n"
                           "        [\n"
                           "            0,\n"
                           "            0\n"
                           "        ]\n"
                           "    ]\n"
                           "}";

    ASSERT_EQ(0, get_diff_dist(correct_board, "player_1.action_board.json"));
}


class ClientFire : public ::testing::Test{
protected:
    Client client;

    void SetUp() override{
        client.initialize(1, BOARD_SIZE);
    }

    void TearDown() override{
        remove("player_1.shot.json");
    }
};

TEST_F(ClientFire, Creates_Fire_Message){
    client.fire(1,1);

    string correct_shot = "{\n"
                          "    \"x\": 1,\n"
                          "    \"y\": 1\n"
                          "}";

    ASSERT_EQ(0, get_diff_dist(correct_shot, "player_1.shot.json"));
}


class ClientResultAvailable : public ::testing::Test{
protected:
    Client client;

    void set_up_result(int result){
        string result_str = "{\n"
                         "    \"result\": "+to_string(result)+"\n"
                         "}";
        ofstream result_file("player_1.result.json");
        result_file << result_str;
        result_file.close();
    }

    void SetUp() override{
        client.initialize(1, BOARD_SIZE);
    }

    void TearDown() override{
        remove("player_1.result.json");
    }
};

TEST_F(ClientResultAvailable, NoResultFile){
   ASSERT_FALSE(client.result_available());
}

TEST_F(ClientResultAvailable, GoodFile){
   set_up_result(HIT);
   ASSERT_TRUE(client.result_available());
}


class ClientGetResult : public ::testing::Test{
protected:
    Client client;

    void set_up_result(int result){
        string result_str = "{\n"
                         "    \"result\": "+to_string(result)+"\n"
                         "}";
        ofstream result_file("player_1.result.json");
        result_file << result_str;
        result_file.close();
    }

    void SetUp() override{
        client.initialize(1, BOARD_SIZE);
    }

    void TearDown() override{
        remove("player_1.result.json");
    }
};

TEST_F(ClientGetResult, Return_Hit){
    set_up_result(HIT);
    ASSERT_EQ(HIT, client.get_result());
}

TEST_F(ClientGetResult, Return_Miss){
    set_up_result(MISS);
    ASSERT_EQ(MISS, client.get_result());
}

TEST_F(ClientGetResult, Return_Out_Of_Bounds){
    set_up_result(OUT_OF_BOUNDS);
    ASSERT_EQ(OUT_OF_BOUNDS, client.get_result());
}

TEST_F(ClientGetResult, Catch_Bad_Result){
    set_up_result(999);
    ASSERT_ANY_THROW(client.get_result());
}

TEST_F(ClientGetResult, Cleanup){
    set_up_result(HIT);
   client.get_result();
    ifstream f("player_1.result.json");
    ASSERT_FALSE(f.good());
}


class ClientUpdateActionBoard : public ::testing::Test{
protected:
    Client client;

    void SetUp() override{
        client.initialize(1, BOARD_SIZE);
    }

    void TearDown() override{
        remove("player_1.action_board.json");
    }
};

TEST_F(ClientUpdateActionBoard, Record_Hit){
    client.update_action_board(HIT, 0, 0);

    string correct_board = "{\n"
                           "    \"board\": [\n"
                           "        [\n"
                           "            1,\n"
                           "            0\n"
                           "        ],\n"
                           "        [\n"
                           "            0,\n"
                           "            0\n"
                           "        ]\n"
                           "    ]\n"
                           "}";

    ASSERT_EQ(0, get_diff_dist(correct_board, "player_1.action_board.json"));
}

TEST_F(ClientUpdateActionBoard, Record_Miss){
    client.update_action_board(MISS, 0, 0);

    string correct_board = "{\n"
                           "    \"board\": [\n"
                           "        [\n"
                           "            -1,\n"
                           "            0\n"
                           "        ],\n"
                           "        [\n"
                           "            0,\n"
                           "            0\n"
                           "        ]\n"
                           "    ]\n"
                           "}";

    ASSERT_EQ(0, get_diff_dist(correct_board, "player_1.action_board.json"));
}



