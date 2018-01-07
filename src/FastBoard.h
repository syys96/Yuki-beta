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

#ifndef FASTBOARD_H_INCLUDED
#define FASTBOARD_H_INCLUDED

#include "config.h"
#include "board.h"

#include <array>
#include <string>
#include <vector>
#include <queue>

class FastBoard {
    friend class FastState;
public:

    /*
        largest board supported
    */
    static constexpr int MAXBOARDSIZE = BOARD_SIZE;

    static constexpr int MAXSQ = BOARD_SIZE * BOARD_SIZE;
    /*
        infinite score
    */
    static constexpr int BIG = 10000000;

    /*
        vertex of a pass
    */
    static constexpr int PASS   = -1;
    /*
        possible contents of a square
    */

    /*
        vertex of a "resign move"
    */
    static constexpr int RESIGN = -2;

    enum square_t : char {
        BLACK = 0, WHITE = 1, EMPTY = 2, INVAL = 3
    };

    /*
        move generation types
    */
    using movescore_t = std::pair<int, float>;
    using scoredmoves_t = std::vector<movescore_t>;

    int get_boardsize() const;
    square_t get_square(int x, int y) const;
    square_t get_square(int vertex) const ;
    int get_vertex(int i, int j) const;
    void set_square(int x, int y, square_t content);
    void set_square(int vertex, square_t content);
    std::pair<int, int> get_xy(int vertex) const;
    bool check_pos(int color, int vertex) const;

    float area_score();

    bool black_to_move();
    bool can_flip(int color, int pos);
    int get_to_move();
    void set_to_move(int color);
    void gen_nodes(int color, std::vector<int> &ret);

    std::string move_to_text(int move);
    std::string move_to_text_sgf(int move);

    void reset_board();
    void display_board();

protected:

    int m_tomove;
    int m_maxsq;

    int m_boardsize;
private:

    Board board;
};

#endif
