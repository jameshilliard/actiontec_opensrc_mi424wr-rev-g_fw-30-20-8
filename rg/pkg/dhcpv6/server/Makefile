RGSRC=../../../
include $(RGSRC)/envir.mak

TARGET 		+= 	dhcp6s
O_OBJS_dhcp6s 		+= 	dhcp6s.o
O_OBJS_dhcp6s 		+= 	../common/common.o
O_OBJS_dhcp6s 		+= 	../common/if.o
O_OBJS_dhcp6s 		+= 	../common/config.o
O_OBJS_dhcp6s 		+= 	../common/timer.o
O_OBJS_dhcp6s 		+= 	../common/lease.o
O_OBJS_dhcp6s 		+= 	../common/base64.o
O_OBJS_dhcp6s 		+= 	../common/auth.o
O_OBJS_dhcp6s 		+= 	../common/dhcp6_ctl.o
O_OBJS_dhcp6s 		+= 	../common/cfparse.o
O_OBJS_dhcp6s 		+= 	../common/cftoken.o
O_OBJS_dhcp6s 		+= 	../missing/ifaddrs_linux.o
O_OBJS_dhcp6s 		+= 	../missing/bcmp.o

CFLAGS		+=	-I../common/ -I../missing/ -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DYYTEXT_POINTER=1 -DHAVE_GETADDRINFO=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GETIFADDRS=1 -DHAVE_IF_NAMETOINDEX=1 -DHAVE_DAEMON=1 -DHAVE_WARNX=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYSLOG_H=1 -DHAVE_UNISTD_H=1 -DHAVE_IFADDRS_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_STRUCT_TM_TM_ZONE=1 -DHAVE_TM_ZONE=1 -DHAVE_SIG_ATOMIC_T=1 -DGETPGRP_VOID=1 -DSETPGRP_VOID=1 -DRETSIGTYPE=void -DHAVE_MKTIME=1 -DHAVE_SELECT=1 -DHAVE_SOCKET=1 -DHAVE_ANSI_FUNC=1 -DHAVE_TAILQ_FOREACH_REVERSE_OLD=1 -DHAVE_STDARG_H=1  -DSYSCONFDIR=\"/etc\" -DLOCALDBDIR=\"/var\"

# to support hierarchical prefix delegation, comment out the following
ifneq ($(CONFIG_IOT_ROUTERLIFETIME),)
CFLAGS += -I../../ctl_layer/tr69fw/framework/tr69_lib
L_OBJS+=../../ctl_layer/tr69fw/framework/tr69_lib/libtr69_client.o
L_OBJS+=../../ctl_layer/tr69fw/framework/tr69_lib/tsl_socket.o
LDFLAGS += -lm
endif

LDFLAGS += -lmgt_client

INTERNAL_HEADERS	+=

RAMDISK_BIN_FILES+=$(TARGET)

include $(RGMK)
