#
# This file was derived from the  example recipe in the
# Yocto Project Development Manual.
#


SUMMARY = "This component provides a log and trace interface, based on the standardised protocol specified in the AUTOSAR standard 4.0 DLT "
SECTION = "apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit cmake
#inherit autotools pkgconfig


PN = "dlt-daemon"
PV = "2.18.4"

APP = "dlt-daemon"
LIB = "libdlt"

SRC_URI = "file://src \
		  "

S = "${WORKDIR}/src"


#FILES_${PN} += "${libdir}/*.so"
#FILES_${PN}-dbg += "${libdir}/.debug"
#FILES_SOLIBSDEV = ""
#INSANE_SKIP_${PN} = "dev-so"

# Packaging Meta-data
# RDEPENDS_${PN} += "boost"

# Task Overrides,Prepends and Appends
do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/build/src/daemon/${APP} ${D}${bindir}

	install -d ${D}${libdir}
	mv -f ${WORKDIR}/build/src/lib/${LIB}.so.${PV} ${D}${libdir}/
	lnr ${D}${libdir}/${LIB}.so.2 ${D}${libdir}/${LIB}.so
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}

## Variants/Class Extensions
## libdlt.so/libdlt.so.2/libdlt.so.2.18.4 --> sdk
BBCLASSEXTEND = "native nativesdk"

