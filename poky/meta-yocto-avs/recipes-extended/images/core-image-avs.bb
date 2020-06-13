DESCRIPTION = "A small image with avs system."

#require recipes-core/images/core-image-minimal.bb
require recipes-sato/images/core-image-sato.bb

IMAGE_INSTALL += "sqlite3 mtd-utils coreutils"
IMAGE_FEATURES += "dev-pkgs"

LICENSE = "MIT"
