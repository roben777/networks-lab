#ifndef __SIMPLEHEADER_H
#define __SIMPLEHEADER_H

#include <cstdint>
// maximum size for the payload 
#define DATA_SZ 1024
// size of header
#define HEADER_SZ 4


struct simplepacket {
  uint8_t header[HEADER_SZ];
  char data[DATA_SZ];  // payload
};

// class to be tested. Implements the demo header structure below.
// exports routines to get and set the header parameters val (integer between 0 and 3) and payload_length (16 bit unsigned integer)
//  0 1  2 7   8 15  16   32
// +---+-----+-----+--------+
// |Val| unu | unu |payload |
// |   | sed | sed |length  |
// +---+-----+-----+--------+

class SimpleHeader {
private:
  struct simplepacket packet;

  // start index of the payload length field (private)
  const int PL=2;

  // start index of val header field
  const int VL=0;
  
public:
  // default constructor initializes the header to zero.
  SimpleHeader();

  // sets the value of the payload length
  // val = length; if val > DATA_SZ, the value set is DATA_SZ
  void setPayloadLength(unsigned int val);

  // returns the length of the payload
  unsigned int getPayloadLength() const;

  // returns the val field of the header. Must be between 0..3 since
  // it is a 2 bit value
  unsigned int getVal() const;

  // sets the value field of the header.
  // If the val is not between 0..3, the value set is 0
  void setVal(unsigned int val);

  // returns the size of the packet, including headers and data
  // to be used with recvfrom() or sendto()
  unsigned int totalPacketSize() const {
    return getPayloadLength() + HEADER_SZ;
  }

  // returns pointer to the structure holding the thePacket, including the headers
  // To be used with recvfrom or sendto
  void * thePacket() {
    return &packet;
  }

  void * thePayload() {
    return packet.data;
  }
};

#endif
