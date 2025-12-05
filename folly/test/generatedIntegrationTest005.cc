#include <gtest/gtest.h>
#include <folly/io/RecordIO.h>
#include <folly/File.h>
#include <folly/io/IOBuf.h>

class generatedIntegrationTest005 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest005, IntegrationScenario) {
    folly::File file("test_recordio_file", O_CREAT | O_RDWR | O_TRUNC, 0644);
    folly::RecordIOWriter writer(std::move(file), 42);
    auto buf = folly::IOBuf::copyBuffer("integration test data");
    writer.write(std::move(buf));
    writer.filePos();
}