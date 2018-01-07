/*
    This file is part of Yuki.
    Copyright (C) 2018 Guofeng Dai

    Yuki is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Yuki is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yuki.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <array>
#include <queue>

#include "config.h"

#include "FastBoard.h"
#include "Utils.h"
#include "Random.h"

using namespace Utils;

const int FastBoard::MAXSQ;
const int FastBoard::BIG;
const int FastBoard::PASS;
const int FastBoard::RESIGN;


int FastBoard::get_boardsize() const {
    return m_boardsize;
}

int FastBoard::get_vertex(int x, int y) const {
    assert(x >= 0 && x < MAXBOARDSIZE);
    assert(y >= 0 && y < MAXBOARDSIZE);
    assert(x >= 0 && x < m_boardsize);
    assert(y >= 0 && y < m_boardsize);

    int vertex = x << 3 | y;

    assert(vertex >= 0 && vertex < m_maxsq);

    return vertex;
}

std::pair<int, int> FastBoard::get_xy(int vertex) const {
    std::pair<int, int> xy;

    //int vertex = x << 3 | y;
    int x = vertex >> 3;
    int y = vertex & 7;

    assert(x >= 0 && x < get_boardsize());
    assert(y >= 0 && y < get_boardsize());

    xy.first  = x;
    xy.second = y;

    assert(get_vertex(x, y) == vertex);

    return xy;
}

FastBoard::square_t FastBoard::get_square(int vertex) const {
    assert(vertex >= 0 && vertex < MAXSQ);
    assert(vertex >= 0 && vertex < m_maxsq);

    if (board.check_pos(BLACK, vertex))
        return BLACK;
    else if (board.check_pos(WHITE, vertex))
        return WHITE;
    else
        return EMPTY;
}

void FastBoard::set_square(int vertex, FastBoard::square_t content) {
    assert(vertex >= 0 && vertex < MAXSQ);
    assert(vertex >= 0 && vertex < m_maxsq);
    assert(content >= BLACK && content <= WHITE);

    if (content == BLACK)
        board.set_pos(BLACK, vertex);
    else if (content == WHITE)
        board.set_pos(WHITE, vertex);
    else {;}

    board.flip(content, vertex);
}

FastBoard::square_t FastBoard::get_square(int x, int y) const {
    return get_square(get_vertex(x,y));
}

void FastBoard::set_square(int x, int y, FastBoard::square_t content) {
    set_square(get_vertex(x, y), content);
}

void FastBoard::reset_board() {
    m_boardsize = BOARD_SIZE;
    m_maxsq = BOARD_SIZE * BOARD_SIZE;
    m_tomove = BLACK;
    board.reset_board();
}




void FastBoard::display_board() {
    board.print_table();
}

bool FastBoard::black_to_move() {
    return m_tomove == BLACK;
}

int FastBoard::get_to_move() {
    return m_tomove;
}

void FastBoard::set_to_move(int tomove) {
    m_tomove = tomove;
}

void FastBoard::gen_nodes(int color, std::vector<int> &ret) {
    board.gen_nodes(color, ret);
}

bool FastBoard::can_flip(int color, int pos) {
    return board.canflip(color, pos);
}

std::string FastBoard::move_to_text(int move) {
    std::ostringstream result;

    int column = move % (m_boardsize + 2);
    int row = move / (m_boardsize + 2);

    column--;
    row--;

    assert(move == FastBoard::PASS || move == FastBoard::RESIGN || (row >= 0 && row < m_boardsize));
    assert(move == FastBoard::PASS || move == FastBoard::RESIGN || (column >= 0 && column < m_boardsize));

    if (move >= 0 && move <= m_maxsq) {
        result << static_cast<char>(column < 8 ? 'A' + column : 'A' + column + 1);
        result << (row + 1);
    } else if (move == FastBoard::PASS) {
        result << "pass";
    } else if (move == FastBoard::RESIGN) {
        result << "resign";
    } else {
        result << "error";
    }

    return result.str();
}

float FastBoard::area_score() {
    return (float)board.area_score();
}

bool FastBoard::check_pos(int color, int vertex) const {
    return board.check_pos(color, vertex);
}

std::string FastBoard::move_to_text_sgf(int move) {
    std::ostringstream result;

    int column = move % (m_boardsize + 2);
    int row = move / (m_boardsize + 2);

    column--;
    row--;

    assert(move == FastBoard::PASS || move == FastBoard::RESIGN || (row >= 0 && row < m_boardsize));
    assert(move == FastBoard::PASS || move == FastBoard::RESIGN || (column >= 0 && column < m_boardsize));

    // SGF inverts rows
    row = m_boardsize - row - 1;

    if (move >= 0 && move <= m_maxsq) {
        if (column <= 25) {
            result << static_cast<char>('a' + column);
        } else {
            result << static_cast<char>('A' + column - 26);
        }
        if (row <= 25) {
            result << static_cast<char>('a' + row);
        } else {
            result << static_cast<char>('A' + row - 26);
        }
    } else if (move == FastBoard::PASS) {
        result << "tt";
    } else if (move == FastBoard::RESIGN) {
        result << "tt";
    } else {
        result << "error";
    }

    return result.str();
}

