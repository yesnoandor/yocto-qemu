SUMMARY = "a test app for debug dynamic library interface"
SECTION = "app"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

APP = "libdebug-test"

SRC_URI = "file://src \
          "
S = "${WORKDIR}/src"

# Fixed bug : No GNU_HASH in the elf binary: '<file>' [ldflags] 
#TARGET_CC_ARCH += "${LDFLAGS}"

# DEPENDS : 这个包被构建前必须被构建的包名的空格分割列表,do_build时依赖
DEPENDS += "lib-debug"

# RDEPENDS : 这个包被运行前必须被构建的包名的空格分割列表,do_package时依赖
RDEPENDS_${PN} += "lib-debug"

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
