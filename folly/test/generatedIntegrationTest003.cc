#include <gtest/gtest.h>
#include <folly/io/async/AsyncPipe.h>
#include <folly/net/NetworkSocket.h>

class generatedIntegrationTest003 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest003, IntegrationScenario) {
    folly::EventBase eventBase;
    int pipeFds[2];
    ::pipe(pipeFds);
    folly::NetworkSocket readSocket = folly::NetworkSocket::fromFd(pipeFds[0]);
    folly::AsyncPipeReader::UniquePtr reader = folly::AsyncPipeReader::newReader(&eventBase, readSocket);
    struct Callback : folly::AsyncReader::ReadCallback {
        void getReadBuffer(void** buf, size_t* len) noexcept override {}
        void readDataAvailable(size_t len) noexcept override {}
        void readEOF() noexcept override {}
        void readErr(const folly::AsyncSocketException& ex) noexcept override {}
    } callback;
    reader->setReadCB(&callback);
    reader->setCloseCallback([](folly::NetworkSocket fd) {
        ::close(fd.toFd());
    });
    const char testData[] = "test";
    ::write(pipeFds[1], testData, sizeof(testData));
    eventBase.loopOnce();
    reader->setReadCB(nullptr);
    ::close(pipeFds[1]);
}