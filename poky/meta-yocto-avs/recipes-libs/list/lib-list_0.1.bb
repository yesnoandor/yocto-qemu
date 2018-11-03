#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "list lib"
SECTION = "list"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

LIB = "liblist.a"

SRC_URI = "file://list.c \
           file://list.h \
           file://makefile \
          "

S = "${WORKDIR}"

PACKAGES = "${PN} ${PN}-dev ${PN}-dbg ${PN}-staticdev"

RDEPENDS_${PN}-staticdev = ""
RDEPENDS_${PN}-dev = ""
RDEPENDS_${PN}-dbg = ""

#FILES_${PN} += "${libdir}/*.so"
#FILES_${PN}-utils += "${bindir}/*"
#FILES_${PN}-staticdev += "${libdir}/*.a"

do_compile() {
	make all
}

do_install() {
	install -d ${D}${libdir}
	install -m 0755 ${LIB} ${D}${libdir}
	install -d ${D}${includedir}
	install -m 0755 *.h ${D}${includedir}
				
#	install -d ${TMPDIR}/sysroots/${MACHINE}/lib/
#	install -m 0755 ${LIB} ${TMPDIR}/sysroots/${MACHINE}/lib/
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
