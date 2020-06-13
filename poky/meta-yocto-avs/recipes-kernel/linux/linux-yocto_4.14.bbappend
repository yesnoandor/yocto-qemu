FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}/:"
SRC_URI += "file://vivid.cfg"

SRC_URI_append = " \
	file://vivid_debug.patch \
"

