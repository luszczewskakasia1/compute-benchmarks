<!---
INTEL CONFIDENTIAL

Copyright (C) 2025 Intel Corporation

This software and the related documents are Intel copyrighted materials,
and your use of them is governed by the express license under which they were
provided to you ("License"). Unless the License provides otherwise,
you may not use, modify, copy, publish, distribute, disclose or transmit this
software or the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
-->

# Contribution guidelines
This document is intended for internal usage by Intel Employees.

This document provides a basic set of instructions and rules for contributing to the Compute Benchmarks repository.
It is structured into two sections:
- Contribution process overview - describing pull request (from now on referred to as PR) submission
- Contributing to Compute Benchmarks - outlining processes specific to the Compute Benchmarks project

## Table of Contents

- [1. Contribution process overview](#contribution-overview)
  - [1.1 Branch choice](#branch-choice)
  - [1.2 Workspace setup](#workspace-setup)
  - [1.3 PR creation](#pr-creation)
  - [1.4 Commit message](#commit-message)
  - [1.5 Multi-commit PRs](#multi-commit)
  - [1.6 Certificate of origin](#certificate)
  - [1.7 Copyright headers](#copyright-headers)
  - [1.8 PR verification and code review](#pr-verification)
  - [1.9 PR disposition](#pr-disposition)
- [2. Contributing to Compute Benchmarks](#benchmarks-contributing)
  - [2.1 Adding new benchmarks](#adding-new-benchmark)
  - [2.2 Generating documentation](#benchmarks-docs)

## 1. Contribution process overview <a id="contribution-overview"></a>
### 1.1 Branch choice <a id="branch-choice"></a>
Compute Benchmarks repository consists of two main development branches: `master` and `embargo`.

`master` branch: mirrored to the open-source repository at [intel/compute-benchmarks](https://github.com/intel/compute-benchmarks).
`embargo` branch: `master` branch plus embargo paths (containing internal-only code)

To maintain consistency between `master` and `embargo`, automated cherry-pick system is implemented.

Based on the target branch, one of the following scenarios will occur:

When PR is opened against the `master` branch:
- PR against the `embargo` branch will be automatically opened and labeled as `no-embargo-content`
- PR against the `master` branch will not be modified nor labeled

When PR is opened against the `embargo` branch:
- If PR diff is contained within embargo path(s) only:
  - PR against the `embargo` branch will be labeled as `embargo-only`
  - PR against the `master` branch will not be opened
- If PR diff is contained within open-source path(s) only:
  - PR against the `embargo` branch will be labeled as `no-embargo-content`
  - PR against the `master` branch will be automatically opened
- If PR diff is contained within BOTH open-source and embargo path(s):
  - PR against the `embargo` branch will not be modified nor labeled
  - PR against the `master` will be automatically opened and will undergo a stripping process to remove embargo-only code from the PR. This is done to prevent IP-leaks.

While all of the mentioned scenarios are allowed, it is recommended to open PRs against the `embargo` branch.

#### IMPORTANT!
When introducing or modifying embargo content - please pay most attention to avoid leaking IP to the open-source repository - not only in the source code but also in the commit message!
Please learn more at [master and embargo commit message](#master-embargo-commit-message).

### 1.2 Workspace setup <a id="workspace-setup"></a>

Compute Benchmarks project uses `devtool` (abbreviated as `dt`) with the `gfx` and `gfx-go` extensions to streamline the development process.

To get started with `devtool`, refer to the [Devtool Setup Guide](https://1source.intel.com/docs/getting_started/environment_setup#user-environment-configuration-automatic-devtool).

To enable `gfx` and `gfx-go` extensions, run following commands:
```
dt extensions enable gfx
dt extensions enable gfx-go
```

To download Compute Benchmarks repository using `dt`:
```
dt init workspace compute-benchmarks
```

Remember to keep dt and extensions up-to-date by running:
```
dt update
```

For more examples of `dt` usage, please refer to the [Working with GitHub](https://gpusw-docs.intel.com/services/scm/working_with_github/) page.

### 1.3 PR creation <a id="pr-creation"></a>

To create a PR using `dt`, the following procedure can be used:
```
dt branch my-branch-name *OR* git checkout -b my-branch-name
git add
git commit
dt pr create
```

### 1.4 Commit message <a id="commit-message"></a>

To make project history more readable a specific structure for the commit message is required:

```
Commit title
<BLANK LINE>
Commit body
<BLANK LINE>
Signed-off-by: Example Author example.author@intel.com
```

Where:
* `Commit title` - will become the PR title. It should concisely describe the work introduced by the PR. The maximum title length is 50 characters.
* `Commit body` - will become the PR description. It should justify the work and optionally include additional details, such as references to related JIRA items. The maximum body line length is 80 characters.
* `Signed-off-by` - should be put at the end of the commit message. Learn more [here.](#4-Certificate-of-origin)

####  IMPORTANT! - Master and embargo commit message <a id="master-embargo-commit-message"></a>

Take utmost care when composing commit messages. While the stripping process can prevent IP-leaks in the source code (by removing embargo code from the master PR) - it will NOT prevent potential IP-leaks in the commit message.

If possible - do NOT include any sensitive keywords in the commit message.
Otherwise - the commit message used by master PR can be explicitly specified using `--- master-commit` block in the embargo commit message:

```
Embargo PR commit title

This is the embargo PR commit message.

--- master-commit
This is the master PR commit title.

And this is the master PR commit message.
--- master-commit

Signed-off-by: Example Author example.author@intel.com 
```

Always double-check to ensure your master commit message is properly set afterwards.

### 1.5 Multi-commit PRs <a id="multi-commit"></a>

#### Important!
Do not use force push to update your PR. Please follow this guide instead.

Compute Benchmarks repository allows creating multi-commit PRs. In such cases, Compute Benchmarks uses a squash-and-merge strategy, where the squash commit will use the title and body of the last non-merge and non-revert commit in the PR.

When adding a new commit to PR, the following command can be used to retain your PR title and description:
```
git commit -s --reset-author -c
```
Or:
```
git commit -s --reuse-message HEAD
```
To update the PR title and/or description without changing the diff:
```
git commit -s --allow-empty --reuse-message HEAD --edit
```

### 1.6 Certificate of origin <a id="certificate"></a>

To establish a clear contribution chain of trust
[signed-off-by language](https://developercertificate.org/) is used.
Ensure that your commit message adheres to this guideline by containing the `Signed-off-by:` phrase.

The signature can be added to the commit message by passing short-option `-s` to `git commit`:
```
git commit -s
```

### 1.7 Copyright headers <a id="copyright-headers"></a>

Embargo files must include [the Intel copyright header](LICENSE) at the top.
Open-source files must include [the MIT copyright header](../LICENSE) at the top.

When creating a new file, the year in the copyright header should reflect the year of creation.
```
INTEL CONFIDENTIAL

Copyright (C) 2025 Intel Corporation
```

When modifying an existing file, put the current year next to the year of creation (if different):
```
INTEL CONFIDENTIAL

Copyright (C) 2023-2025 Intel Corporation
```

The above rule applies the same way to files containing the MIT header (when contributing to the master branch).
When open-sourcing files, `dt` will automatically replace the Intel copyright header with the MIT one.

### 1.8 PR verification and code review <a id="pr-verification"></a>

Before submitting a PR, use `clang-format` to properly format the code. You can use:
  - [Visual Studio extension](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).
  - [Visual Studio code extension](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
  - Script for formatting all files - [Windows](scripts/run_clang-format_on_all_files.cmd)/[Linux](scripts/run_clang-format_on_all_files.sh)

After PR is submitted:
- [benchmarks-verify](https://neo-jenkins.igk.intel.com/jenkins/view/Benchmarks/job/benchmarks-verify/) will be triggered to execute all automated checks. In case `verify` result is `failed` - automated mail notification with failure analysis summary will be sent to you.  
IMPORTANT! Before re-triggering a failed `verify` build, visit [this Wiki article](https://wiki.ith.intel.com/pages/viewinfo.action?pageId=4160489664).
- [cherry-pick](https://neo-jenkins.igk.intel.com/jenkins/job/benchmarks-cherry-pick/) will be triggered to create counterpart PR against embargo or master branch. It will be labeled as `empty` for `embargo-only` PRs.
- [spectral scan](https://wiki.ith.intel.com/display/public/ASC/Secrets+Scanning) will scan the diff for any security threats, such as secret leaks.

If `verify` check was successful:
- [owners](https://neo-jenkins.igk.intel.com/jenkins/job/benchmarks-pr-set-approvals/1138//artifact/approvals.html) (including listed PDL members) can now review the code, to ensure it fits within the architecture and design of Compute Benchmarks, is readable and maintainable. Once it's approved, this check will be marked as complete.

Additionally, for PRs targeting the `master` branch.
- [ip-clean](https://wiki.ith.intel.com/display/GPUCompute/github.com+-+IP-Clean+process) check is performed by an *ip-guardian* to confirm that the PR does not introduce any IP-leaks.

Once all checks are completed, a final step called [gating](https://neo-jenkins.igk.intel.com/jenkins/view/Benchmarks/job/benchmarks-manifest-gate/) will be triggered. Your PR will be merged once it completes successfully.

### 1.9 PR disposition <a id="pr-disposition"></a>

We reserve, upon conclusion of the code review, the right to do one of the following:
1. Merge the PR as submitted.
2. Merge the PR (with modifications).
3. Reject the PR.

If merged, you will be listed as the commit author.
Your commit may be reverted if a major regression is identified post-merge.

Your commit will also be scanned by our Coverity instance. If any issues are identified in your code, you will be notified via email. You are required to review and address those issues.
For more information about Coverity tool and scans, visit [Coverity](https://wiki.ith.intel.com/pages/viewpage.action?pageId=3166908161) page on GPUCompute Wiki.

## 2. Contributing to Compute Benchmarks <a id="benchmarks-contributing"></a>

### 2.1 Adding new benchmarks <a id="adding-new-benchmark"></a>
A good way to add new benchmarks is to mimic the existing ones and tweak them to your needs. The general process for adding a brand new test is as follows:
1. Select a binary that suits your benchmark, for example `memory_benchmark`.
2. Choose a name for your benchmark, for example `TwoWayTransfer`.
3. Add a definition file of your benchmark as `source/benchmarks/memory_benchmark/definitions/TwoWayTransfer.h`. This file specifies general information about your test, including its name, description and parameters.
4. Add a test registration file as `source/benchmarks/memory_benchmark/gtest/TwoWayTransfer.cpp`. This file registers your test, so the framework recognizes it and can execute it.
5. Add an implementation file as `source/benchmarks/memory_benchmark/implementations/ocl/TwoWayTransfer_ocl.cpp`. This file contains the actual implementation of your test. Replace *ocl* with *l0* for LevelZero implementation. Each test *can* be implemented in more than one API.
6. Regenerate documentation (see below).

### 2.2 Generating documentation <a id="benchmarks-docs"></a>
Test documentation is generated from the code and stored in the [TESTS.md](TESTS.md) file. Contributors are required to regenerate the documentation by building the `run_docs_generator` target. [TESTS.md](TESTS.md) should be generated with *only* OpenCL and Level Zero enabled - otherwise, the generated file may contain incorrect contents. No further parameters are needed. After generating, include `TESTS.md` as part of the commit.
