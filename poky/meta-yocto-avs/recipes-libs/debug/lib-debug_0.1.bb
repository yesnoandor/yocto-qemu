#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "debug so"
SECTION = "debug"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://debug.c \
		   file://debug.h \
           file://makefile \
          "

S = "${WORKDIR}"



FILES_${PN} += "${libdir}/*.so"
FILES_SOLIBSDEV = ""

#INSANE_SKIP_${PN} += "ldflags"

do_compile() {
	make all
}

do_install() {
	install -d ${D}${libdir}/
	install -m 0755 ${S}/libdebug.so ${D}${libdir}/
	install -d ${D}${includedir}
	install -m 0755 *.h ${D}${includedir}

	#install -d ${TMPDIR}/sysroots/${MACHINE}/lib/
	#install -m 0755 libdebug.so ${TMPDIR}/sysroots/${MACHINE}/lib/
}


do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
