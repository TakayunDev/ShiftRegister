#include "Arduino.h"
#include "ShiftRegister.h"

class Impl {
private:
  uint8_t pin[3];
  enum {SER, SCK, RCK}; // 信号, シフト, 出力
  std::map<uint8_t, uint8_t> output_pin;  // 仮想ピンからレジスタピンへの参照
  byte state[32];
  bool drivedOnShiftRegister = false;
  uint8_t byte_num;
  ShiftRegister *sr;
  bool usedSPI = false;
  void subDigitalWrite(uint8_t pin, uint8_t value);
  void flash(uint8_t pin);
  /* 仮想ピンの割り当てを削除
   * <引数>
   * num :uint8_t = 仮想ピン番号
   */
  uint8_t unassign(uint8_t num);
  void setParam();
  void memcopy(byte *to, byte *from, size_t siz);
  void shiftOut(int dataPin, int clockPin, int bitOrder, byte value) {
    if (usedSPI) SPI.transfer(value);
    else ::shiftOut(dataPin, clockPin, bitOrder, value);
  }
public:
  Impl(uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num);
  Impl(uint8_t byte_num, uint8_t ss);
  Impl(ShiftRegister *sr, uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num);
  void push(uint8_t level);
  void init(uint8_t value);
  void show();
  void set(byte data[], size_t siz);
  void set(byte data);
  void assign(uint8_t num, uint8_t origin);
  void assign();
  void assign(const int num[], size_t siz);
  void assign(const int num[8]);
  void assign(int start_num);
  void digitalWrite(uint8_t num, uint8_t value);
  byte getState(uint8_t index);
};

// Private
void Impl::subDigitalWrite(uint8_t pin, uint8_t value) {
  if ( !drivedOnShiftRegister ) {
    ::digitalWrite(pin, value);
  } else {
    sr->digitalWrite(pin, value);
  }
}
void Impl::flash(uint8_t pin) {
  subDigitalWrite(pin, HIGH);
  subDigitalWrite(pin, LOW);
}
uint8_t Impl::unassign(uint8_t num) {
  uint8_t finded = 0;
  auto itr = output_pin.find(num);
  if ( itr != output_pin.end() ) {
    finded = 1;
    output_pin.erase(itr);
  }
  return finded;
}
void Impl::setParam() {
  for (uint8_t i = 0; i < byte_num*8; i++) {
    output_pin[i] = i;
  }
  for (uint8_t i = SER; i < 3; i++) {
    if ( !drivedOnShiftRegister ) pinMode(pin[i], OUTPUT);
    subDigitalWrite(pin[i], LOW);
  }
  init(LOW);
  show();
}
void Impl::memcopy(byte *to, byte *from, size_t siz) {
  for (int i = 0; i < siz*8; i++) to[i] = from[i];
}

// Public
Impl::Impl(uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
: pin{ser, sck, rck}, byte_num(byte_num) {
  setParam();
}
Impl::Impl(uint8_t ss, uint8_t byte_num)
: pin{ss, ss, ss}, byte_num(byte_num) {
  usedSPI = true;
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE2);
  setParam();
}
Impl::Impl(ShiftRegister *sr, uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
: sr(sr), pin{ser, sck, rck}, byte_num(byte_num) {
  drivedOnShiftRegister = true;
  setParam();
}
void Impl::push(uint8_t level) {
  for (int i = byte_num-1; i >= 0; i--) {
    byte add;
    if ( (i-1) >= 0 ) {
      add = (0x80 & state[i-1]);
    } else {
      add = 0x00;
    }
    state[i] = ((byte)state[i] >> 1) | add;
  }
  bitWrite(state[0], 7, level);
  subDigitalWrite(pin[SER], level);
  flash(pin[SCK]);
}
void Impl::init(uint8_t value) {
  for (uint8_t i = 0; i < byte_num*8; i++) push(value);
}
void Impl::show() {
  flash(pin[RCK]);
}
void Impl::set(byte data[], size_t siz) {
  for (int i = siz-1; i >= 0; i--) {
    if (drivedOnShiftRegister) {
      for (int j = 0; j < 8; j++) { push(bitRead(data[i], j)); }
    } else {
      memcopy(state, data, byte_num);
      shiftOut(pin[SER], pin[SCK], LSBFIRST, data[i]);
    }
  }
}
void Impl::set(byte data) {
  set(&data, 1);
}
void Impl::assign(uint8_t num, uint8_t origin) {
  output_pin[num] = origin;
}
void Impl::assign() {
  output_pin.clear();
  for (uint8_t origin = 0; origin < byte_num*8; origin++) {
    output_pin[origin] = origin;
  }
}
void Impl::assign(const int num[], size_t siz) {
  output_pin.clear();
  for (uint8_t origin = 0; origin < siz; origin++) {
    output_pin[num[origin]] = origin;
  }
}
void Impl::assign(const int num[8]) {
  assign(num, 8);
}
void Impl::assign(int start_num) {
  int nums[byte_num*8];
  for (int i = 0; i < byte_num*8; i++) nums[i] = start_num + i;
  assign(nums, byte_num*8);
}
void Impl::digitalWrite(uint8_t num, uint8_t value) {
  byte data[byte_num*8];
  memcopy(data, state, byte_num);
  auto itr = output_pin.find(num);
  if ( itr != output_pin.end() ) {
    uint8_t index = output_pin[num] / 8;
    uint8_t b = (byte_num*8-1-output_pin[num]) % 8;
    bitWrite(data[index], b, value);
    set(data, byte_num);
    show();
  } else {
    subDigitalWrite(num, value);
  }
}
byte Impl::getState(uint8_t index) {
  return state[index];
}

ShiftRegister::ShiftRegister(uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
: impl(new Impl(ser, sck, rck, byte_num)) {}
ShiftRegister::ShiftRegister(uint8_t ss, uint8_t byte_num)
: impl(new Impl(ss, byte_num)) {}
ShiftRegister::ShiftRegister(ShiftRegister *sr, uint8_t ser, uint8_t sck, uint8_t rck, uint8_t byte_num)
: impl(new Impl(sr, ser, sck, rck, byte_num)) {}
void ShiftRegister::push(uint8_t level) { return impl->push(level); }
void ShiftRegister::init(uint8_t value=LOW) { return impl->init(value); }
void ShiftRegister::show() { return impl->show(); }
void ShiftRegister::set(byte data[], size_t siz) { return impl->set(data, siz); }
void ShiftRegister::set(byte data) { return impl->set(data); }
void ShiftRegister::assign(uint8_t num, uint8_t origin) { return impl->assign(num, origin); }
void ShiftRegister::assign() { return impl->assign(); }
void ShiftRegister::assign(const int num[], size_t siz) { return impl->assign(num, siz); }
void ShiftRegister::assign(const int num[8]) { return impl->assign(num); }
void ShiftRegister::assign(int start_num) { return impl->assign(start_num); }
void ShiftRegister::digitalWrite(uint8_t num, uint8_t value) { return impl->digitalWrite(num, value); }
byte ShiftRegister::getState(uint8_t index) { return this->impl->getState(index); }