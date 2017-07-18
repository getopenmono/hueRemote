# Makefile created by monomake, Ons 27 Jul 2016 09:57:31 CEST
# Project: hueRemote

MONO_PATH=$(subst \,/,$(shell monomake path --bare))
include $(MONO_PATH)/predefines.mk

TARGET=hueRemote

include $(MONO_PATH)/mono.mk
