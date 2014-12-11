#include "ooni/net_test.hpp"
#include "common/log.h"
#include <ctime>

using namespace ight::ooni::net_test;

NetTest::NetTest(std::string input_filepath_, ight::common::Settings options_)
  : input_filepath(input_filepath_), options(options_)
{
  std::string report_file = "example_test_report.yaml";
  std::string test_name = "light-meter";
  std::string test_version = "0.0.1";
  std::string probe_ip = "127.0.0.1";

  time_t start_time = time(0);
  std::map<std::string, std::string> options;
  options["opt1"] = "value1";
  options["opt2"] = "value2";
  
  file_report.test_name = test_name;
  file_report.test_version = test_version;
  file_report.start_time = start_time;
  file_report.probe_ip = probe_ip;
  file_report.options = options;
  file_report.filename = report_file;
}

NetTest::NetTest(void) : NetTest::NetTest("", ight::common::Settings())
{
  // nothing
}

NetTest::NetTest(std::string input_filepath_) : 
  NetTest::NetTest(input_filepath_, ight::common::Settings())
{
  // nothing
}

InputFileIterator
NetTest::input_file()
{
  return InputFileIterator(input_filepath);
}

void
NetTest::geoip_lookup()
{
  // TODO: implement
}

void
NetTest::run_next_measurement(std::function<void()>&& cb)
{
  ight_debug("Running next measurement");
  if (input == input.end()) {
    ight_debug("Reached the end of the input");
    cb();
    return;
  }
  entry = ReportEntry(*input);
  ight_debug("Calling setup");
  setup();
  ight_debug("Running with input %s", (*input).c_str());
  main(*input, options, [&](ReportEntry entry) {
      ight_debug("Tearing down");
      teardown();
      file_report.writeEntry(entry);
      ight_debug("Written entry");
      ++input;
      run_next_measurement(std::move(cb));
  }); 
}

void
NetTest::begin(std::function<void()>&& cb)
{
  geoip_lookup();
  write_header();
  if (input_filepath != ""){
    ight_debug("Found input file");
    input = input_file();
    run_next_measurement(std::move(cb));
  } else {
    ight_debug("No input file");
    entry = ReportEntry();
    setup();
    main(options, [=](ReportEntry entry) {
      teardown();
      file_report.writeEntry(entry);
      cb();
    });
  }
}

void
NetTest::write_header()
{
  file_report.open();
}

void
NetTest::end(std::function<void()>&& cb)
{
  file_report.close();
  cb();
}

void
NetTest::setup(std::string) {
};

void
NetTest::setup() {
};

void
NetTest::teardown(std::string) {
};

void
NetTest::teardown() {
};

void
NetTest::main(ight::common::Settings,
              std::function<void(ReportEntry)>&& cb) {
  delayed_call = IghtDelayedCall(1.25, [=](void) {
    ReportEntry entry;
    cb(entry);
  });
}

void
NetTest::main(std::string, ight::common::Settings,
              std::function<void(ReportEntry)>&& cb) {
  delayed_call = IghtDelayedCall(1.25, [=](void) {
    ReportEntry entry;
    cb(entry);
  });
}
