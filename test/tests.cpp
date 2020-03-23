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
 * Compute the diff distance between two files using the dtl library
 * @param file1 - name of the first file
 * @param file2 - name of the second file
 * @return distance between the contents of two files
 */
#include <dtl.hpp> // a library for diffing strings
long get_diff_dist(string file1, string file2){
   // load the file contents
   ifstream ifstr1(file1);
   string file1_S((std::istreambuf_iterator<char>(ifstr1)),
                  std::istreambuf_iterator<char>());
   ifstr1.close();
   ifstream ifstr2(file2);
   string file2_S((std::istreambuf_iterator<char>(ifstr2)),
                  std::istreambuf_iterator<char>());
   ifstr2.close();

   // compute the distance between correct and constructed boards
   dtl::Diff<char, string> d(file1_S, file2_S);
   d.onOnlyEditDistance();
   d.compose();

   return d.getEditDistance();
}


TEST(BitArray2DCreate, Correct_1by1){
   BitArray2D *array;
   ASSERT_NO_THROW(array = new BitArray2D(1,1));
}

TEST(BitArray2DCreate, Correct_10by10){
   BitArray2D *array;
   ASSERT_NO_THROW(array = new BitArray2D(10,10));
}

TEST(BitArray2DCreate, Incorrect){
   BitArray2D *array;
   ASSERT_ANY_THROW(array = new BitArray2D(0,0));
}




class BitArray2DSetGet : public ::testing::Test{
protected:
   BitArray2D *array;

   void SetUp() override {
      array = new BitArray2D(10, 10);
   };

   void TearDown() override{
      delete array;
   }

};

TEST_F(BitArray2DSetGet, Zero){
   int row=0, col=0;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, One){
   int row=0, col=1;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Seven){
   int row=0, col=7;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Eight){
   int row=0, col=8;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Nine){
   int row=0, col=9;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Ten){
   int row=9, col=1;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Hundred){
   int row=9, col=9;
   array->set(row, col);
   ASSERT_EQ(1, array->get(row,col));
}

TEST_F(BitArray2DSetGet, Get_Out_Of_Bounds_High){
   ASSERT_ANY_THROW(array->get(BOARD_SIZE, BOARD_SIZE));
}

TEST_F(BitArray2DSetGet, Get_Out_Of_Bounds_Low){
   ASSERT_ANY_THROW(array->get(-1, -1));
}

TEST_F(BitArray2DSetGet, Set_Out_Of_Bounds_High){
   ASSERT_ANY_THROW(array->set(BOARD_SIZE, BOARD_SIZE));
}

TEST_F(BitArray2DSetGet, Set_Out_Of_Bounds_Low){
   ASSERT_ANY_THROW(array->set(-1, -1));
}

class ServerInitialize : public ::testing::Test{
protected:
    Server srv;
};

TEST_F(ServerInitialize, Correct_Board_Size){
    ASSERT_NO_THROW(srv.initialize(BOARD_SIZE, "player_1.setup_board.txt", "player_2.setup_board.txt"));
}

TEST_F(ServerInitialize, Wrong_Board_Size){
    ASSERT_ANY_THROW(srv.initialize(BOARD_SIZE-1, "player_1.setup_board.txt", "player_2.setup_board.txt"));
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
    ASSERT_EQ(HIT, srv.evaluate_shot(1,9,0));
}

TEST_F(ServerEvaluateShot, Miss_Detected){
    ASSERT_EQ(MISS, srv.evaluate_shot(1,9,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_X_High){
    ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,srv.board_size+1,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_X_Low){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,-1,1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_Y_High){
    ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,1,srv.board_size+1));
}

TEST_F(ServerEvaluateShot, Out_Of_Bounds_Y_Low){
   ASSERT_EQ(OUT_OF_BOUNDS, srv.evaluate_shot(1,1,-1));
}

TEST_F(ServerEvaluateShot, Max_In_Bounds){
    ASSERT_NO_THROW(srv.evaluate_shot(1,srv.board_size-1,srv.board_size-1));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_Low){
    ASSERT_ANY_THROW(srv.evaluate_shot(0,0,0));
}

TEST_F(ServerEvaluateShot, Bad_Player_Number_High){
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
    set_up_shot(0, 1);
    srv.process_shot(1);
    ASSERT_EQ(0, get_diff_dist("correct_hit_result.json", "player_1.result.json"));
}

TEST_F(ServerProcessShot, Miss_Detected){
    set_up_shot(1, 1);
    srv.process_shot(1);
    ASSERT_EQ(0, get_diff_dist("correct_miss_result.json", "player_1.result.json"));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_X){
    set_up_shot(srv.board_size, 0);
    srv.process_shot(1);
    ASSERT_EQ(0, get_diff_dist("correct_out_of_bounds_result.json", "player_1.result.json"));
}

TEST_F(ServerProcessShot, Out_Of_Bounds_Y){
    set_up_shot(0, srv.board_size);
    srv.process_shot(1);
    ASSERT_EQ(0, get_diff_dist("correct_out_of_bounds_result.json", "player_1.result.json"));
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
    ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_start_action_board.json"));
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
    client.fire(0,1);
    ASSERT_EQ(0, get_diff_dist("player_1.shot.json", "correct_fire_message.json"));
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
    ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_hit_action_board.json"));
}

TEST_F(ClientUpdateActionBoard, Record_Miss){
    client.update_action_board(MISS, 0, 0);

    ASSERT_EQ(0, get_diff_dist("player_1.action_board.json", "correct_miss_action_board.json"));
}



