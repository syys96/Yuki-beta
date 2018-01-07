//
// Created by root on 1/5/18.
//

#ifndef LEELAZ_BOARD_H
#define LEELAZ_BOARD_H

#include <vector>
#include "iostream"
#include "config.h"

const int dx[] = { -1,-1,-1,0,0,1,1,1 };
const int dy[] = { -1,0,1,-1,1,-1,0,1 };
const int DIRCNUM = 8;


class Board
{
public:
    Board();
    bool inMap(int x, int y);
    void reset_board();
    void init_reach();
    void init_near();
    void init_between();
    bool check_pos(int color, int pos) const;
    void set_pos(int color, int pos);
    void get_one_pos(int64 state, std::vector<int> &ret);
    int64 get_blank();
    int64 get_near_opp(int color);
    bool check_no_obstacle(int pos1, int pos2, int64 state);
    bool canflip(int color, int pos);
    void flip(int color, int pos);
    void gen_nodes(int color, std::vector<int> &ret);
    void print_table();
    int area_score();

private:
    int64 state[2];
    long long reach[BOARD_SIZE * BOARD_SIZE] = { 0LL };
    long long near[BOARD_SIZE * BOARD_SIZE] = { 0LL };
    long long between[BOARD_SIZE * BOARD_SIZE][BOARD_SIZE * BOARD_SIZE] = {{0LL}};
};

#endif //LEELAZ_BOARD_H
