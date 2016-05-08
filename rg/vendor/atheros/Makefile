RGSRC=../..
include $(RGSRC)/envir.mak

ifeq ($(CONFIG_ATHEROS_OLD_CARRIER),y)
SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_HW_AR5416),pb42_kiwi_rc)
else
SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_HW_AR5416),carrier)
endif
SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_HW_AR5212),madwifi)
SUBDIRS+=$(if $(CONFIG_RG_ATHEROS_ART),art)

JPKG_EXPORTED_DIR+=madwifi pb42_kiwi_rc carrier art

include $(RGMK)
