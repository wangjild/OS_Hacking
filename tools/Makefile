bochs:
	tar zxf bochs-2.4.5.tar.gz
	cd bochs-2.4.5 && \
	LDFLAGS='-lpthread'  ./configure --enable-debugger --enable-logging --enable-disasm && \
	make -j 4 && make install
