Distributions (DIST):
=====================
    MI424WR (BHR Rev-A/C/D)
    MC524WR (BHR Rev-E/F/G)
    FEROCEON (BHR Rev-I/J)

Target Customers (ACTION_TEC_CUSTOMER):
======================================
    ACTION_TEC_VERIZON=y
    ACTION_TEC_FRONTIER=y
    ACTION_TEC_NCS=y

Hardware Revs 
==============
    CONFIG_BHR_REV_E
    CONFIG_BHR_REV_F
    CONFIG_BHR_REV_G
    CONFIG_BHR_REV_I
    CONFIG_BHR_REV_J

Big vs Small Image Compile Option:
==================================
    Big Image: Nothing to be specified on command line
    Small Image: ACTION_TEC_SMALL_IMG=y

License File:
=============
For BHR Rev-E/F/G (Cavium processor based BHR's)
    LIC=../jpkg_actiontec_oct.lic
For BHR Rev-I/J (Marvell processor based BHR's)
    LIC=../jpkg_actiontec_mv.lic


BHR (Big/Regular Image)
-----------------------
make DIST=<distribution> <ACTION_TEC_CUSTOMER=y> <CONFIG_BHR_REV_X=y> [Other CONFIG_RG_OPTIONS ....] LIC=../<license filename> && make

BHR Rev-E/Rev-F:
    Big Image: 
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y CONFIG_IPV6=y ACTION_TEC_IPV6_FIREWALL=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_NCS=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y LIC=../jpkg_actiontec_oct.lic && make
    Small Image: 
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_NCS=y CONFIG_BHR_REV_E=y CONFIG_BHR_REV_F=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make

BHR Rev-G:
    Big Image:
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_G=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_G=y CONFIG_IPV6=y ACTION_TEC_IPV6_FIREWALL=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_G=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_NCS=y CONFIG_BHR_REV_G=y LIC=../jpkg_actiontec_oct.lic && make
    Small Image: 
    make config DIST=MC524WR ACTION_TEC_VERIZON=y CONFIG_BHR_REV_G=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_G=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make
    make config DIST=MC524WR ACTION_TEC_NCS=y CONFIG_BHR_REV_G=y ACTION_TEC_SMALL_IMG=y LIC=../jpkg_actiontec_oct.lic && make

BHR Rev-I:
    make config DIST=FEROCEON ACTION_TEC_VERIZON=y CONFIG_BHR_REV_I=y LIC=../jpkg_actiontec_mv.lic && make
    make config DIST=FEROCEON ACTION_TEC_VERIZON=y CONFIG_BHR_REV_I=y CONFIG_IPV6=y ACTION_TEC_IPV6_FIREWALL=y LIC=../jpkg_actiontec_mv.lic && make
    make config DIST=FEROCEON ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_I=y LIC=../jpkg_actiontec_mv.lic && make
    make config DIST=FEROCEON ACTION_TEC_NCS=y CONFIG_BHR_REV_I=y LIC=../jpkg_actiontec_mv.lic && make

BHR Rev-J:
    make config DIST=FEROCEON ACTION_TEC_VERIZON=y CONFIG_BHR_REV_J=y LIC=../jpkg_actiontec_mv.lic && make
    make config DIST=FEROCEON ACTION_TEC_FRONTIER=y CONFIG_BHR_REV_J=y LIC=../jpkg_actiontec_mv.lic && make
    make config DIST=FEROCEON ACTION_TEC_NCS=y CONFIG_BHR_REV_J=y LIC=../jpkg_actiontec_mv.lic && make

