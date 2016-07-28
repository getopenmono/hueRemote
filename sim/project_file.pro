MONO_PROJECT_DIR = ..
MBED_COMPABILITY_DIR = ../../mbedComp
EMUNO_DIR = ../../emuno
FRAMEWORK_DIR = ../../mono_framework

QT = core gui widgets network
TEMPLATE = app
TARGET = hueRemote
DEFINES += EMUNO EMUNO_DEFINES_MAIN
OBJECTS_DIR = build

INCLUDEPATH +=	$$EMUNO_DIR $$EMUNO_DIR/vmbed $$EMUNO_DIR/vmbed/target_emuno \
				$$FRAMEWORK_DIR $$FRAMEWORK_DIR/display $$FRAMEWORK_DIR/display/ui \
				$$MBED_COMPABILITY_DIR/api $$MBED_COMPABILITY_DIR/hal $$MBED_COMPABILITY_DIR/common

SOURCES +=	$$files($$MONO_PROJECT_DIR/*.cpp) \
			$$files($$EMUNO_DIR/*.cpp) \
			$$files($$EMUNO_DIR/vmbed/*.cpp) \
			$$files($$EMUNO_DIR/vmbed/target_emuno/*.c) \
			$$files($$EMUNO_DIR/vmbed/target_emuno/*.cpp) \
			$$files($$FRAMEWORK_DIR/display/*.cpp) \
			$$files($$FRAMEWORK_DIR/display/ui/*.cpp) \
			$$files($$FRAMEWORK_DIR/*.cpp) \
			$$files($$FRAMEWORK_DIR/media/*.cpp) \
			$$files($$MBED_COMPABILITY_DIR/common/*.cpp) \
			$$files($$MBED_COMPABILITY_DIR/common/*.c) \
			$$FRAMEWORK_DIR/slre.c

SOURCES -=	$$FRAMEWORK_DIR/application_context.cpp \
			$$FRAMEWORK_DIR/mono_power_management.cpp \
			$$FRAMEWORK_DIR/act8600_power_system.cpp \
			$$FRAMEWORK_DIR/mono_accelerometer.cpp \
			$$FRAMEWORK_DIR/pct2075_temperature.cpp \
			$$FRAMEWORK_DIR/at30ts74_temperature.cpp \
			$$FRAMEWORK_DIR/mono_buzzer.cpp \
			$$FRAMEWORK_DIR/mono_touch_system.cpp \
			$$FRAMEWORK_DIR/http_client.cpp \
			$$FRAMEWORK_DIR/dns_resolver.cpp \
			$$MBED_COMPABILITY_DIR/common/retarget.cpp \
			$$MBED_COMPABILITY_DIR/common/FileLike.cpp \
			$$MBED_COMPABILITY_DIR/common/FileBase.cpp \
			$$MBED_COMPABILITY_DIR/common/FilePath.cpp \
			$$MBED_COMPABILITY_DIR/common/FileSystemLike.cpp \
			$$MBED_COMPABILITY_DIR/common/I2C.cpp \
			$$MBED_COMPABILITY_DIR/common/SPI.cpp \
			$$MBED_COMPABILITY_DIR/common/Stream.cpp \
			$$MBED_COMPABILITY_DIR/common/Serial.cpp \
			$$MBED_COMPABILITY_DIR/common/SerialBase.cpp \
			$$MBED_COMPABILITY_DIR/common/CallChain.cpp

HEADERS +=	$$files($$MONO_PROJECT_DIR/*.hpp) \
			$$files($$MONO_PROJECT_DIR/*.h) \
			$$files($$EMUNO_DIR/*.h) \
			$$files($$EMUNO_DIR/vmbed/target_emuno/*.h) \
			$$files($$FRAMEWORK_DIR/*.h) \
			$$files($$FRAMEWORK_DIR/display/*.h) \
			$$files($$FRAMEWORK_DIR/display/ui/*.h) \
			$$files($$FRAMEWORK_DIR/media/*.h) \
			$$files($$MBED_COMPABILITY_DIR/api/*.h) \
			$$files($$MBED_COMPABILITY_DIR/hal/*.h)

HEADERS -=  $$FRAMEWORK_DIR/application_context.h \
			$$FRAMEWORK_DIR/mono_power_management.h \
			$$FRAMEWORK_DIR/act8600_power_system.h \
			$$FRAMEWORK_DIR/mono_accelerometer.h \
			$$FRAMEWORK_DIR/pct2075_temperature.h \
			$$FRAMEWORK_DIR/at30ts74_temperature.h \
			$$FRAMEWORK_DIR/mono_buzzer.h \
			$$FRAMEWORK_DIR/mono_touch_system.h \
			$$MBED_COMPABILITY_DIR/api/FileLike.h \
			$$MBED_COMPABILITY_DIR/api/FileBase.h \
			$$FRAMEWORK_DIR/http_client.h \
			$$FRAMEWORK_DIR/dns_resolver.h \
			$$MBED_COMPABILITY_DIR/api/FilePath.h \
			$$MBED_COMPABILITY_DIR/api/DirHandle.h \
			$$MBED_COMPABILITY_DIR/api/Stream.h \
			$$MBED_COMPABILITY_DIR/api/FileSystemLike.h \
			$$MBED_COMPABILITY_DIR/api/FilePath.h \
			$$MBED_COMPABILITY_DIR/api/Stream.h \
			$$MBED_COMPABILITY_DIR/api/Serial.h \
			$$MBED_COMPABILITY_DIR/api/SerialBase.h \
			$$MBED_COMPABILITY_DIR/api/CallChain.h