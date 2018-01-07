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

#include <assert.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "FastBoard.h"
#include "FastState.h"
#include "Random.h"
#include "Utils.h"
#include "Zobrist.h"
#include "GTP.h"

using namespace Utils;

void FastState::init_game() {
    board.reset_board();

    m_movenum = 0;
    m_continous_pass = 0;
    std::fill(begin(m_lastmove), end(m_lastmove), 0);

    return;
}


void FastState::reset_game(void) {
    reset_board();

    m_movenum = 0;
    m_continous_pass = 0;

    std::fill(begin(m_lastmove), end(m_lastmove), 0);
}

void FastState::reset_board(void) {
    board.reset_board();
}

std::vector<int> FastState::generate_moves(int color) {
    std::vector<int> result;
    board.gen_nodes(color, result);
    if(result.empty())
        result.push_back(FastBoard::PASS);
    return result;
}

void FastState::play_pass(void) {
    m_movenum++;

    std::rotate(rbegin(m_lastmove), rbegin(m_lastmove) + 1, rend(m_lastmove));
    m_lastmove[0] = FastBoard::PASS;

    board.m_hash  ^= 0xABCDABCDABCDABCDULL;
    board.m_tomove = !board.m_tomove;

    board.m_hash ^= Zobrist::zobrist_pass[get_passes()];
    increment_passes();
    board.m_hash ^= Zobrist::zobrist_pass[get_passes()];
}

void FastState::play_move(int vertex) {
    play_move(board.m_tomove, vertex);
}

void FastState::play_move(int color, int vertex) {
    if (vertex != FastBoard::PASS  && vertex != FastBoard::RESIGN) {

        board.update_board(color, vertex);
        std::rotate(rbegin(m_lastmove), rbegin(m_lastmove) + 1,
                    rend(m_lastmove));
        m_lastmove[0] = vertex;

        m_movenum++;

        if (board.m_tomove == color) {
            board.m_hash  ^= 0xABCDABCDABCDABCDULL;
        }
        board.m_tomove = !color;

        if (get_passes() > 0) {
            board.m_hash ^= Zobrist::zobrist_pass[get_passes()];
            set_passes(0);
            board.m_hash ^= Zobrist::zobrist_pass[0];
        }

    } else {
        play_pass();
    }
}

size_t FastState::get_movenum() const {
    return m_movenum;
}


int FastState::get_last_move(void) const {
    return m_lastmove.front();
}

int FastState::get_prevlast_move() const {
    return m_lastmove[1];
}


int FastState::get_to_move() const {
    return board.m_tomove;
}

void FastState::set_to_move(int tom) {
    board.m_tomove = tom;
}

void FastState::display_state() {

    if (board.black_to_move()) {
        myprintf("Black (B) to move");
    } else {
        myprintf("White (W) to move");
    }

    board.display_board();
}

std::string FastState::move_to_text(int move) {
    return board.move_to_text(move);
}

float FastState::final_score() {
    FastState workstate(*this);
    return workstate.board.area_score();
}

int FastState::get_passes() const {
    return m_continous_pass;
}

void FastState::set_passes(int val) {
    m_continous_pass = val;
}

void FastState::increment_passes() {
    m_continous_pass++;
    if (m_continous_pass > 2)
        m_continous_pass = 2;
}
