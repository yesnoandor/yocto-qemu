#
# This file was derived from the  example recipe in the
# Yocto Project Development Manual.
#


SUMMARY = "This repository is a merger of the formerly separate GoogleTest and GoogleMock projects. "
SECTION = "lib"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit cmake


PN = "vsomeip"
PV = "3.1.15"

LIB = "libvsomeip3"

SRC_URI = "file://src \
		  "
S = "${WORKDIR}/src"

# DEPENDS : 这个包被构建前必须被构建的包名的空格分割列表,do_build时依赖
DEPENDS += "boost"

# RDEPENDS : 这个包被运行前必须被构建的包名的空格分割列表,do_package时依赖
RDEPENDS_${PN} += "boost"

# Task Overrides,Prepends and Appends
do_install() {
	install -d ${D}${libdir}

	cp -f ${WORKDIR}/build/libvsomeip3.so.3.1.15 ${D}${libdir}/
	lnr ${D}${libdir}/${LIB}.so.${PV} ${D}${libdir}/${LIB}.so
	lnr ${D}${libdir}/${LIB}.so.${PV} ${D}${libdir}/${LIB}.so.3
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}

## Variants/Class Extensions
## libdlt.so/libdlt.so.2/libdlt.so.2.18.4 --> sdk
BBCLASSEXTEND = "native nativesdk"

