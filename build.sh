git config --global http.proxy http://proxy-dmz.intel.com:911
git config --global https.proxy http://proxy-dmz.intel.com:912
export http_proxy="http://proxy-dmz.intel.com:911/"
export https_proxy="http://proxy-dmz.intel.com:912/"

mkdir build -p
cd build
cmake .. $@
cd ..
