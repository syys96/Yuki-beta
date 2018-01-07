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

#ifndef FASTSTATE_H_INCLUDED
#define FASTSTATE_H_INCLUDED

#include <vector>

#include "FullBoard.h"

class FastState {
public:
    void init_game();
    void reset_game();
    void reset_board();

    void play_pass(void);
    void play_move(int vertex);

    std::vector<int> generate_moves(int color);


    int get_passes() const;
    int get_to_move() const;
    void set_to_move(int tomove);
    void set_passes(int val);
    void increment_passes();

    float final_score();

    size_t get_movenum() const;
    int get_last_move() const;
    int get_prevlast_move() const;
    void display_state();
    std::string move_to_text(int move);

    FullBoard board;

    int m_continous_pass;
    size_t m_movenum;
    std::array<int, 16> m_lastmove;

protected:
    void play_move(int color, int vertex);
};

#endif
