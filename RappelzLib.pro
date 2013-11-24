#-------------------------------------------------
#
# Project created by QtCreator 2013-03-10T12:01:47
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = RappelzLib
TEMPLATE = lib

DEFINES += RAPPELZLIB_LIBRARY
INCLUDEPATH += Common Interfaces

OBJECTS_DIR = objs
DESTDIR = bin
MOC_DIR = objs
RCC_DIR = objs
UI_DIR = ui
unix {
    LIBS += -lcrypto
    QMAKE_CXXFLAGS += -std=c++11
}
win32 {
	LIBS += -lD:/Programmes/Programmation/libraries/OpenSSL/lib/libeay32
	INCLUDEPATH += D:/Programmes/Programmation/libraries/OpenSSL/include
}


SOURCES += \
    Network/Server.cpp \
    Network/RC4Cipher.cpp \
    Network/EncryptedSocket.cpp \
    Network/Authentication.cpp \
    Network/RSACipher.cpp \
    Network/DESPasswordCipher.cpp \
    Network/Socket.cpp

HEADERS +=\
    Network/Server.h \
    Network/RC4Cipher.h \
    Network/EncryptedSocket.h \
    Packets/PacketEnums.h \
    Packets/PacketBaseMessage.h \
    Network/Authentication.h \
    Packets/AuthPackets.h \
    Packets/PacketStructs.h \
    Packets/TS_SC_RESULT.h \
    Packets/TS_CA_VERSION.h \
    Packets/TS_CA_OPT_ACCOUNT.h \
    Packets/TS_CA_IMBC_ACCOUNT.h \
    Packets/TS_CA_ACCOUNT.h \
    Packets/TS_CA_DISTRIBUTION_INFO.h \
    Packets/TS_CA_SELECT_SERVER.h \
    Packets/TS_CA_SERVER_LIST.h \
    Packets/TS_AC_RESULT.h \
    Packets/TS_AC_RESULT_WITH_STRING.h \
    Packets/TS_AC_SERVER_LIST.h \
    Packets/TS_AC_SELECT_SERVER.h \
    Packets/TS_AC_UPDATE_PENDING_TIME.h \
    Packets/TS_CS_VERSION.h \
    Packets/TS_CS_ACCOUNT_WITH_AUTH.h \
    Network/RSACipher.h \
    Network/DESPasswordCipher.h \
    Packets/TS_CA_RSA_PUBLIC_KEY.h \
    Packets/TS_AC_AES_KEY_IV.h \
    Common/RappelzLib_global.h \
    Common/Guid.h \
    Interfaces/IObject.h \
    Interfaces/ISocket.h \
    Interfaces/ISocketListener.h \
    Common/Delegate.h \
    Interfaces/ISocketListener.h \
    Interfaces/ISocket.h \
    Network/Socket.h
