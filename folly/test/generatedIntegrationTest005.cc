#include <gtest/gtest.h>
#include <folly/io/async/AsyncSocket.h>
#include <folly/net/NetworkSocket.h>

class generatedIntegrationTest005 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest005, IntegrationScenario) {
    folly::NetworkSocket sock(folly::NetworkSocket::invalid_handle_value);
    int fd = sock.toFd();
    folly::NetworkSocket sockFromFd = folly::NetworkSocket::fromFd(fd);

    folly::EventBase evb;
    folly::AsyncSocket::UniquePtr asyncSocket(
        new folly::AsyncSocket(&evb, sockFromFd));
}
