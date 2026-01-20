//*****************************************************************************
//*
//* Config.h
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

// I2C Addresses
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_I2C_ADDRESS       0x6A

#define I2C_CLOCK               100000
#define I2C_BUFFER_SZ           32

// DCC Decoder Configuration
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_PIN_DECODER       2

#define DCCEX_DCC_ADDRESS       0x82

// Decoder Configuration Variable(s)
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_CVMAP             8

#define DCCEX_CV_RESET          0x0A
#define DCCEX_CV_OPEN           0x0B
#define DCCEX_CV_NTRL           0x0C
#define DCCEX_CV_CLSE           0x0D
#define DCCEX_CV_STOP           0x0E

// Serial communication pin configuration
///////////////////////////////////////////////////////////////////////////////
#define SERIAL_BAUD             9600

// ...
///////////////////////////////////////////////////////////////////////////////
#define DCCEX_MEM_EEPROM
//#define DCCEX_MEM_RP2040
//#define DCCEX_MEM_FLASH
