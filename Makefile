ARCH?=x86_64
IOS_PLATFORM?=SIMULATOR64

BOOST_VERSION=1.66.0
OPENSSL_VERSION=1.0
MONERO_BRANCH?=release-v0.12-multisig-wallet-assembled
MONERO_BUILD_TYPE?=release
PWD=$(shell pwd)
BOOST="${PWD}/boost-${ARCH}/include"
BOOST_LIB="${PWD}/boost-lipo"
OPENSSL="${PWD}/openssl-lipo"
BUILD_FLAGS?=-DIOS_PLATFORM=${IOS_PLATFORM}
MONERO_TAR=monero-ios-${MONERO_BUILD_TYPE}-lipo.tar.gz

S3_BUCKET=monero-nodejs-addon
S3_PATH=s3://${S3_BUCKET}/monero/ios/${MONERO_BRANCH}
BOOST_URL=https://monero-nodejs-addon.s3.amazonaws.com/boost/ios/${BOOST_VERSION}/lipo/boost-ios-lipo.tar.gz
OPENSSL_URL=https://monero-nodejs-addon.s3.amazonaws.com/openssl/ios/${OPENSSL_VERSION}/lipo/openssl-ios-lipo.tar.gz
MONERO_URL=https://monero-nodejs-addon.s3.amazonaws.com/monero/ios/${MONERO_BRANCH}/lipo/${MONERO_TAR}


.PHONY: all
all: build/${ARCH}

boost-lipo:
	rm -rf boost-lipo && mkdir -p boost-lipo
	curl -s -L -o boost-lipo/boost-ios-lipo.tar.gz ${BOOST_URL}
	cd boost-lipo && tar xf boost-ios-lipo.tar.gz

openssl-lipo:
	rm -rf openssl-lipo && mkdir -p openssl-lipo
	curl -s -L -o openssl-lipo/openssl-ios-lipo.tar.gz ${OPENSSL_URL}
	cd openssl-lipo && tar xf openssl-ios-lipo.tar.gz

monero-lipo: 
	rm -rf monero-lipo && mkdir -p monero-lipo
	curl -s -L -o monero-lipo/${MONERO_TAR} ${MONERO_URL}
	cd monero-lipo && tar xf ${MONERO_TAR}

build/${ARCH}: monero-lipo boost-lipo openssl-lipo
	mkdir -p build/${ARCH}
	cd build/${ARCH} && cmake ${BUILD_FLAGS} -DBOOST_DEBUG=1 -D BOOST_IGNORE_SYSTEM_PATHS=ON -D EMBEDDED_WALLET=ON \
		-D USE_DEVICE_LEDGER=0 -D BUILD_TESTS=OFF -D ARCH=${ARCH} -D STATIC=ON -D BUILD_64=ON \
		-D CMAKE_BUILD_TYPE=${MONERO_BUILD_TYPE} -D IOS=true -D BUILD_TAG="ios" -D BOOST_INCLUDEDIR=${BOOST} \
		-D BOOST_LIBRARYDIR=${BOOST_LIB} -D OPENSSL_ROOT_DIR=${OPENSSL} \
		-D MONERO_INCLUDEDIR=${PWD}/monero-lipo/include -D MONERO_LIBRARYDIR=${PWD}/monero-lipo \
		-D OPENSSL_LIBRARYDIR=${PWD}/openssl-lipo \
		-D CMAKE_POSITION_INDEPENDENT_CODE:BOOL=true -D EMBEDDED_WALLET=ON ../..	
	cd build/${ARCH} && make -j8 VERBOSE=1

