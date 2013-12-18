PREFIX=opt/
bochs:
	PWD=`pwd`
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-logging --enable-disasm --prefix=/usr/ --with-x11 --with-term --enable-debugger && \
	make -j 4 && sudo make install
