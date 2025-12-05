#include <gtest/gtest.h>
#include <folly/io/async/AsyncSocket.h>
#include <folly/io/async/EventBase.h>

class generatedIntegrationTest006 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest006, IntegrationScenario) {
    folly::EventBase eventBase;
    auto socket = folly::AsyncSocket::newSocket(&eventBase);

    eventBase.loopOnce();
}