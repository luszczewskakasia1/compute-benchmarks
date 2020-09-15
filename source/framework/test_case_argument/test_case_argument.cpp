#include "test_case_argument.h"

#include "framework/test_case_argument/test_case_arguments.h"

TestCaseArgument::TestCaseArgument(TestCaseArguments &parent, const std::string &key, const std::string &extraHelp)
    : key(key),
      extraHelp(extraHelp) {
    parent.arguments.push_back(this);
}
