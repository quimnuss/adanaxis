# -*-makefile-*-
##############################################################################
#
# This file contains original work by Andy Southgate.  Contact details can be
# found at http://www.mushware.com.  This file was placed in the Public
# Domain by Andy Southgate and Mushware Limited in 2004.
#
# This software carries NO WARRANTY of any kind.
#
##############################################################################

#
# $Id: app.Makefile.am,v 1.4 2005/08/01 20:23:43 southa Exp $
# $Log: app.Makefile.am,v $
# Revision 1.4  2005/08/01 20:23:43  southa
# Backdrop and build fixes
#
# Revision 1.3  2005/06/22 20:02:00  southa
# MushGame link work
#
# Revision 1.2  2005/06/04 13:18:46  southa
# Updates for Mac OS Release 0.1.2
#
# Revision 1.1  2004/01/07 11:14:15  southa
# Snapshot fixes
#

AUTOMAKE_OPTIONS=1.6
SUBDIRS = @TOP_SUBDIRS@
DIST_SUBDIRS = @TOP_SUBDIRS@
RUNFILE=${DESTDIR}${bindir}/adanaxiscontroldemo
RUNFILESAFE=${DESTDIR}${bindir}/adanaxiscontroldemo-recover
GZIPFILE=adanaxiscontroldemo-@APP_VERSION@.tar.gz
BZIPFILE=adanaxiscontroldemo-@APP_VERSION@.tar.bz2

DATAMAKEFILE=data-adanaxiscontroldemo/Makefile.am
MUSHDATADIR=data-adanaxiscontroldemo

all:prebuild-script

prebuild-script:
	-$(SHELL) -c 'if [ -f prebuild.sh ];then $(SHELL) ./prebuild.sh;fi'

release: install-strip
	(cd $(top_srcdir);$(SHELL) MakeRelease.sh)

mac-release: dist
	(cd $(top_srcdir);$(SHELL) macosx/MakeRelease.sh 'Adanaxis Control Demo' 'adanaxiscontroldemo' '@APP_VERSION@' '$(top_srcdir)/macosx/project/build/Deployment' '$(top_srcdir)/data-adanaxis')

win32-release: dist
	(cd $(top_srcdir);$(SHELL) win32/MakeInstaller.sh 'Adanaxis Control Demo' 'adanaxiscontroldemo' '@APP_VERSION@' '$(top_srcdir)/VisualC/project/Release' '$(top_srcdir)/data-adanaxis')

$(GZIPFILE): dist

$(BZIPFILE):$(GZIPFILE)
	rm -f $(BZIPFILE)
	gunzip -c $(GZIPFILE) | bzip2 --best > $(BZIPFILE)

rpm:
	@echo
	@echo No rpm target.  Available targets are mdk-rpm, redhat-rpm and suse-rpm
	@exit 1

%-rpm:adanaxiscontroldemo.spec.%
	cp -f $< adanaxiscontroldemo.spec
	rm -f $(BZIPFILE)
	$(MAKE) $(BZIPFILE)
	rpm -ta $(BZIPFILE)

install-exec-hook:
	rm -f $(RUNFILE)
	echo #!/bin/sh > $(RUNFILE)
	echo cd $(DESTDIR)$(pkgdatadir)/system >> $(RUNFILE)
	echo $(DESTDIR)$(bindir)/adanaxiscontroldemobinary >> $(RUNFILE)
	chmod 0755 $(RUNFILE)
	rm -f $(RUNFILESAFE)
	echo #!/bin/sh > $(RUNFILESAFE)
	echo cd $(DESTDIR)$(pkgdatadir)/system >> $(RUNFILESAFE)
	echo "$(DESTDIR)$(bindir)/adanaxiscontroldemobinary \"load('start_safe.txt')\"" >> $(RUNFILESAFE)
	chmod 0755 $(RUNFILESAFE)

uninstall-hook:
	rm -f $(RUNFILE)
	rm -f $(RUNFILESAFE)

# Blank line required - configuration scripts append here
EXTRA_DIST=adanaxiscontroldemo.spec.mdk adanaxiscontroldemo.spec.generic macosx/project/Adanaxis-Info.plist macosx/project/Maurheen-Info.plist macosx/project/MushMesh-Info.plist macosx/project/MushPie-Info.plist macosx/project/Mushcore-Info.plist macosx/project/Mustl-Info.plist macosx/project/TesseractTrainer.plist macosx/project/TestMushMesh-Info.plist macosx/project/TestMushPie-Info.plist macosx/project/TestMushcore-Info.plist macosx/project/TestMustl-Info.plist macosx/project/all.Info.plist macosx/project/dummp-Info.plist README.linux README README.macosx README.win32 SourceConditioner.pl autogen.pl SourceProcess.pm macosx/project/project.xcodeproj/project.pbxproj macosx/MakeRelease.sh macosx/Start_in_Recovery_Mode.app macosx/Start_in_Recovery_Mode.scpt win32/adanaxiscontroldemo_app.ico win32/adanaxiscontroldemo_inst_app.ico win32/adanaxiscontroldemo_uninst_app.ico win32/MakeInstaller.sh win32/installer.nsi VisualC/project/adanaxiscontroldemo_app.ico 
dist_man_MANS=x11/man/adanaxiscontroldemo.6 x11/man/adanaxiscontroldemo-recover.6 
