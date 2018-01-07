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
#include "TimeControl.h"
#include "Utils.h"
#include "GTP.h"

using namespace Utils;

TimeControl::TimeControl(int boardsize, int maintime)
    : m_maintime(maintime) {

    reset_clocks();
    set_boardsize(boardsize);
}

void TimeControl::reset_clocks() {
    m_remaining_time[0] = m_maintime;
    m_remaining_time[1] = m_maintime;
}

void TimeControl::start(int color) {
    m_times[color] = Time();
}

void TimeControl::stop(int color) {
    Time stop;
    int elapsed = Time::timediff(m_times[color], stop);

    assert(elapsed >= 0);

    m_remaining_time[color] -= elapsed;

}

void TimeControl::display_times() {
    {
        int rem = m_remaining_time[0] / 100;  /* centiseconds to seconds */
        int hours = rem / (60 * 60);
        rem = rem % (60 * 60);
        int minutes = rem / 60;
        rem = rem % 60;
        int seconds = rem;
        myprintf("Black time: %02d:%02d:%02d", hours, minutes, seconds);
        myprintf("\n");
    }
    {
        int rem = m_remaining_time[1] / 100;  /* centiseconds to seconds */
        int hours = rem / (60 * 60);
        rem = rem % (60 * 60);
        int minutes = rem / 60;
        rem = rem % 60;
        int seconds = rem;
        myprintf("White time: %02d:%02d:%02d", hours, minutes, seconds);
        myprintf("\n");
    }
    myprintf("\n");
}

int TimeControl::max_time_for_move(int color) {
    /*
        always keep a 1 second margin for net hiccups
    */
    const int BUFFER_CENTISECS = cfg_lagbuffer_cs;

    int timealloc = 0;

    timealloc = (m_remaining_time[color] - BUFFER_CENTISECS)
                / m_moves_expected;

    timealloc = std::max<int>(timealloc, 0);
    return timealloc;
}

void TimeControl::adjust_time(int color, int time) {
    m_remaining_time[color] = time;
}

void TimeControl::set_boardsize(int boardsize) {
    // Note this is constant as we play, so it's fair
    // to underestimate quite a bit.
    m_moves_expected = (boardsize * boardsize) / 5;
}

