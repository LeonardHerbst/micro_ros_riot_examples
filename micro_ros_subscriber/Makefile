# name of your application
APPLICATION = micro_ros_subscriber

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

EXTERNAL_PKG_DIRS += $(CURDIR)/../external_pkgs
MICRO_ROS_MODULE ?= $(CURDIR)/../external_pkgs/micro_ros

USEPKG += micro_ros
#USEMODULE += micro_ros_serial_transport
USEMODULE += micro_ros_udp_transport

CFLAGS += -Wno-error=strict-prototypes
CFLAGS += -DTHREAD_STACKSIZE_MAIN=31768

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include

