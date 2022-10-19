#include "SimpleHeader.h"

SimpleHeader::SimpleHeader() {
  // silly code, you can do better
  packet.header[0] = packet.header[1] = packet.header[2] = packet.header[3] = 0;
}

void SimpleHeader::setPayloadLength(unsigned int val) {
  packet.header[PL] = (val>>8); // shift the integer to right by 8 bits to get the msb
  packet.header[PL+1] = (val&255); // bitwise AND with 8 LSB bits set to 1.
}

unsigned int SimpleHeader::getPayloadLength() const {
  return packet.header[PL+1] | (packet.header[PL]<<8);
}

unsigned int SimpleHeader::getVal() const {
  return (packet.header[VL] >> 6);
}

 
void SimpleHeader::setVal(unsigned int val) {
  // clear the val first
  packet.header[VL] &= 0x3f;

  // set the lowest 2 bits of val to the header field, but do not disturb the other bits
  packet.header[VL] |= (val << 6);
  // OBS: this implementation contains an error. It does not clear the header field if the value is > 3. Add a test for this case, see it fail, then fix the error
}
