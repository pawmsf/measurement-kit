// Part of measurement-kit <https://measurement-kit.github.io/>.
// Measurement-kit is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
#ifndef MEASUREMENT_KIT_COMMON_NET_TEST_HPP
#define MEASUREMENT_KIT_COMMON_NET_TEST_HPP

#include <cstdint>
#include <measurement_kit/common/callback.hpp>
#include <measurement_kit/common/delegate.hpp>
#include <measurement_kit/common/logger.hpp>
#include <measurement_kit/common/net_test.hpp>
#include <measurement_kit/common/reactor.hpp>
#include <measurement_kit/common/settings.hpp>
#include <string>

namespace mk {

class NetTest {
  public:
    NetTest &on_log(Delegate<uint32_t, const char *> func) {
        logger->on_log(func);
        return *this;
    }
    NetTest &set_verbosity(uint32_t level) {
        logger->set_verbosity(level);
        return *this;
    }
    NetTest &increase_verbosity() {
        logger->increase_verbosity();
        return *this;
    }

    virtual void begin(Callback<> func) {
        // You must override this on subclasses to actually start
        // running the test you're interested to run
        reactor->call_soon(func);
    }
    virtual void end(Callback<> func) {
        // You must override this on subclasses to actually terminate
        // running the test (i.e. send results to collector)
        reactor->call_soon(func);
    }

    unsigned long long identifier() { return (unsigned long long)this; }

    NetTest() {}
    NetTest(Settings o) : options(o) {}
    NetTest(std::string i, Settings o) : options(o), input_filepath(i) {}
    virtual ~NetTest();

    NetTest &set_input_file_path(std::string s) {
        input_filepath = s;
        return *this;
    }
    NetTest &set_output_file_path(std::string s) {
        output_filepath = s;
        return *this;
    }
    NetTest &set_reactor(Var<Reactor> r) {
        reactor = r;
        return *this;
    }
    template <typename T> NetTest &set_options(std::string key, T value) {
        options[key] = value;
        return *this;
    }

    virtual Var<NetTest> create_test_() {
        // You must override this in subclasses to create the actual
        // instance of the test you would like to run on a runner
        return Var<NetTest>{nullptr};
    }

    void run();
    void run(Callback<> func);

    Var<Logger> logger = Logger::make();
    Var<Reactor> reactor = Reactor::global();
    Settings options;
    std::string input_filepath;
    std::string output_filepath;
};

// Note: until we properly unify tests as seen through the DSL (for example,
// DnsInjectionTest) and tests meant to run (DNSInjectionTestImpl), when using
// the DSL style creation it would not be possible to run the tests.
//
// Ideally, in the future, this would change and it would be possible to
// create a test class and either schedule it for running on the runner or
// just directly run it using ->begin() and ->end().
//
// For NDT, it is already like this. For OONI, not yet.
#define MK_DECLARE_TEST_DSL(_name_)                                            \
    class _name_ : public mk::NetTest {                                        \
      public:                                                                  \
        Var<NetTest> create_test_() override;                                  \
      private:                                                                 \
        void begin(Callback<>) override {                                      \
            throw std::runtime_error("not yet implemented");                   \
        }                                                                      \
        void end(Callback<>) override {                                        \
            throw std::runtime_error("not yet implemented");                   \
        }                                                                      \
    };

} // namespace mk
#endif
