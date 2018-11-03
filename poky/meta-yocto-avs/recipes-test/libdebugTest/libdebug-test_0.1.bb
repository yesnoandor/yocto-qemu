#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "libdebug test application"
SECTION = "libdebugTest"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += "lib-debug"

SRC_URI = "file://libdebugTest.c \
           file://makefile \
          "

S = "${WORKDIR}"

#FILES_${PN} += "${libdir}/*.so"
#FILES_SOLIBSDEV = ""
#INSANE_SKIP_${PN} += "dev-so"

#INSANE_SKIP_${PN} += "ldflags"
#INHIBIT_PACKAGE_STRIP = "1"
#INHIBIT_SYSROOT_STRIP = "1"
#INHIBIT_PACKAGE_DEBUG_SPLIT  = "1"
RDEPENDS_${PN} += "lib-debug"
#RDEPENDS_SKIP_${PN} += "file-rdeps"

do_compile() {
	make all
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 libdebugTest ${D}${bindir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
