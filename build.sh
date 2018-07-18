function cplib {
  find ../monero -name $1 -exec cp {} ./deps \;
}
MONERO_ROOT=../monero
mkdir -p $MONERO_ROOT/build
pushd $MONERO_ROOT/build
cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC=ON -DBUILD_GUI_DEPS=ON .. && make -j8 lmdb epee easylogging wallet_merged unbound
popd
mkdir -p deps
cplib libepee.a 
cplib libeasylogging.a
cplib liblmdb.a
cplib libunbound.a
cplib libwallet_merged.a
pushd build
make clean
cmake -DCMAKE_BUILD_TYPE=Debug -DMONERO_ROOT=$MONERO_ROOT .. && VERBOSE=1 make -j8 
popd
