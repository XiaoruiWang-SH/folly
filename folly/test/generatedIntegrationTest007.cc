#include <gtest/gtest.h>
#include <folly/debugging/symbolizer/Symbolizer.h>
#include <folly/debugging/symbolizer/SymbolizePrinter.h>

class generatedIntegrationTest007 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest007, IntegrationScenario) {
  folly::symbolizer::SafeStackTracePrinter safePrinter(STDERR_FILENO);
  safePrinter.printStackTrace(true);
  safePrinter.flush();
}