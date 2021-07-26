# Clean
cmake_dir="build"
if [ -d "$cmake_dir" ]; then
    ans=
    while [ "$ans" != 'y' -a "$ans" != 'n' ]; do
        echo -n "Build directory is present? Do you want to delete it to ensure a clean build? (y/n): "
        read ans
        if [ "$ans" == 'y' ]; then
            echo "Deleting $cmake_dir..."
            rm -rf "$cmake_dir"
        fi
    done
fi

# Get index
echo "Input two-digit index of the release within current day. For first release of today, input \"01\", for second release, input \"02\" and so on."
while [[ ! "$version_index" =~ ^[0-9][0-9]$ ]]; do
    echo -n "Index: "
    read version_index
done

# Run CMake
binary_dir="build/bin"
./build.sh -DBUILD_FOR_PUBLISHING=ON -DVERSION_INDEX=$version_index

# Compile
pushd $cmake_dir >/dev/null 2>&1
if [ `uname -a | grep Linux | wc -l` == 1 ]; then
    extra_args="-- -j `nproc`"
fi
cmake --build . --config Release $extra_args
rm -rf bin/Release # There are gtest_main files  there. TODO: steer CMake to not generate them
rm -rf bin/workloads
rm     bin/docs_generator
popd >/dev/null 2>&1
echo

# Get gta-asset binary
gta_asset_path=`which gta-asset 2>/dev/null`
if [ $? != 0 ]; then
    echo "ERROR: gta-asset not found in PATH"
    exit 1
fi

# Get asset path
if [ `uname -a | grep Linux | wc -l` == 1 ]; then
    asset_path=gfx-ocl-abn-assets-igk/linux
else
    asset_path=gfx-ocl-abn-assets-igk/windows
fi

# Get asset name
asset_name=compute_benchmarks

# Get version
pushd $binary_dir >/dev/null 2>&1
version=$(cat version.txt)
popd >/dev/null 2>&1

# Get user
user=`whoami`

# Get artifactory_url
artifactory_url=https://gfx-assets.igk.intel.com/artifactory

# Print info
echo "Gathered parameters:"
echo "    gta_asset_path=$gta_asset_path"
echo "    asset_path=$asset_path"
echo "    asset_name=$asset_name"
echo "    version=$version"
echo "    binary_dir=$binary_dir"
echo "    user=$user"
echo "    artifactory_url=$artifactory_url"
echo
echo "Contents of binary_dir that will be pushed:"
ls -l $binary_dir | sed 's/^/    /'
echo "Full command to be run:"
echo "$gta_asset_path push $asset_path $asset_name $version $binary_dir --user $user --root-url=$artifactory_url"

ans=
while [ "$ans" != 'y' ]; do
    echo -n "Do you want to proceed? (y/n): "
    read ans
    if [ "$ans" == 'n' ]; then exit 0; fi
done

$gta_asset_path push $asset_path $asset_name $version $binary_dir --user $user --root-url=$artifactory_url
