SUMMARY = "a dynamic library for debug interface"
SECTION = "lib"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"


SRC_URI = "file://src \
          "

S = "${WORKDIR}/src"

LIB = "libdebug"

# FILES_${PN} : 列出需要打包进入IMAGE的目录或文件 (bitbake.conf中默认只放入lib*.so.*)
FILES_${PN} += "${libdir}/*.so"

# FILES_SOLIBSDEV : 定义动态库软链接的后缀名,默认为so
FILES_SOLIBSDEV = ""

# INSANE_SKIP_${PN} : 禁止某些检查工作, dev-so 动态库检查,  ldflags 链接检查
#INSANE_SKIP_${PN} = "dev-so"
#INSANE_SKIP_${PN} += "ldflags"		


do_compile() {
	make all
}

do_install() {
	install -d ${D}${libdir}/
	install -m 0755 ${S}/${LIB}.so ${D}${libdir}/
 
	install -d ${D}${includedir}
	install -m 0755 *.h ${D}${includedir}
}


do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}

## Variants/Class Extensions
## libdebug.so --> sdk
BBCLASSEXTEND = "native nativesdk"
