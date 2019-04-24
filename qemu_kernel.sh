KERNEL_DIR=$PWD
ROOTFS_DIR=/home/wenbo/KERNEL/X86_KERNEL/buildroot-x86.git


qemu-system-x86_64 \
-nographic -smp 8 -m 2048 \
-kernel $KERNEL_DIR/arch/x86/boot/bzImage \
-initrd $ROOTFS_DIR/output/images/rootfs.cpio \
-append "console=ttyS0" 
