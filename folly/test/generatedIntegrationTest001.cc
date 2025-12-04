#include <gtest/gtest.h>
#include <folly/io/async/test/SocketPair.h>
#include <folly/net/NetworkSocket.h>

class generatedIntegrationTest001 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest001, IntegrationScenario) {
    folly::SocketPair sp(folly::SocketPair::NONBLOCKING);
    folly::NetworkSocket sock0 = sp.extractNetworkSocket0();
    folly::NetworkSocket sock1 = sp.extractNetworkSocket1();
    int fd0 = sock0.toFd();
    int fd1 = sock1.toFd();
    folly::NetworkSocket sock0_from_fd = folly::NetworkSocket::fromFd(fd0);
    folly::NetworkSocket sock1_from_fd = folly::NetworkSocket::fromFd(fd1);
    bool eq0 = (sock0 == sock0_from_fd);
    bool eq1 = (sock1 == sock1_from_fd);
    (void)eq0;
    (void)eq1;
}