#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "Simple Hello World application"
SECTION = "zeromqRecv"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


APP = "zeromqRecv"

DEPENDS += "lib-zmq"

SRC_URI = "file://zeromqRecv.c \
           file://makefile \
          "

S = "${WORKDIR}"
RDEPENDS_${PN} += "lib-zmq"

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
