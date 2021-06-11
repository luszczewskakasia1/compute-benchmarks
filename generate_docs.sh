./build.sh -DGENERATE_DOCS=ON
cd build
make generate_docs -j`nproc`
