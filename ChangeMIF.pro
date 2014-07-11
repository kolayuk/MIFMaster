#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T19:26:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MIFMaster
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    application.cpp

HEADERS += \
    application.h \
    akniconconfig.h


symbian:TARGET.CAPABILITY += PowerMgmt WriteDeviceData
symbian:ICON=mifmaster3.svg
symbian:LIBS+=-lbafl -lstarterclient -lefsrv -lcone -lws32 -legul -leikcore -leikcore -lavkon -leiksrv -laknnotify -lhal -lapgrfx -laknicon
vendorinfo =  "%{\"KolaySoft\",\"KolaySoft\"}" \
    ":\"KolaySoft\""
vendor.pkg_prerules = vendorinfo
symbian:DEPLOYMENT += vendor
MMP_RULES-="DEBUGGABLE_UDEBONLY"
MMP_RULES+="DEBUGGABLE"

folder_01.sources = $$files($$PWD/patches/*.dll1)
folder_01.path = patches
DEPLOYMENT += folder_01

folder_02.sources = $$PWD/mifmaster_*.qm
folder_02.path =
DEPLOYMENT += folder_02

symbian: {
  TARGET.UID3 = 0xEA84EADA
  s60launcher.UID3 =  0xEA84EAEA
  }

lang =  "&EN,RU"
lang1.pkg_prerules = lang
symbian:DEPLOYMENT += lang1

packageheader = "$${LITERAL_HASH}{\"MIFMaster\",\"MIFMaster\"}, (0xEA84EADA), 1, 0, 0, TYPE=SA"
info.pkg_prerules=packageheader
symbian:DEPLOYMENT+=info

default_deployment.pkg_prerules -= pkg_platform_dependencies pkg_depends_qt

dep="(0x2001E61C), 4, 7, 4, {\"Qt\",\"Qt\"} \
[0x20022E6D],0,0,0,{\"S60ProductID\",\"S60ProductID\"} \
[0x20032DE7],0,0,0,{\"S60ProductID\",\"S60ProductID\"}"
depends.pkg_prerules=dep
symbian:DEPLOYMENT+=depends
include(s60autostart/s60autostart.pri)


TRANSLATIONS += mifmaster_ru.ts
    deployRscFile = "IF (LANGUAGE=16) \
    \"text16.txt\" - \"\", FT, TA \
    ELSE \
    \"text.txt\" - \"\", FT, TA \
ENDIF"
    deployFiles.pkg_postrules += deployRscFile
    DEPLOYMENT += deployFiles

default_bin_deployment.flags += FR RR

