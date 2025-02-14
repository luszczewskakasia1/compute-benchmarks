#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2025 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.

import logging
from pathlib import Path
import argparse
import typing
import re
import os
import subprocess
import csv
import sys

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
formatter = logging.Formatter(f"{Path(__file__).name} - %(levelname)s - %(message)s")
logger.addHandler(logging.StreamHandler(sys.stdout))
logger.handlers[0].setFormatter(formatter)


def execute(cmd: typing.List[str]) -> subprocess.CompletedProcess:
    logger.debug("Executing command: %s", " ".join(cmd))
    try:
        process = subprocess.run(cmd, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=15)
    except subprocess.CalledProcessError as error:
        logger.error("Error while executing command: %s", error)
        logger.debug("Output: %s", (error.stdout, error.stderr))
        sys.exit(1)
    return process


class BenchmarkData:
    def __init__(self, path: Path, limited_content: bool):
        self.limited_content = limited_content
        self.workload_path: Path = path
        self.command_lines: typing.List[str] = self.get_command_lines()

    @staticmethod
    def get_args_from_parsed_text(cmd_row: str) -> str:
        group = re.match(r"^(.*)\[.*\[", cmd_row)
        if group:
            args = group.group(1).strip()
            return args
        logger.error("Error while parsing command line: %s", cmd_row)
        raise Exception("Error while parsing command line")

    def get_command_lines(self) -> typing.List[str]:
        cases: typing.Set[str] = set()
        output = execute([str(self.workload_path.resolve()), "--noop", "--csv", "--noHeaders", "--noColumnNames"])
        csv_with_class = csv.reader(output.stdout.decode("utf-8").splitlines())

        cmd_line = [
            str(self.workload_path.resolve()),
            "--noop",
            "--csv",
            "--noHeaders",
            "--noColumnNames",
            "--dumpCommandLines",
        ]
        if self.limited_content:
            cmd_line.append("--allowLimitedTests")
        output = execute(cmd_line)
        csv_with_cmds = csv.reader(output.stdout.decode("utf-8").splitlines())
        for row1, row2 in zip(csv_with_class, csv_with_cmds):
            if "NO_IMPLEMENT" in row1[0]:
                logger.info("Skipping case %s due to NO_IMPLEMENT", row2[0])
                continue
            if "NO_SUPPORT (API)" in row1[0]:
                logger.info("Skipping case %s due to NO_SUPPORT (API)", row2[0])
                continue
            if "INVALID_ARGS" in row2[0]:
                logger.error("Test issue: INVALID_ARGS in workload: %s", self.workload_path.name)
                sys.exit(1)
            case_args = BenchmarkData.get_args_from_parsed_text(row2[0])
            binary_name = self.workload_path.stem
            # Support for `BUILD_ALL_API_BINARIES` option
            api_arg_match = re.match(r".*--api=(\w+).*", case_args)
            if api_arg_match:
                api_arg = api_arg_match.group(1)
                if f"_{api_arg}" not in binary_name:
                    binary_name = f"{binary_name}_{api_arg}"
            logger.debug("%s %s", binary_name, case_args)
            cases.add(f"{binary_name} {case_args}")
        logger.debug("Detected cases %s", cases)
        return list(cases)


class Collector:
    def __init__(self, args):
        self.binaries_path: Path = args.binaries_path
        self.limited_content = args.limited
        self.benchmark_list = None
        if args.benchmark_list:
            self.benchmark_list = []
            for item in args.benchmark_list.split(";"):
                benchmark_name = item.strip()
                if item.strip():
                    self.benchmark_list.append(benchmark_name)

    @staticmethod
    def is_binary_exception(file: Path) -> bool:
        return (
            any(file.name.endswith(ext) for ext in [".spv", ".cl", ".txt", ".pdb", ".ilk"])
            or file.is_dir()
            or "workload" in file.name
            or any(
                file.name.startswith(prefix)
                for prefix in ["mutex_comparison", "clflush_comparison", "show_devices", "run_tests_with_cal"]
            )
            or not os.access(file, os.X_OK)
        )

    def collect(self) -> typing.List[BenchmarkData]:
        limited_content_str = "limited" if self.limited_content else "default"
        logger.info("Validating %s content's noop command lines in %s", limited_content_str, self.binaries_path)
        binaries_paths = []
        for path in self.binaries_path.rglob("*"):
            if self.benchmark_list:
                if path.name in self.benchmark_list:
                    binaries_paths.append(BenchmarkData(path, self.limited_content))
                    continue
            elif not Collector.is_binary_exception(path):
                binaries_paths.append(BenchmarkData(path, self.limited_content))
        logger.info("Validated binaries: %s", len(binaries_paths))
        return binaries_paths


def main(args=None) -> None:
    if args is None:
        args = process_command_line()
    collector = Collector(args)
    workloads = collector.collect()
    if args.generate_csv:
        add_workloads_to_csv(workloads, args.output_path, args.limited)


def add_workloads_to_csv(workloads: typing.List[BenchmarkData], output_path: Path, limited_content: bool) -> None:
    class CommandCheck:
        def __init__(self, command_line: str):
            self.command_line = command_line

    command_lines: typing.List[CommandCheck] = []
    if not output_path.exists():
        output_path.mkdir(parents=True, exist_ok=True)
    csv_postfix = "presi" if limited_content else "postsi"
    test_content_csv_path = output_path / f"test_content_{csv_postfix}.csv"

    for workload in workloads:
        for workload_command_line in workload.command_lines:
            if any(workload_command_line == csv_command_check.command_line for csv_command_check in command_lines):
                continue
            logger.info("Adding new command line: %s", workload_command_line)
            command_lines.append(CommandCheck(workload_command_line))
    command_lines.sort(key=lambda x: x.command_line)

    with open(test_content_csv_path, mode="w", newline="", encoding="utf-8") as file:
        fieldnames = ["command_line"]
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        if file.tell() == 0:
            writer.writeheader()
        for command in command_lines:
            writer.writerow({"command_line": command.command_line})


def process_command_line() -> argparse.Namespace:
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    setup_parser(parser)
    return parser.parse_args()


def setup_parser(root_parser: argparse.ArgumentParser) -> None:
    root_parser.add_argument(
        "--binaries_path",
        type=Path,
        default=Path("./build/bin"),
        help="Path to package with binaries",
    )
    root_parser.add_argument(
        "--output_path",
        type=Path,
        default=Path("./source/tools/embargo/test_content/"),
        help="Path to csv with test content",
    )
    root_parser.add_argument(
        "--benchmark_list",
        type=str,
        default=None,
        help="Instead doing an implicit discovery, provide a list of binaries",
    )
    root_parser.add_argument(
        "--limited",
        action="store_true",
        help="Presi limited content",
    )
    root_parser.add_argument(
        "--generate_csv",
        action="store_true",
        help="Generate CSV with test content",
    )


if __name__ == "__main__":
    main()
