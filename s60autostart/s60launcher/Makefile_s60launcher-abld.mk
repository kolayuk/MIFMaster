MAKE = make

do_nothing :
	@rem do_nothing

MAKMAKE: do_nothing

LIB: do_nothing

BLD: bld_launcher

CLEAN: extension_clean

CLEANLIB: do_nothing

RESOURCE: do_nothing

FREEZE: do_nothing

SAVESPACE: do_nothing

RELEASABLES: do_nothing

FINAL: do_nothing

pre_targetdeps :
	-$(MAKE) -f "..\..\Makefile" startuplist.rss
	-$(MAKE) -f "..\..\Makefile" s60launcher.mmp

extension_clean :
	-@ if EXIST abld.bat abld reallyclean
	-@ if EXIST abld.bat bldmake clean
	-@ if EXIST startuplist.rss del startuplist.rss
	-@ if EXIST s60launcher.mmp del s60launcher.mmp

bld_launcher : pre_targetdeps
	bldmake bldfiles
	abld build $(PLATFORM) $(CFG)
