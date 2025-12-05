#include <gtest/gtest.h>
#include <folly/fibers/Baton.h>
// #include <folly/fibers/Waiter.h>

class generatedIntegrationTest008 : public ::testing::Test {
protected:
    void SetUp() override {}
};

class TestWaiter {
 public:
  TestWaiter() : posted_(false) {}
  void post() {
    posted_ = true;
    baton_.post();
  }
  void wait() {
    baton_.wait();
  }
  bool posted() const {
    return posted_;
  }
 private:
  bool posted_;
  folly::fibers::Baton baton_;
};

TEST_F(generatedIntegrationTest008, IntegrationScenario) {
    TestWaiter waiter;
    waiter.post();
    waiter.wait();
}
