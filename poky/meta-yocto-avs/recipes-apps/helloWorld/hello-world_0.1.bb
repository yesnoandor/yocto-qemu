#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "Simple helloworld application"
SECTION = "helloWorld"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://helloWorld.c \
          "

S = "${WORKDIR}"

do_compile() {
	${CC} ${LDFLAGS} helloWorld.c -o helloWorld
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 helloWorld ${D}${bindir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}