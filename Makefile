# Variables
BUILD_PATH = $(PWD)/build
IMGFILES_PATH = $(PWD)/imgfiles

BUILD_IMAGE_PATH = $(BUILD_PATH)/image
BUILD_OS_PATH = $(BUILD_PATH)/tskos
BUILD_TSK_PATH = $(BUILD_PATH)/tsk

IMAGE_BIN_PATH = /usr/local/bin

# Targets
default: clean \
	setup \
	build_tsk \
	build_tskos \
	create_img

run:
	qemu-system-x86_64 -net none  -kernel $(IMGFILES_PATH)/vmlinuz -initrd ./build/initrd.img -m 200M -enable-kvm -smp sockets=1,cores=2,threads=2 -append "console=ttyS0" -nographic

# Helper Targets
clean:
	rm -rf build
	make -s -C ./tsk clean

setup:
	mkdir -p build/
	cp -r $(IMGFILES_PATH)/imageraw $(BUILD_IMAGE_PATH)
	mkdir -p $(BUILD_OS_PATH)
	mkdir -p $(BUILD_TSK_PATH)

build_tskos:
	gcc -lncurses ./src/tsk_os.c -o $(BUILD_OS_PATH)/starttsk
	cp $(BUILD_OS_PATH)/starttsk $(BUILD_IMAGE_PATH)$(IMAGE_BIN_PATH)/starttsk

build_tsk:
	make -s -C ./tsk clean
	make -s -C ./tsk release
	cp ./tsk/tsk $(BUILD_IMAGE_PATH)$(IMAGE_BIN_PATH)/tsk

create_img:
	cd $(BUILD_IMAGE_PATH) ; find . -print0 | cpio --null -o --format=newc > $(BUILD_PATH)/initrd.img
