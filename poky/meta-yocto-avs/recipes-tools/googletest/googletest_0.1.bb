#
# This file was derived from the  example recipe in the
# Yocto Project Development Manual.
#


SUMMARY = "This repository is a merger of the formerly separate GoogleTest and GoogleMock projects. "
SECTION = "lib"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit cmake


#PN = "googletest"
#PV = "2.18.4"

#APP = "dlt-daemon"
#LIB = "libdlt"

SRC_URI = "file://src \
		  "

S = "${WORKDIR}/src"

# 设置禁止当前静态包在运行时所依赖的包
RDEPENDS_${PN}-staticdev = ""

# Task Overrides,Prepends and Appends
do_install() {
	install -d ${D}${libdir}
	install -m 0755 ${WORKDIR}/build/lib/lib*.a ${D}${libdir}
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}

## Variants/Class Extensions
## libdlt.so/libdlt.so.2/libdlt.so.2.18.4 --> sdk
BBCLASSEXTEND = "native nativesdk"

