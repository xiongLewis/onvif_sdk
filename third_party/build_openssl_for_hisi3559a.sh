platform=hisi3559a
cross=aarch64-himix100-linux-

export ARCH=arm

echo "Beginning Build:"
#rm -rf openssl-1.0.2t
#echo "tar xf openssl-1.0.2t.tar.gz"
#tar xf openssl-1.0.2t.tar.gz
cd openssl-1.0.2t
echo "config openssl-1.0.2t"
./config -fPIC no-shared no-asm --prefix=$(pwd)/../$platform --cross-compile-prefix=$cross
echo "build openssl-1.0.2t"
make clean
make -j12
make install
cd ../../
rm -rf include/openssl
rm -rf lib/$platform

mkdir -p lib/$platform
cp third_party/$platform/include/openssl include/ -rf
cp third_party/$platform/lib/libcrypto.a lib/$platform
cp third_party/$platform/lib/libssl.a lib/$platform