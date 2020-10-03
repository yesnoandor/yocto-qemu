#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "Ratchet application for diagnostic"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

APP = "Ratchet"

SRC_URI = "file://bin \
          "

S = "${WORKDIR}/bin"

# DEPENDS : 这个包被构建前必须被构建的包名的空格分割列表,do_build时依赖
#DEPENDS += "dbus"
#DEPENDS += "libpthread-stubs"

# RDEPENDS : 这个包被运行前必须被构建的包名的空格分割列表,do_package时依赖
#RDEPENDS_${PN} += "dbus"
#RDEPENDS_${PN} += "libpthread-stubs"

inherit bin_package

do_install() {
	install -d ${D}${bindir}
	install ${S}/${APP} ${D}${bindir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
