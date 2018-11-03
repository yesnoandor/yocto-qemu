#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "liblist test application"
SECTION = "liblistTest"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

APP = "liblistTest"

DEPENDS += "lib-list"

SRC_URI = "file://liblistTest.c \
           file://makefile \
          "

S = "${WORKDIR}"

do_compile() {
	make all
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${APP} ${D}${bindir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
