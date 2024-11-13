#!/bin/sh

# copy files from host to target
cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
#cp $BASE_DIR/../custom-scripts/cod.py $BASE_DIR/target/usr/bin
#cp $BASE_DIR/../custom-scripts/S42sstf $BASE_DIR/target/etc/init.d

# changing permissions
#chmod +x $BASE_DIR/target/usr/bin/cod.py
chmod +x $BASE_DIR/target/etc/init.d/S41network-config
#chmod +x $BASE_DIR/target/etc/init.d/S42sstf

# compiling c code
$BASE_DIR/host/usr/bin/i686-buildroot-linux-uclibc-gcc custom-scripts/deadline.c -o $BASE_DIR/target/usr/bin/deadline
$BASE_DIR/host/usr/bin/i686-buildroot-linux-uclibc-gcc custom-scripts/setpriority.c -o $BASE_DIR/target/usr/bin/setpriority

#Compile simple driver 
make -C $BASE_DIR/../modules/simple_driver/
