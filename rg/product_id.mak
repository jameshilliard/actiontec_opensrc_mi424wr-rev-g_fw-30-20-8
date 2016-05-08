# 
# ACTION_TEC
# This file defines the IDs for Actiontec family of products
# This gets inserted into the EXTERNAL_VERSION in the firmware image
# 

# Default product ID
PRODUCT_ID=0

ifeq ($(CONFIG_BHR_REV_E),y)
PRODUCT_ID=20
SECONDARY_PRODUCT_ID=19
endif
ifeq ($(CONFIG_BHR_REV_F),y)
PRODUCT_ID=20
SECONDARY_PRODUCT_ID=19
endif
ifeq ($(CONFIG_MR1000),y)
PRODUCT_ID=21
SECONDARY_PRODUCT_ID=0
endif
ifeq ($(CONFIG_BHR_REV_G),y)
PRODUCT_ID=30
SECONDARY_PRODUCT_ID=29
endif
ifeq ($(CONFIG_BHR_REV_I),y)
PRODUCT_ID=40
SECONDARY_PRODUCT_ID=0
endif

BUILD_IDENTIFIER=`cat ../CVS/Entries | grep act_changelog | cut -d / -f 3`

export PRODUCT_ID
export SECONDARY_PRODUCT_ID
export BUILD_IDENTIFIER
