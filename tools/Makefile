PREFIX=opt/
all:
	echo "make bochs or make grub"
bochs:
	PWD=`pwd`
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-debugger --enable-logging --enable-disasm --with-term \
	--prefix=$(PWD)/../$(PREFIX) && \
	make -j 4 && make install
grub:
	PWD=`pwd`
	cd grub-0.97 && ./configure --prefix=$(PWD)/../$(PREFIX) && make && make install
