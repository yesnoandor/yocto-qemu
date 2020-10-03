inherit setuptools

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DESCRIPTION = "Recipe to build the 'nano' editor"

PN = "nano"
PV = "2.2.6"

#SITE = "http://www.nano-editor.org/dist"
#PV_MAJOR = 
#PV_MINOR = 

SRC_URI = "http://www.nano-editor.org/dist/v2.2/nano-2.2.6.tar.gz"
SRC_URI[md5sum] = "03233ae480689a008eb98feb1b599807"
SRC_URI[sha256sum] = "be68e133b5e81df41873d32c517b3e5950770c00fc5f4dd23810cd635abce67a"

python do_fetch() {
    bb.note("Downloading source tarball from ${SRC_URI} ...")	
    src_uri = (d.getVar('SRC_URI', True) or "").split()
    if len(src_uri) == 0:
        bb.fatal("Empty URI")
    try:
        fetch = bb.fetch2.Fetch(src_uri, d)
        fetch.download()
    except bb.fetch2.BBFetchException:
        bb.fatal("Could not fetch source tarball.")

    bb.note("Download successful.")
}
addtask fetch before do_build

#python do_unpack() {
#    bb.note("Unpacking source tarball ...")
#    os.system("tar x -C ${WORKDIR} -f ${DL_DIR}/${P}.tar.gz")
#    bb.note("Unpacked source tarball.")
#}
#addtask unpack before do_configure after do_fetch

do_unpack() {
	tar x -C ${WORKDIR} -f ${DL_DIR}/${P}.tar.gz
}
addtask unpack

#python do_configure() {
#    bb.note("Configuring source package ...")
#    os.system("cd ${WORKDIR}/${P} && ./configure")
#    bb.note("Configured source package.")
#}
#addtask configure before do_build after do_unpack

do_configure() {
	cd ${WORKDIR}/${P} && ./configure -host=x86_64-unknown-linux-gnu
}
addtask configure 

do_compile() {
	cd ${WORKDIR}/${P} && make
}
addtask compile

do_install[nostamp] = "1"
do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${S}/src/nano ${D}${bindir}
}
addtask install


do_clean[nostamp] = "1"
do_clean() {
	rm -rf ${WORKDIR}/*
	rm -rf ${TMPDIR}/stamps/${TARGET_SYS}/${PN}
}
addtask clean
