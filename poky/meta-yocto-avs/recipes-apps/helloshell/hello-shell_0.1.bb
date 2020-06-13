#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "shell scripts application for boot"
SECTION = "base"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

APP = "shell.sh"

SRC_URI = "file://scripts \
          "

S = "${WORKDIR}/scripts"

INITSCRIPT_NAME = "shell.sh"
INITSCRIPT_PARAMS = "start 03 S ."

inherit update-rc.d

do_install() {
	install -d ${D}${sysconfdir}/init.d/
	install -m 0755 ${S}/${APP} ${D}${sysconfdir}/init.d/
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
