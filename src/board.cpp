//
// Created by root on 1/5/18.
//

#include "board.h"

Board::Board() {
    init_near();
    init_reach();
    init_between();
    reset_board();
}

bool Board::check_pos(int color, int pos) const {
    return (bool)((state[color] >> pos) & (int64)1);
}

void Board::reset_board() {
    state[0] = 0x0000000810000000;
    state[1] = 0x0000001008000000;
}

void Board::set_pos(int color, int pos) {
    state[color] ^= ((int64)1 << pos);
}

void Board::get_one_pos(int64 state, std::vector<int> &ret) {
    int pos = 0;
    for (int64 temp = state; temp; temp ^= ((int64)1 << pos)) {
        pos =  __builtin_ctzll(temp);
        ret.emplace_back(pos);
    }
}

void Board::init_reach() {
    int x, y;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            long long temp = 0LL;
            for (int dir = 0; dir < DIRCNUM; dir++) {
                for (int delta = 1; delta < BOARD_SIZE; delta++) {
                    x = i + dx[dir] * delta;
                    y = j + dy[dir] * delta;
                    if (!inMap(x, y))
                        break;
                    temp |= (1LL << (x << 3 | y));
                }
            }
            reach[i << 3 | j] = temp;
        }
    }
}

bool Board::inMap(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

void Board::init_near() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            long long temp = 0LL;
            int x, y;
            for (int dirc = 0; dirc < DIRCNUM; dirc++) {
                x = i + dx[dirc];
                y = j + dy[dirc];
                if (inMap(x, y)) {
                    temp |= (1LL << (x << 3 | y));
                }
            }
            near[i << 3 | j] = temp;
        }
    }
}

void Board::init_between() {
    int a, b;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            for (int dir = 0; dir < DIRCNUM; dir++) {
                for (int delta = 1; delta < BOARD_SIZE; delta++) {
                    a = i + dx[dir] * delta;
                    b = j + dy[dir] * delta;
                    if (!inMap(a, b))
                        break;
                    long long temp = 0LL;
                    int tempX, tempY;
                    for (int k = 1; k < delta; k++) {
                        tempX = i + dx[dir] * k;
                        tempY = j + dy[dir] * k;
                        temp |= 1LL << (tempX << 3 | tempY);
                    }
                    between[i << 3 | j][a << 3 | b] = temp;
                }
            }
        }
    }
}

int64 Board::get_blank() {
    return ~(state[0] | state[1]);
}

bool Board::check_no_obstacle(int pos1, int pos2, int64 state) {
    return between[pos1][pos2]
           == (between[pos1][pos2] & state);
}

bool Board::canflip(int color, int pos) {
    int check_pos = 0;
    for (long long temp = reach[pos] & state[color]; temp; temp ^= (1LL << check_pos)) {
        check_pos = __builtin_ctzll(temp);
        if (check_no_obstacle(check_pos, pos, state[color ^ 1])
            && !(near[check_pos] & (1LL << pos)))
            return true;
    }
    return false;
}

void Board::flip(int color, int pos) {
    int check_pos = 0;
    for (long long temp = reach[pos] & state[color]; temp; temp ^= (1LL << check_pos)) {
        check_pos = __builtin_ctzll(temp);
        if (check_no_obstacle(check_pos, pos, state[color ^ 1])) {
            state[color] |= between[check_pos][pos];
            state[color ^ 1] ^= between[check_pos][pos];
        }
    }
}

void Board::gen_nodes(int color, std::vector<int> &ret) {
    long long near_enemy = get_near_opp(color);
    int near_enemy_pos = 0;
    for (long long temp = near_enemy; temp; temp ^= (1LL << near_enemy_pos)) {
        near_enemy_pos = __builtin_ctzll(temp);
        if (canflip(near_enemy_pos, color)) {
            ret.emplace_back(near_enemy_pos);
        }
    }
}

int64 Board::get_near_opp(int color) {
    long long ret = 0LL;
    long long blankState = get_blank();
    int enemy_pos = 0;
    for (long long temp = state[color ^ 1]; temp; temp ^= (1LL << enemy_pos)) {
        enemy_pos = __builtin_ctzll(temp);
        ret |= near[enemy_pos] & blankState;
    }
    return ret;
}

void Board::print_table() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (check_pos(0, (j << 3 | i)))
                std::cout << "B ";
            else if (check_pos(1, (j << 3 | i)))
                std::cout << "W ";
            else
                std::cout << "* ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int Board::area_score() {
    //BLACK = 0, WHITE = 1
    return __builtin_popcount(state[0]) - __builtin_popcount(state[1]);
}
