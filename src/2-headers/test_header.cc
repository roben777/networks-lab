#include "SimpleHeader.h"
#include "gtest/gtest.h"  // google test framework

#define TEST_BUFFER_SZ 20

class HeaderTest : public testing::Test {
protected:
  SimpleHeader * h_;
  uint8_t * buffer_;

  void SetUp() override {
    h_ = new SimpleHeader;  // create a new class before each test to start fresh
    buffer_ = new uint8_t[TEST_BUFFER_SZ]; // allocate a buffer to test
  }

  void TearDown() override {
    delete h_;
    delete [] buffer_;
  }
};

TEST_F(HeaderTest, setPayloadLength) {
  h_->setBuffer(buffer_, TEST_BUFFER_SZ);  // tell the class to manage the test buffer
  h_->setPayloadLength(0x1234);
  ASSERT_TRUE(buffer_[2] == 0x12);
  ASSERT_TRUE(buffer_[3] == 0x34);
}

TEST_F(HeaderTest, getHeader) {
  buffer_[2] = 0x56;
  buffer_[3] = 0xa2;
  h_->setBuffer(buffer_, TEST_BUFFER_SZ);  // tell the class to manage the test buffer
  ASSERT_TRUE(h_->getPayloadLength() == 0x56a2);
}

TEST_F(HeaderTest, testSetVal) {
  // initialize the first byte of the test buffer to some value.
  buffer_[0] = 0x2e;
  h_->setBuffer(buffer_, TEST_BUFFER_SZ);  // tell the class to manage the test buffer
  h_->setVal(2);
  ASSERT_TRUE(buffer_[0] == 0xae);
}
