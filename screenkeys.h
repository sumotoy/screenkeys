/*
 ___  _   _  _ __ ___    ___  | |_  ___   _   _ 
/ __|| | | || '_ ` _ \  / _ \ | __|/ _ \ | | | |
\__ \| |_| || | | | | || (_) || |_| (_) || |_| |
|___/ \__,_||_| |_| |_| \___/  \__|\___/  \__, |
                                          |___/ 
										  
	screenkeys - A Library and Hardware (For Teensy3.x) to drive 1..64 LCD ScreenKeys buttons with 6..7 wires
---------------------------------------------------------------------------------------------------------------------
Version history:
0.1: nothing working, just start coding
---------------------------------------------------------------------------------------------------------------------
		Copyright (c) 2014, s.u.m.o.t.o.y [sumotoy(at)gmail.com]
---------------------------------------------------------------------------------------------------------------------

    screenkeys Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    screenkeys Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

	
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:0.1
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef _SCREENKEYS_XX_H_
#define _SCREENKEYS_XX_H_

#include <inttypes.h>
#include <Arduino.h>

class screenkeys {

public:
	screenkeys( );
	virtual void 	begin(bool protocolInitOverride=false) = 0;
protected:

private:
};

#endif