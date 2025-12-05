#include <gtest/gtest.h>
#include <functional>
#include <cstdint>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <folly/io/async/AsyncSocket.h>
#include <folly/ExceptionWrapper.h>
#include <glog/logging.h>

class TimeoutQueue {
 public:
  typedef int64_t Id;
  typedef std::function<void(Id, int64_t)> Callback;

  TimeoutQueue() : nextId_(1) {}

  Id add(int64_t now, int64_t delay, Callback callback) {
    Event ev;
    ev.id = nextId_++;
    ev.expiration = now + delay;
    ev.repeatInterval = 0;
    ev.callback = std::move(callback);
    timeouts_.insert(ev);
    return ev.id;
  }

  Id addRepeating(int64_t now, int64_t interval, Callback callback) {
    Event ev;
    ev.id = nextId_++;
    ev.expiration = now + interval;
    ev.repeatInterval = interval;
    ev.callback = std::move(callback);
    timeouts_.insert(ev);
    return ev.id;
  }

  bool erase(Id id) {
    auto& idx = timeouts_.get<BY_ID>();
    auto it = idx.find(id);
    if (it == idx.end()) {
      return false;
    }
    idx.erase(it);
    return true;
  }

  int64_t runOnce(int64_t now) { return runInternal(now, true); }
  int64_t runLoop(int64_t now) { return runInternal(now, false); }

  int64_t nextExpiration() const {
    if (timeouts_.empty()) {
      return std::numeric_limits<int64_t>::max();
    }
    auto const& idx = timeouts_.get<BY_EXPIRATION>();
    return idx.begin()->expiration;
  }

 private:
  struct Event {
    Id id;
    int64_t expiration;
    int64_t repeatInterval;
    Callback callback;
  };

  typedef boost::multi_index_container<
      Event,
      boost::multi_index::indexed_by<
          boost::multi_index::ordered_unique<
              boost::multi_index::member<Event, Id, &Event::id>>,
          boost::multi_index::ordered_non_unique<
              boost::multi_index::member<Event, int64_t, &Event::expiration>>>>
      Set;

  enum {
    BY_ID = 0,
    BY_EXPIRATION = 1,
  };

  int64_t runInternal(int64_t now, bool onceOnly) {
    while (true) {
      auto& idx = timeouts_.get<BY_EXPIRATION>();
      auto it = idx.begin();
      if (it == idx.end() || it->expiration > now) {
        break;
      }
      Event ev = *it;
      idx.erase(it);
      ev.callback(ev.id, now);
      if (ev.repeatInterval != 0) {
        ev.expiration = now + ev.repeatInterval;
        timeouts_.insert(ev);
      }
      if (onceOnly) {
        break;
      }
    }
    return nextExpiration();
  }

  Set timeouts_;
  Id nextId_;
};

class ZeroCopyTestAsyncSocket {
 public:
  explicit ZeroCopyTestAsyncSocket(folly::AsyncSocket* sock) : sock_(sock) {}

  void onConnected() {}

  void onDataFinish(folly::exception_wrapper) {}

  void getReadBuffer(void** bufReturn, size_t* lenReturn) {
    *bufReturn = buffer_;
    *lenReturn = sizeof(buffer_);
  }

  void readDataAvailable(size_t len) {
    lastReadLen_ = static_cast<int64_t>(len);
  }

  folly::AsyncSocket* sock_{nullptr};
  char buffer_[1024];
  int64_t lastReadLen_{0};
};

class Callback
      : public folly::AsyncSocket::ReadCallback,
        public folly::AsyncSocket::ConnectCallback {
   public:
    explicit Callback(ZeroCopyTestAsyncSocket* parent) : parent_(parent) {}

    void connectSuccess() noexcept override {
      parent_->sock_->setReadCB(this);
      parent_->onConnected();
    }

    void connectErr(const folly::AsyncSocketException& ex) noexcept override {
      LOG(ERROR) << "Connect error: " << ex.what();
      parent_->onDataFinish(folly::exception_wrapper(ex));
    }

    void getReadBuffer(void** bufReturn, size_t* lenReturn) override {
      parent_->getReadBuffer(bufReturn, lenReturn);
    }

    void readDataAvailable(size_t len) noexcept override {
      parent_->readDataAvailable(len);
    }

    void readEOF() noexcept override {
      parent_->onDataFinish(folly::exception_wrapper());
    }

    void readErr(const folly::AsyncSocketException& ex) noexcept override {
      parent_->onDataFinish(folly::exception_wrapper(ex));
    }

   private:
    ZeroCopyTestAsyncSocket* parent_{nullptr};
};

class generatedIntegrationTest004 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(generatedIntegrationTest004, IntegrationScenario) {
    folly::EventBase evb;
    folly::AsyncSocket::UniquePtr rawSock(new folly::AsyncSocket(&evb));
    ZeroCopyTestAsyncSocket parent(rawSock.get());
    Callback cb(&parent);
    TimeoutQueue tq;
    int64_t now = 0;
    tq.add(now, 5, [&](TimeoutQueue::Id, int64_t firedAt) {
      if (firedAt >= 5) {
        cb.connectSuccess();
      }
    });
    tq.addRepeating(now, 3, [&](TimeoutQueue::Id, int64_t firedAt) {
      if (firedAt >= 9) {
        std::size_t n = 16;
        void* buf = nullptr;
        size_t len = 0;
        cb.getReadBuffer(&buf, &len);
        parent.readDataAvailable(n);
        tq.erase(1);
      }
    });
    now = 5;
    tq.runOnce(now);
    now = 9;
    tq.runLoop(now);
}