#include <gtest/gtest.h>
#include <folly/fibers/FiberManager.h>
#include <folly/fibers/SimpleLoopController.h>

class generatedIntegrationTest009 : public ::testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(generatedIntegrationTest009, IntegrationScenario) {
  folly::fibers::FiberManager manager(
      std::make_unique<folly::fibers::SimpleLoopController>());
  auto& loopController =
      dynamic_cast<folly::fibers::SimpleLoopController&>(manager.loopController());

  bool outerRan = false;
  bool nestedRan = false;

  auto loopFunc = [&]() {
    if (!outerRan) {
      manager.addTask([&]() {
        outerRan = true;
        auto* fm = folly::fibers::FiberManager::getFiberManagerUnsafe();
        ASSERT_NE(fm, nullptr);
        EXPECT_TRUE(fm->hasActiveFiber());

        fm->addTask([&]() { nestedRan = true; });
      });
    } else {
      loopController.stop();
    }
  };

  loopController.loop(std::move(loopFunc));

  EXPECT_TRUE(outerRan);
  EXPECT_TRUE(nestedRan);
}
