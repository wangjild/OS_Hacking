bochs:
	PWD=`pwd`
	tar zxvf bochs-2.4.5.tgz
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-logging --enable-disasm --prefix=/usr/ --with-x11 --with-term --enable-debugger --enable-debugger-gui && \
	make -j 4 && sudo make install
