base:
	PWD=`pwd`
	mkdir -p $(PWD)/../opt/

bochs:
	PWD=`pwd`
	tar zxvf bochs-2.4.5.tgz
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-logging --enable-disasm --prefix=/usr/ --with-x11 --with-term --enable-debugger && \
	make -j 4 && sudo make install

grub: grub-pc
	PWD=`pwd`
	tar zxvf grub-0.97.tar.gz
	cd grub-0.97 && ./configure --prefix=$(PWD)/../opt && make install
	rm -rf grub-0.97

grub-pc: base
	PWD=`pwd`
	tar zxvf grub-0.97-i386-pc.tar.gz
	cd grub-0.97-i386-pc && cp -R boot $(PWD)/../
	rm -rf grub-0.97-i386-pc
