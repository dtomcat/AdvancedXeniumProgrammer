/* RaspberryPI implementation of Custom 4-bit "BitBus"

Copyright (C) 2019-2020 Koos du Preez (kdupreez@hotmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "BitBusPI.hpp"

#include <thread>
#include <chrono>
#include <fstream>

#include "wiringPi.h"
#include "XeniumDefines.h"

namespace XK
{
    BitBusPI::BitBusPI() : BitBus()
    {
        InitGPIO();
    }

    void BitBusPI::DelayMicroseconds(uint64_t usec)
    {
        delayMicroseconds(usec);
    }

    std::string BitBusPI::GetHardwareString()
    {
        std::ifstream vfile("/sys/firmware/devicetree/base/model");
        std::string vstr;

        if (vfile.is_open())
            std::getline(vfile, vstr);
        else
            vstr = "UNKNOWN";

        vfile.close();

        return vstr;
    }

    void  BitBusPI::SetGPIOMode(GPIOMode gpiomode)
    {
        pinMode(BITBUS_D0, gpiomode);
        pinMode(BITBUS_D1, gpiomode);
        pinMode(BITBUS_D2, gpiomode);
        pinMode(BITBUS_D3, gpiomode);
    }

    void  BitBusPI::Clock()
    {

        digitalWrite(BITBUS_CLK, HIGH);
        if (platform_delay_)
        {   
            //ugly way to delay for 3B+.. cause normal thread_this:wait_until aint working!
            auto waituntil = std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(1);
            while(true) if (std::chrono::high_resolution_clock::now() >= waituntil) break;
        }       
        else
        {
// TODO - TEST double clock with 3B+ and remove above ugglyness...
            digitalWrite(BITBUS_CLK, HIGH);
            digitalWrite(BITBUS_CLK, LOW);
        }         
        digitalWrite(BITBUS_CLK, LOW);
    }

    void BitBusPI::SetBusMode(BusMode busmode)
    {
        digitalWrite(BITBUS_DIR, busmode);
        Clock();
    }

    void BitBusPI::WriteByte(const uint8_t& data)
    {
        // Send lower nibble
        digitalWrite(BITBUS_D0, (data & 0x01));
        digitalWrite(BITBUS_D1, (data >> 1 & 0x01));
        digitalWrite(BITBUS_D2, (data >> 2 & 0x01));
        digitalWrite(BITBUS_D3, (data >> 3 & 0x01));
        Clock();

        // Send upper nibble
        digitalWrite(BITBUS_D0, (data >> 4 & 0x01));
        digitalWrite(BITBUS_D1, (data >> 5 & 0x01));
        digitalWrite(BITBUS_D2, (data >> 6 & 0x01));
        digitalWrite(BITBUS_D3, (data >> 7 & 0x01));

        Clock();
    }

    uint8_t BitBusPI::ReadByte()
    {
        uint8_t inbyte = 0x00;

        // receive lower nibble
        Clock();
        inbyte = digitalRead(BITBUS_D0);
        inbyte |= digitalRead(BITBUS_D1) << 1;
        inbyte |= digitalRead(BITBUS_D2) << 2;
        inbyte |= digitalRead(BITBUS_D3) << 3;

        // receive upper nibble
        Clock();
        inbyte |= digitalRead(BITBUS_D0) << 4;
        inbyte |= digitalRead(BITBUS_D1) << 5;
        inbyte |= digitalRead(BITBUS_D2) << 6;
        inbyte |= digitalRead(BITBUS_D3) << 7;

        return inbyte;
    }

    void  BitBusPI::InitGPIO()
    {
        //setup wiringPi
        wiringPiSetupGpio();

        //setup BitBusPI control pins
        pinMode(BITBUS_CLK, OUTPUT);
        pinMode(BITBUS_DIR, OUTPUT);
    }

}

