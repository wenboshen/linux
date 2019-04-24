
make ARCH=x86_64 defconfig
time make ARCH=x86_64 -j4 2>&1 | tee build.log

# using upstream llvm
#export PATH=/home/wenbo/tools/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-18.04/bin:$PATH
#make ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- CLANG_TRIPLE=aarch64-linux-gnu- CC=clang defconfig
#time make ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- CLANG_TRIPLE=aarch64-linux-gnu- HOSTCC=clang CC=clang -j4 2>&1 | tee build.log



# for wllvm
#export LLVM_COMPILER=clang
#time make ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- CLANG_TRIPLE=aarch64-linux-gnu- HOSTCC=clang CC=wllvm -j4 2>&1 | tee build.log

