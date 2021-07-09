#include "framework/benchmark_info.h"
#include "framework/configuration.h"
#include "framework/gtest_event_listener.h"
#include "framework/print_device_info.h"
#include "framework/test_map.h"
#include "framework/utility/common_help_message.h"
#include "framework/utility/file_helper.h"
#include "framework/utility/string_utils.h"

#include <gtest/gtest.h>
#include <iostream>

int printVersion(bool enableWarning, const char *prefix = "") {
    const std::string version = BENCHMARK_VERSION;
    if (!version.empty()) {
        std::cout << prefix << BENCHMARK_VERSION << std::endl;
        return 0;
    }

    if (enableWarning) {
        std::cerr << "Unknown version. Run CMake with \"-D INCLUDE_VERSION=ON\" to include it in the binary." << std::endl;
        return 1;
    }

    return 0;
}

int generateDocs(const std::string &docsFileName) {
    FileHelper::FileOrConsole fileOrConsole{docsFileName, std::ios::app, std::cout};
    std::ostream &file = fileOrConsole.get();

    file << "# " << BenchmarkInfo::get().getBenchmarkName() << '\n';
    file << BenchmarkInfo::get().getBenchmarkDescription() << '\n';
    file << "| Test name | Description | Params | L0 | OCL |\n";
    file << "|-----------|-------------|--------|----|-----|\n";
    for (const auto &testEntry : TestMap::get()) {
        const std::unique_ptr<TestCaseInterface> &testCase = testEntry.second;

        file << testCase->getTestCaseName() << '|';
        file << testCase->getHelp() << '|';
        file << "<ul>";
        const std::unique_ptr<ArgumentContainer> arguments = testCase->getArguments();
        for (const Argument *argument : arguments->getArguments()) {
            file << "<li>" << argument->getHelp() << "</li>";
        }
        file << "</ul>|";
        file << (testCase->isApiImplemented(Api::L0) ? ":heavy_check_mark:" : ":x:") << '|';
        file << (testCase->isApiImplemented(Api::OpenCL) ? ":heavy_check_mark:" : ":x:") << '|';
        file << '\n';
    }
    file << "\n\n\n";

    if (fileOrConsole.hasOwnedFile()) {
        std::cout << BenchmarkInfo::get().getBenchmarkName() << ": Generated documentation to " << docsFileName << '\n';
    }

    return 0;
}

int executeSingleTest(const std::string &testName, CommandLineArguments &commandLineArguments) {
    if (!Configuration::get().noHeaders) {
        DeviceInfo::printDeviceInfo();
        printVersion(false, "Benchmark version: ");
    }

    const auto &testMap = TestMap::get();
    auto it = testMap.find(testName);
    if (it == testMap.end()) {
        std::cerr << "Unknown test case\n";
        return 1;
    }

    TestCaseInterface *testCase = it->second.get();
    replaceGtestListener<SingleTestGtestListener>();
    if (!testCase->runFromCommandLine(commandLineArguments)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }
    return 0;
}

int executeAllTests(CommandLineArguments &commandLineArguments) {
    if (!Configuration::get().noHeaders) {
        DeviceInfo::printDeviceInfo();
        printVersion(false, "Benchmark version: ");
    }

    for (auto &commandLineArgument : commandLineArguments) {
        if (commandLineArgument.getKey().find("gtest_") == 0) {
            commandLineArgument.markAsProcessed();
        }
    }

    if (const auto unprocessedArgs = CommandLineArgument::getUnprocessedArguments(commandLineArguments); !unprocessedArgs.empty()) {
        const auto getKey = +[](const CommandLineArgument *a) { return a->getKey(); };
        std::cerr << CommonHelpMessage::errorIgnoredCommandLineArgs() << joinStrings(", ", unprocessedArgs, getKey) << std::endl;
        return 1;
    }

    replaceGtestListener<AllTestsGtestListener>();
    return RUN_ALL_TESTS();
}

int printHelp() {
    const auto filename = BenchmarkInfo::get().getBenchmarkFilename();
    // clang-format off
    std::cout << BenchmarkInfo::get().getBenchmarkDescription() << "\n"
                 "\n"
                 "The benchmark works in two modes - all-tests mode and single-test mode. They are further described below. "
                 "Global parameters applicable for both modes:\n"
                 << Configuration::get().getHelp(1u) << "\n"
                 "\n"
                 "First mode is the default and it runs all available benchmarks in many predefined configurations. Underlying test engine "
                 "is googletest, so standard googletest arguments like --gtest_filter can be used, if necessary.\n"
                 "\n"
                 "Second mode runs one specific benchmark with custom parameter values. Running benchmarks in this fashion requires "
                 "using --test argument, along with benchmark-specific parameters. All parameters have to be specified, there are no "
                 "default values.\n"
                 "\n"
                 "Example invocations:\n"
                 "\t" << filename << "                                                runs all possible tests\n"
                 "\t" << filename << " --api=ocl                                      runs all possible OpenCL tests\n"
                 "\t" << filename << " --iterations=100 --csv                         runs all possible tests with 100 iterations and dumps results as CSV\n"
                 "\t" << filename << " --gtest_filter=<regex>                         runs all tests matching a regular expression\n"
                 "\t" << filename << " --gtest_filter=*TestName*                      runs a test named \"TestName\" in all predefined configurations\n"
                 "\t" << filename << " --test=TestName --someParam=1 --otherParam=30  runs a test named \"TestName\" with specified parameters\n"
                 "\n"
                "All available test cases with their parameters:\n";
    // clang-format on
    for (const auto &entry : TestMap::get()) {
        const TestCaseInterface &testCase = *entry.second.get();
        const std::vector<Api> apis = testCase.getApisWithImplementation();
        if (apis.size() == 0) {
            continue;
        }

        std::cout << '\t' << testCase.getTestCaseName() << " - " << testCase.getHelp();
        std::cout << " Supported compute APIs: " << joinStrings(", ", apis, getUserFriendlyApiName) << ".";

        const std::string helpParameters = testCase.getHelpParameters();
        if (helpParameters.size() != 0) {
            std::cout << " Parameters:\n"
                      << helpParameters;
        } else {
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    return 0;
}

int main(int argc, char **argv) {
    BenchmarkInfo::set(new BenchmarkInfoImpl());

    CommandLineArguments commandLineArguments = {};
    std::string commandLineArgumentsParsingErrors = {};
    if (!CommandLineArgument::parseArguments(argc, argv, commandLineArguments, commandLineArgumentsParsingErrors)) {
        std::cerr << commandLineArgumentsParsingErrors << std::endl;
        return 1;
    }

    if (!Configuration::parseArgumentsForConfiguration(commandLineArguments)) {
        std::cerr << "Error parsing command line\n";
        return 1;
    }

    const Configuration &configuration = Configuration::get();
    if (configuration.generateDocs) {
        return generateDocs(configuration.generateDocsPath);
    }
    if (configuration.hwInfo) {
        DeviceInfo::printAvailableDevices();
        return 0;
    }

    for (auto &argument : commandLineArguments) {
        if (argument.isKeyEqualTo("test")) {
            argument.markAsProcessed();
            return executeSingleTest(argument.getValue(), commandLineArguments);
        }
        if (argument.isKeyEqualTo("help")) {
            return printHelp();
        }
        if (argument.isKeyEqualTo("version")) {
            return printVersion(true);
        }
    }

    ::testing::InitGoogleTest(&argc, argv);
    return executeAllTests(commandLineArguments);
}
