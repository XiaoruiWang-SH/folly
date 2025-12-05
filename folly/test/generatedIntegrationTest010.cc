#include <gtest/gtest.h>
#include <folly/io/ShutdownSocketSet.h>
#include <folly/File.h>
#include <folly/net/NetworkSocket.h>
#include <sys/socket.h>
#include <unistd.h>

class generatedIntegrationTest010 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest010, IntegrationScenario) {
    folly::ShutdownSocketSet socketSet;

    int sv[2];
    ASSERT_EQ(0, ::socketpair(AF_UNIX, SOCK_STREAM, 0, sv));

    folly::NetworkSocket s0(sv[0]);
    folly::NetworkSocket s1(sv[1]);

    socketSet.add(s0);
    socketSet.add(s1);

    // Use duplicated descriptors with folly::File so that ShutdownSocketSet
    // remains the sole owner of s0/s1.
    folly::File fileRead(::dup(sv[0]), true);
    folly::File fileWrite(::dup(sv[1]), true);

    socketSet.shutdown(s0, true);

    socketSet.close(s0);
    socketSet.close(s1);
}
