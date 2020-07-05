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


PN = "gsl"
PV = "25.0.0"

LIB = "libgsl"

SRC_URI = "file://src \
		  "

S = "${WORKDIR}/src"

inherit autotools pkgconfig

#FILES_${PN} += "${libdir}/*.so"
#FILES_${PN}-dbg += "${libdir}/.debug"
#FILES_SOLIBSDEV = ""
#INSANE_SKIP_${PN} = "dev-so"


# Task Overrides,Prepends and Appends
do_install() {
	install -d ${D}${libdir}
	cp -f ${WORKDIR}/build/.libs/${LIB}.so.${PV} ${D}${libdir}/
	lnr ${D}${libdir}/${LIB}.so.${PV} ${D}${libdir}/${LIB}.so
	lnr ${D}${libdir}/${LIB}.so.${PV} ${D}${libdir}/${LIB}.so.25
}

do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}

## Variants/Class Extensions
## libdlt.so/libdlt.so.2/libdlt.so.2.18.4 --> sdk
BBCLASSEXTEND = "native nativesdk"

