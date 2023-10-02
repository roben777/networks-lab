#include "SimpleHeader.h"

SimpleHeader::SimpleHeader() {
  // silly code, you can do better
  packet = nullptr;
  size = 0;
}

void SimpleHeader::setBuffer(void * buf, int sz) {
  packet = static_cast<uint8_t*> (buf);
  size = sz;
}

void SimpleHeader::setPayloadLength(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    packet[PL] = (val>>8); // shift the integer to right by 8 bits to get the msb
    packet[PL+1] = (val&255); // bitwise AND with 8 LSB bits set to 1.
  }
}

unsigned int SimpleHeader::getPayloadLength() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return packet[PL+1] | (packet[PL]<<8);
  }
  else {
    return 0;
  }
}

unsigned int SimpleHeader::getVal() const {
  if (packet != nullptr && size > HEADER_SZ) {
    return (packet[VL] >> 6);
  }
  else {
    return 0;
  }
}

 
void SimpleHeader::setVal(unsigned int val) {
  if (packet != nullptr && size > HEADER_SZ) {
    // clear the val first
    packet[VL] &= 0x3f;

    // set the lowest 2 bits of val to the header field, but do not disturb the other bits
    packet[VL] |= (val << 6);
    // OBS: this implementation contains an error. It does not clear the header field if the value is > 3. Add a test for this case, see it fail, then fix the error
  }
}
