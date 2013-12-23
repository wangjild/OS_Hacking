base:
	PWD=`pwd`
	mkdir -p $(PWD)/../binuntils/

bochs:
	PWD=`pwd`
	tar zxvf bochs-2.4.5.tgz
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-logging --enable-disasm --prefix=$(PWD)/../binutils --with-x11 --with-term --enable-debugger && \
	make -j 4 && sudo make install

grub: grub-pc
	PWD=`pwd`
	tar zxvf grub-0.97.tar.gz
	cd grub-0.97 && ./configure --prefix=$(PWD)/../binutils && make install

grub-pc: base
	PWD=`pwd`
	tar zxvf grub-0.97-i386-pc.tar.gz
	cd grub-0.97-i386-pc && cp -R boot $(PWD)/../
