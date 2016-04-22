#ifndef ShiftRegisterDigitalWrite_h
#define ShiftRegisterDigitalWrite_h

#include "ShiftRegister.h"

#define ShiftRegisterDigitalWrite(ser, sck, rck, byte_num) ShiftRegister __shiftRegister(ser, sck, rck, byte_num)
#define ShiftRegisterAssign(start_num) __shiftRegister.assign(start_num)

#undef digitalWrite
#define digitalWrite(num, value) __shiftRegister.digitalWrite(num, value)

#endif