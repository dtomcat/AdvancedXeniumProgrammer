/* Custom 4-bit "BitBus"

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

#ifndef XK_BITBUS_
#define XK_BITBUS_

#include <stdint.h>
#include <string>

namespace XK
{
    enum BusMode
    {
        BITBUS_WRITE = 0,
        BITBUS_READ = 1
    };

    enum GPIOMode
    {
        GPIO_IN = 0,
        GPIO_OUT = 1
    };

    class BitBus 
    {
        public:
            BitBus();
            void SetPlatformDelay(bool delay_enabled);

            virtual void DelayMicroseconds(uint64_t usec) = 0;
            virtual std::string GetHardwareString() = 0;
            virtual void SetGPIOMode(GPIOMode gpiomode) = 0;
            virtual void Clock() = 0;;
            virtual void SetBusMode(BusMode busmode) = 0;
            virtual void WriteByte(const uint8_t& data) = 0;
            virtual uint8_t ReadByte() = 0;

        protected:
            bool platform_delay_;
            virtual void InitGPIO() = 0;
    };
}

#endif