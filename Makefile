BUILDDIR=./builddir
LIB=./builddir/dasscc.so
INCLUDE=./include/dasscc.hh ./include/dasscc/*.hh
SRC=./src/main.cc ./src/dasscc/*.cc
MESON_CONF=meson.build
BUILD_TYPE=release

@all: ${LIB}

${BUILDDIR}: ${MESON_CONF}
	meson setup --buildtype=${BUILD_TYPE} ${BUILDDIR}

${LIB}: ${BUILDDIR} ${SRC} ${INCLUDE}
	ninja -j 0 -C ${BUILDDIR}

clean:
	rm -rf ${BUILDDIR}

test:
	meson test -C ${BUILDDIR}

install:
	mkdir -p ${DESTDIR}/usr/local/lib/
	mkdir -p ${DESTDIR}/usr/local/include/
	mkdir -p ${DESTDIR}/usr/local/share/pkgconfig
	mv builddir/libdasscc.so ${DESTDIR}/usr/local/lib/
	cp -r include/* ${DESTDIR}/usr/local/include/
	cp -r dasscc.pc ${DESTDIR}/usr/local/share/pkgconfig

run:
	./builddir/main.exe

docs:
	make -C ./doc
