FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

do_install_append () {
    install -d ${D}/debugfs

    cat >> ${D}${sysconfdir}/fstab <<EOF

# Generated from debugfs-userspace
# debugfs /debugfs debugfs debug 0 0
#debugfs 	/debugfs 	debugfs 	auto 		0 	0
debugfs 　　　　/debugfs             debugfs     defaults,auto                0 0 

EOF
}
