#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2022 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.
#

echo -n "Machine IP: "
read ip

while IFS= read -r project_file; do
    settings_file=$project_file.user
    benchmark_name=`basename $settings_file | cut -d'.' -f1`

    echo "Setting up $benchmark_name..."

    echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>                                                       "   > $settings_file
    echo "<Project ToolsVersion=\"Current\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\"> "  >> $settings_file
    echo "  <PropertyGroup Condition=\"'\$(Configuration)|\$(Platform)'=='Debug|x64'\">                    "  >> $settings_file
    echo "    <RemoteDebuggerCommand>S:\\a\\Debug\\$benchmark_name.exe</RemoteDebuggerCommand>             "  >> $settings_file
    echo "    <RemoteDebuggerCommandArguments></RemoteDebuggerCommandArguments>                            "  >> $settings_file
    echo "    <RemoteDebuggerWorkingDirectory>S:\\a\\Debug</RemoteDebuggerWorkingDirectory>                "  >> $settings_file
    echo "    <RemoteDebuggerServerName>$ip:4020</RemoteDebuggerServerName>                                "  >> $settings_file
    echo "    <RemoteDebuggerConnection>RemoteWithoutAuthentication</RemoteDebuggerConnection>             "  >> $settings_file
    echo "    <DebuggerFlavor>WindowsRemoteDebugger</DebuggerFlavor>                                       "  >> $settings_file
    echo "  </PropertyGroup>                                                                               "  >> $settings_file
    echo "</Project>                                                                                       "  >> $settings_file
done < <(find ../build/source/ -name "*.vcxproj" | grep benchmarks/.*_benchmark)

echo "Done!"
