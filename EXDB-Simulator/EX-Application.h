//*****************************************************************************
//*
//* EX-Application.h
//*   Copyright © 2025-2026 Michael Schaff
//*   All rights reserved.
//*
//* Brief:
//*   ???.
//*
//* Notes:
//*   This is free software: you can redistribute it and/or modify it under
//*   the terms of the GNU General Public License as published by the Free
//*   Software Foundation, either version 3 of the License, or (at your
//*   option) any later version.
//*
//*   It is distributed in the hope that it will be useful, but WITHOUT ANY
//*   WARRANTY; without even the implied warranty of MERCHANTABILITY FITNESS
//*   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
//*   details.
//*
//*   You should have received a copy of the GNU General Public License along
//*   with the product. If not, see <https://www.gnu.org/licenses/>.
//*
//*****************************************************************************

#ifndef EX_Application_h
#define EX_Application_h

#include <Arduino.h>

#include "DCCEX_Delegate.h"
#include "DCCEX_Serial.h"
#include "DCCEX_I2C.h"

// Command definitions
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_UNKNOWN       0x00    // ''

//            +++------------------------------ DCC-EX Application Commands
//            |||
//            vvv
#define DCCEX_APP_VERSION   0x21    // 'V'
#define DCCEX_APP_RESET     0x22    // '# n'
#define DCCEX_APP_DECODER   0x23    // '@ nn nnnn'
#define DCCEX_APP_CONFIG    0x24    // '~ nn nnnnnnnn'
#define DCCEX_APP_COMMIT    0x25    // '='
#define DCCEX_APP_STATUS    0x26    // '?'

//            +++------------------------------ Developer Tools
//            |||
//            vvv
#define DCCEX_DEV_UNITTEST  0xFD    // 'U'
#define DCCEX_DEV_DEBUG     0xFE    // 'D'
#define DCCEX_DEV_TRACE     0xFF    // 'A'

// ...
///////////////////////////////////////////////////////////////////////////////
#include "EXDB-Simulator.h"
#include "EXDB_Cache.h"

#endif
