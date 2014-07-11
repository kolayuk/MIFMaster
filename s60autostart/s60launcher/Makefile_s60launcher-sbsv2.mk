MAKE = make
SBS = sbs

rvct40bin:=$(subst \,/,$(wildcard $(RVCT40BIN)))
rvct22bin:=$(subst \,/,$(wildcard $(RVCT22BIN)))
armplat:=$(filter ARMV%,$(PLATFORM))

# switch armvX platform target to gcce if there's no RVCT environment defined
PLAT_lc:=$(shell perl -e "if (!'$(armplat)' || '$(rvct40bin)' || '$(rvct22bin)') { print lc($(PLATFORM)); } else { print 'gcce'; }")
CFG_lc:=$(shell perl -e "print lc($(CFG))")

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
	@rem -$(SBS) reallyclean # disabled invoking sbs, seems to cause lockups
	-@ if EXIST startuplist.rss del startuplist.rss
	-@ if EXIST s60launcher.mmp del s60launcher.mmp

bld_launcher : pre_targetdeps
	$(SBS) -c $(PLAT_lc)_$(CFG_lc)