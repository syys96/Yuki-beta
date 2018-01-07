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

#include <cassert>
#include <algorithm>

#include "config.h"

#include "FullBoard.h"
#include "Zobrist.h"
#include "Utils.h"

using namespace Utils;


uint64 FullBoard::calc_hash(void) {
    auto res = uint64{0x1234567887654321ULL};

    for (int i = 0; i < m_maxsq; i++) {
        square_t m_square = get_square(i);
        if (m_square != INVAL) {
            res ^= Zobrist::zobrist[m_square][i];
        }
    }

    if (m_tomove == BLACK) {
        res ^= 0xABCDABCDABCDABCDULL;
    }

    m_hash = res;

    return res;
}

uint64 FullBoard::get_hash(void) const {
    return m_hash;
}


int FullBoard::update_board(const int color, const int i) {
    square_t m_square = get_square(i);
    assert(m_square == EMPTY);

    m_hash ^= Zobrist::zobrist[m_square][i];

    set_square(i, (square_t)color);

    m_square = get_square(i);
    m_hash ^= Zobrist::zobrist[m_square][i];

    return -1;
}

void FullBoard::display_board() {
    FastBoard::display_board();

    myprintf("Hash: %llX\n\n", get_hash());
}

void FullBoard::reset_board() {
    FastBoard::reset_board();

    calc_hash();
}
