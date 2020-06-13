#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "adb tool for linux"
SECTION = "app"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://adb-tools \
          "

S = "${WORKDIR}/adb-tools"

APP = "adb"

DEPENDS += "zlib"
DEPENDS += "openssl"

RDEPENDS_${PN} += "zlib"
RDEPENDS_${PN} += "openssl"

do_compile() {
	cd ${S}/adb && make
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/adb/${APP} ${D}${bindir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
