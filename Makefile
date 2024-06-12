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
	ninja -C ${BUILDDIR}

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

produce:
	./produce.sh

docs:
	make -C ./doc

report/main.pdf: report/main.tex report/images/*
	make -C report
	cp report/main.pdf ~/Desktop/dasscc.pdf

edit:
	lvim report/main.tex
