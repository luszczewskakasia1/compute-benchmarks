REM
REM INTEL CONFIDENTIAL
REM
REM Copyright (C) 2022 Intel Corporation
REM
REM This software and the related documents are Intel copyrighted materials,
REM and your use of them is governed by the express license under which they were
REM provided to you ("License"). Unless the License provides otherwise,
REM you may not use, modify, copy, publish, distribute, disclose or transmit this
REM software or the related documents without Intel's prior written permission.
REM
REM This software and the related documents are provided as is, with no express or
REM implied warranties, other than those that are expressly stated in the License.
REM

@echo off
setlocal enabledelayedexpansion

SET CLANG_FORMAT_BINARY="clang-format.exe"
SET EXTENSIONS_TO_PROCESS=(c, h, cpp, hpp, cxx, hxx, cc, inl)

IF NOT EXIST %CLANG_FORMAT_BINARY% (
  GOTO :FAIL
)


FOR %%e IN %EXTENSIONS_TO_PROCESS% DO (
  ECHO Extension: %%e
  FOR /R ..\source %%f IN (*.%%e) DO (
    %CLANG_FORMAT_BINARY% -i -style=file %%f
  )
)

:EXIT
EXIT /B 0

:FAIL
echo.
echo Script FAILED
EXIT /B 1
