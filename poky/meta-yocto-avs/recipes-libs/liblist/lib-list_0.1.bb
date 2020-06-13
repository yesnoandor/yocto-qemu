#
# This file was derived from the 'Hello World!' example recipe in the
# Yocto Project Development Manual.
#

SUMMARY = "a static library for list"
SECTION = "lib"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


SRC_URI = "file://src \
          "

S = "${WORKDIR}/src"

LIB = "liblist"


#PACKAGES = "${PN} ${PN}-dev ${PN}-dbg ${PN}-staticdev"
#RDEPENDS_${PN}-dev = ""
#RDEPENDS_${PN}-dbg = ""
# 设置禁止当前静态包在运行时所依赖的包
RDEPENDS_${PN}-staticdev = ""

do_compile() {
	make all
}

do_install() {
	install -d ${D}${libdir}
	install -m 0755 ${LIB}.a ${D}${libdir}

	install -d ${D}${includedir}
	install -m 0755 *.h ${D}${includedir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
