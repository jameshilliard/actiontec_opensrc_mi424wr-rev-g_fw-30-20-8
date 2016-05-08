RGSRC=../..
include $(RGSRC)/envir.mak


SUBDIRS+=common hostapd
LINK_DIRS+=common hostapd
JPKG_EXPORTED_DIR_SRC+=common hostapd

ifeq ($(CONFIG_ATHEROS_MADWIFI_HOSTAPD),y)
SUBDIRS+=hostapd-0.4.8
LINK_DIRS+=hostapd-0.4.8
JPKG_EXPORTED_DIR_SRC+=hostapd-0.4.8
endif

SUBDIRS+=main
LINK_DIRS+=main
JPKG_EXPORTED_DIR_SRC+=main

include $(RGMK)

