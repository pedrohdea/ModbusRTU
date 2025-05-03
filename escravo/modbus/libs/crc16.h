
#ifndef CRC16_H
#define CRC16_H
#define LIBRARY_VERSION_CRC16_H   "0.1.2"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#elif defined(ARDUINO)
  #include "WProgram.h"
#else
  #include <cstdint>
#endif

class Crc16 {
   private:
        uint16_t _msbMask;
        uint16_t _mask;
        uint16_t _xorIn;
        uint16_t _xorOut;
        uint16_t _polynomial;
        uint8_t _reflectIn;
        uint8_t _reflectOut;
        uint16_t _crc;
        uint8_t reflect(uint8_t data);
        uint16_t reflect(uint16_t data);

   public:
        inline Crc16()
        {
            _reflectIn = false;
            _reflectOut = false;
            _polynomial = 0x1021;
            _xorIn = 0x0000;
            _xorOut = 0x0000;
            _msbMask = 0x8000;
            _mask = 0xFFFF;
            _crc = _xorIn;
        }
        inline Crc16(uint8_t reflectIn, uint8_t reflectOut, uint16_t polynomial, uint16_t xorIn, uint16_t xorOut, uint16_t msbMask, uint16_t mask)
        {
            _reflectIn = reflectIn;
            _reflectOut = reflectOut;
            _polynomial = polynomial;
            _xorIn = xorIn;
            _xorOut = xorOut;
            _msbMask = msbMask;
            _mask = mask;
            _crc = _xorIn;
        }
        inline void clearCrc();
        inline void updateCrc(uint8_t data);
        inline uint16_t getCrc();
        inline unsigned int fastCrc(uint8_t data[], uint8_t start, uint16_t length, uint8_t reflectIn, uint8_t reflectOut, uint16_t polynomial, uint16_t xorIn, uint16_t xorOut, uint16_t msbMask, uint16_t mask);
        inline unsigned int XModemCrc(uint8_t data[], uint8_t start, uint16_t length)
        {
            return fastCrc(data, start, length, false, false, 0x1021, 0x0000, 0x0000, 0x8000, 0xffff);
        }
        inline unsigned int Mcrf4XX(uint8_t data[], uint8_t start, uint16_t length)
        {
          return fastCrc(data, start, length, true, true, 0x1021, 0xffff, 0x0000, 0x8000, 0xffff);
        }
        inline unsigned int Modbus(uint8_t data[], uint8_t start, uint16_t length)
        {
          return fastCrc(data, start, length, true, true, 0x8005, 0xffff, 0x0000, 0x8000, 0xffff);
        }
};

void Crc16::clearCrc() {
    _crc = _xorIn;
}

void Crc16::updateCrc(uint8_t data) {
    if (_reflectIn != 0)
        data = (uint8_t) reflect(data);

    int j = 0x80;

    while (j > 0) {
        uint16_t bit = (uint16_t)(_crc & _msbMask);
        _crc <<= 1;
        if ((data & j) != 0)
            bit = (uint16_t)(bit ^ _msbMask);
        if (bit != 0)
            _crc ^= _polynomial;
        j >>= 1;
    }
}

uint16_t Crc16::getCrc() {
    if (_reflectOut != 0)
        _crc = (unsigned int)((reflect(_crc) ^ _xorOut) & _mask);
    return _crc;
}

unsigned int Crc16::fastCrc(uint8_t data[], uint8_t start, uint16_t length, uint8_t reflectIn, uint8_t reflectOut, uint16_t polynomial, uint16_t xorIn, uint16_t xorOut, uint16_t msbMask, uint16_t mask) {
    uint16_t crc = xorIn;
    int j;
    uint8_t c;
    unsigned int bit;

    if (length == 0) return crc;

    for (int i = start; i < (start + length); i++) {
        c = data[i];
        if (reflectIn != 0)
            c = (uint8_t) reflect(c);
        j = 0x80;
        while (j > 0) {
            bit = (unsigned int)(crc & msbMask);
            crc <<= 1;
            if ((c & j) != 0)
                bit = (unsigned int)(bit ^ msbMask);
            if (bit != 0)
                crc ^= polynomial;
            j >>= 1;
        }
    }

    if (reflectOut != 0)
        crc = (unsigned int)((reflect((uint16_t) crc) ^ xorOut) & mask);
    return crc;
}

uint8_t Crc16::reflect(uint8_t data) {
    const uint8_t bits = 8;
    unsigned long reflection = 0x00000000;
    for (uint8_t bit = 0; bit < bits; bit++) {
        if ((data & 0x01) != 0)
            reflection |= (unsigned long)(1 << ((bits - 1) - bit));
        data = (uint8_t)(data >> 1);
    }
    return reflection;
}

uint16_t Crc16::reflect(uint16_t data) {
    const uint8_t bits = 16;
    unsigned long reflection = 0x00000000;
    for (uint8_t bit = 0; bit < bits; bit++) {
        if ((data & 0x01) != 0)
            reflection |= (unsigned long)(1 << ((bits - 1) - bit));
        data = (uint16_t)(data >> 1);
    }
    return reflection;
}

#endif