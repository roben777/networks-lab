#include "SimpleHeader.h"
#include "gtest/gtest.h"  // google test framework

class HeaderTest : public testing::Test {
protected:
  SimpleHeader * h_;

  void SetUp() override {
    h_ = new SimpleHeader;  // create a new class before each test to start fresh
  }

  void TearDown() override {
    delete h_;
  }
};

TEST_F(HeaderTest, setPayloadLength) {
  h_->setPayloadLength(0x1234);
  struct simplepacket * ptr = static_cast<struct simplepacket*> (h_->thePacket());
  ASSERT_TRUE(ptr->header[2] == 0x12);
  ASSERT_TRUE(ptr->header[3] == 0x34);
}

TEST_F(HeaderTest, getHeader) {
  struct simplepacket * ptr = static_cast<struct simplepacket*> (h_->thePacket());
  ptr->header[2] = 0x56;
  ptr->header[3] = 0xa2;
  ASSERT_TRUE(h_->getPayloadLength() == 0x56a2);
}

TEST_F(HeaderTest, testSetVal) {
  // get a pointer to the packet.
  struct simplepacket * ptr = static_cast<struct simplepacket*> (h_->thePacket());
  // initialize the first byte to some value.
  ptr->header[0] = 0x2e;
  h_->setVal(2);
  ASSERT_TRUE(ptr->header[0] == 0xae);
}
