//*****************************************************************************
//*
//* EXDB-Simulator.h
//*   Copyright © 2025-2026 Michael Schaff
//*   All rights reserved.
//*
//* Brief:
//*   ???.
//*
//* Notes:
//*   This is free software: you can redistribute it and/or modify
//*   it under the terms of the GNU General Public License as published by
//*   the Free Software Foundation, either version 3 of the License, or
//*   (at your option) any later version.
//*
//*   It is distributed in the hope that it will be useful,
//*   but WITHOUT ANY WARRANTY; without even the implied warranty of
//*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//*   GNU General Public License for more details.
//*
//*   You should have received a copy of the GNU General Public License
//*   along with the product.  If not, see <https://www.gnu.org/licenses/>.
//*
//*****************************************************************************

#ifndef EXDB_Simulator_h
#define EXDB_Simulator_h

// Command definitions
///////////////////////////////////////////////////////////////////////////////

//            +++------------------------------ Drawbridge Commands
//            |||
//            vvv
#define DCCEX_CMD_STOP        0x41    // 'S'
#define DCCEX_CMD_HOME        0x42    // 'H'
#define DCCEX_CMD_MOVE        0x43    // 'M nnnnnnnnnn'
#define DCCEX_CMD_MANEUVER    0x44    // 'G nnnnnnnnnn'
#define DCCEX_CMD_LOUDNESS    0x45    // 'L nn'
#define DCCEX_CMD_CALIBRATE   0x46    // 'C'

//            +++------------------------------ Drawbridge Requests
//            |||
//            vvv
#define DCCEX_REQ_CVMAP       0x61    // 'P'

//            ++------------------------------- Drawbridge Responses
//            ||
//            vv
#define DCCEX_ST_HEARTBEAT    0x81    // 'Z'
#define DCCEX_ST_ERROR        0x82    // '!'
#define DCCEX_ST_ISBUSY       0x83    // '$'
#define DCCEX_ST_COMPLETE     0x84    // '*'

struct
  {
  BYTE
    cmdCode;

  WORD
    cmdID;

  } _cmdDict[] = {
                  'V', DCCEX_APP_VERSION,
                  '#', DCCEX_APP_RESET,
                  '@', DCCEX_APP_DECODER,
                  '~', DCCEX_APP_CONFIG,
                  '=', DCCEX_APP_COMMIT,
                  '?', DCCEX_APP_STATUS,

                  'U', DCCEX_DEV_UNITTEST,
                  'D', DCCEX_DEV_DEBUG,
                  'A', DCCEX_DEV_TRACE,

                  'S', DCCEX_CMD_STOP,
                  'H', DCCEX_CMD_HOME,
                  'M', DCCEX_CMD_MOVE,
                  'G', DCCEX_CMD_MANEUVER,
                  'L', DCCEX_CMD_LOUDNESS,
                  'C', DCCEX_CMD_CALIBRATE,

                  'P', DCCEX_REQ_CVMAP,

                  'Z', DCCEX_ST_HEARTBEAT,
                  '!', DCCEX_ST_ERROR,
                  '$', DCCEX_ST_ISBUSY,
                  '*', DCCEX_ST_COMPLETE
                 };

#endif
