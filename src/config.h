/*
    This file is part of Yuki.

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

#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

/*  Timing code. Define one or none of:
 *
 *  GETTICKCOUNT, GETTIMEOFDAY
 */
#ifdef _WIN32
#define GETTICKCOUNT
#undef HAVE_SELECT
#define NOMINMAX
#else
#define HAVE_SELECT
#define GETTIMEOFDAY
#endif

/* Features */
#define USE_BLAS
#if !defined(__APPLE__) && !defined(__MACOSX)
#define USE_OPENBLAS
#endif
//#define USE_MKL
#define USE_OPENCL
// Use 16-bit floating point storage for net calculations
// #define USE_HALF
//#define USE_TUNER

#define PROGRAM_NAME "Yuki"
#define PROGRAM_VERSION "0.9"

// OpenBLAS limitation
#if defined(USE_BLAS) && defined(USE_OPENBLAS)
#define MAX_CPUS 64
#else
#define MAX_CPUS 128
#endif

/* Integer types */

typedef int int32;
typedef short int16;
typedef signed char int8;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

/* Data type definitions */

#ifdef _WIN32
typedef __int64 int64 ;
typedef unsigned __int64 uint64;
#else
typedef long long int int64 ;
typedef  unsigned long long int uint64;
#endif

#ifdef USE_HALF
#include "half/half.hpp"
using net_t = half_float::half;
#else
using net_t = float;
#endif

#if (_MSC_VER >= 1400) /* VC8+ Disable all deprecation warnings */
    #pragma warning(disable : 4996)
#endif /* VC8+ */

#ifdef GETTICKCOUNT
    typedef int rtime_t;
#else
    #if defined(GETTIMEOFDAY)
        #include <sys/time.h>
        #include <time.h>
        typedef struct timeval rtime_t;
    #else
        typedef time_t rtime_t;
    #endif
#endif

const int BOARD_SIZE = 8;
const int SQUARE_NUM = BOARD_SIZE * BOARD_SIZE;
const int ACTION_N = BOARD_SIZE * BOARD_SIZE;
/*
 * We can't choose to pass in othello if we have other actions.
 */
const int PLANE_SIDE = 8;
const int PLANE_NUM = PLANE_SIDE * 2;
const int INPUT_CHANNEL = PLANE_NUM + 2;
const int DATA_ITEM_NUM = PLANE_NUM + 2;

#endif
