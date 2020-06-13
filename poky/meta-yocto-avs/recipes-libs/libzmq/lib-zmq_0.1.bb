#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "zeromq lib so"
SECTION = "libzmq"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#PN = "zeromqlib"
#PV = "4.2.5"

SRC_URI = "file://src \
          "

S = "${WORKDIR}/src"

FILES_${PN} += "${libdir}/*.so"
FILES_SOLIBSDEV = ""

do_configure() {
	./configure
}
#addtask configure

do_compile() {
    make
}
#addtask compile

do_install() {
    install -d ${D}${libdir}
    install -m 0755 ${S}/src/.libs/*.so* ${D}${libdir}

	install -d ${D}${includedir}
	install -m 0755 ./include/*.h ${D}${includedir}
}
#addtask install

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
