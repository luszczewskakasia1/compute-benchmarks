root_dir=`echo ${BASH_SOURCE[0]} | xargs realpath | xargs dirname | xargs dirname `
find "$root_dir/source" | grep -E "*\.(cpp|inl|h|c|hpp)$" | xargs clang-format -i --verbose

