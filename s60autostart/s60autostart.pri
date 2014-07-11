# s60autostart.pri
#
# Include this file in your .pro to add user-selectable autostart feature for
# the Qt for Symbian application. Compatible with S60 devices that support
# Startup List Management (and Qt) - that is, S60 3rd Ed, FP1 and up,
# including Symbian^3, Anna, Belle, ...
#
# Define TARGET.UID3 and s60launcher.UID3 before including this file!
#

# Add statement to bld.inf to build also s60launcher
symbian-sbsv2: BLD_INF_RULES.prj_mmpfiles += "gnumakefile s60autostart/s60launcher/Makefile_s60launcher-sbsv2.mk"
symbian-abld: BLD_INF_RULES.prj_mmpfiles += "gnumakefile s60autostart/s60launcher/Makefile_s60launcher-abld.mk"

symbian: {
    
    isEmpty( TARGET.UID3 ) {
      error("[S60AutoStart]: Main application UID (TARGET.UID3) must be defined before including s60autostart.pri")
    }
    
    isEmpty( s60launcher.UID3 ) {
      error("[S60AutoStart]: Launcher application UID (s60launcher.UID3) must be defined before including s60autostart.pri")
    }  
    
    TARGET.UID3 = $$replace(TARGET.UID3, 0X, 0x)
    s60launcher.UID3 = $$replace(s60launcher.UID3, 0x, "")
    s60launcher.UID3 = $$replace(s60launcher.UID3, 0X, "")
        
    launcherpath = $$PWD/s60launcher
    
    SOURCES += s60autostart/s60autostart.cpp
    HEADERS += s60autostart/s60autostart.h

    contains(QT, declarative) {
        SOURCES += s60autostart/qtquick-autostart.cpp
        HEADERS += s60autostart/qtquick-autostart.h
    }

    LIBS += -lefsrv \
            -lbafl

    # Update the native launcher resources
    asresource.target = startuplist.rss
    asresource.commands = perl \
        -pe \
        "s/LAUNCHAPP/$$TARGET/g" \
        < \
        $$launcherpath/$${asresource.target}.in \
        > \
        $$launcherpath/$$asresource.target

    # Update the native launcher mmp file
    launchermmp.target = s60launcher.mmp
    launchermmp.commands = perl \
        -pe \
        "s/LAUNCHERUID/$$s60launcher.UID3/g" \
        < \
        $$launcherpath/$${launchermmp.target}.in \
        > \
        $$launcherpath/$$launchermmp.target \

    QMAKE_EXTRA_TARGETS += asresource \                          
                           launchermmp

    PRE_TARGETDEPS += $$asresource.target \
                      $$launchermmp.target
    
    # Startup list manager resource file
    slmResource = "SOURCEPATH ./s60autostart/s60launcher" \
        "START RESOURCE $$asresource.target" \
        " TARGETPATH /private/101f875a" \
        "END" \
        " " \
        "$${LITERAL_HASH}ifdef SYMBIAN_ENABLE_SPLIT_HEADERS" \
        "MACRO          SYMBIAN_ENABLE_PUBLIC_PLATFORM_HEADER_SPLIT" \
        "$${LITERAL_HASH}endif"

    MMP_RULES += slmResource

    # Specify path and filename for autostart ini-file
    iniFile = \"c:\\system\\data\\$${TARGET}.ini\"

    # Use CONFIG += enable-autostart to have autostart enabled by default
    enable-autostart {
        # Add data file to the pkg as normal deployable
        pkgIniFile = "\"s60autostart/autostart\"-$${iniFile}"
    } else {
        # Add data file to the pkg as FILENULL, so it will be removed at uninstall
        pkgIniFile = "\"\"-$${iniFile}, FN"
    }
    
    # Add the launcher exe and startup list resource to the pkg file
    s60launcher.pkg_postrules = "\"$${EPOCROOT}epoc32/release/$(PLATFORM)/$(TARGET)/as_s60launcher.exe\" - \"c:\\sys\\bin\\as_$${TARGET}.exe\"" \
                                "\"$${EPOCROOT}epoc32/data/z/private/101f875a/startuplist.rsc\" - \"c:\\private\\101f875a\\import\\[$$replace(TARGET.UID3, 0x, "")].rsc\""
    
    # autostart ini file
    s60datafile.pkg_postrules = $${pkgIniFile}
    DEPLOYMENT += s60launcher \
                  s60datafile
} # : symbian

# eof
