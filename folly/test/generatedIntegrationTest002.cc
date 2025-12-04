#include <gtest/gtest.h>
#include "folly/Executor.h"

class generatedIntegrationTest002 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest002, IntegrationScenario) {
    folly::ExecutorBlockingContext ctx1;
    ctx1.forbid = false;
    ctx1.allowTerminationOnBlocking = true;
    ctx1.ex = nullptr;
    ctx1.tag = folly::StringPiece("FirstContext");

    folly::ExecutorBlockingContext ctx2;
    ctx2.forbid = true;
    ctx2.allowTerminationOnBlocking = false;
    ctx2.ex = nullptr;
    ctx2.tag = folly::StringPiece("SecondContext");

    folly::ExecutorBlockingList list1;
    list1.prev = nullptr;
    list1.curr = ctx1;

    folly::ExecutorBlockingList list2;
    list2.prev = &list1;
    list2.curr = ctx2;

    // simulate traversing the blocking list chain starting from list2
    folly::ExecutorBlockingList* current = &list2;
    while (current != nullptr) {
        folly::ExecutorBlockingContext& context = current->curr;
        (void)context.forbid;
        (void)context.allowTerminationOnBlocking;
        (void)context.ex;
        (void)context.tag;
        current = current->prev;
    }
}
