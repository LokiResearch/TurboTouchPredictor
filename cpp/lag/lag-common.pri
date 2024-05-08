# lag/lag-common.pri --
#
# Authors: Nicolas Roussel
#
# Copyright © Inria

# message("Reading lag-common.pri")

# Remove these optimization flags...
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2 
# ... and add -O3 if not present (might not work on all platforms)
QMAKE_CXXFLAGS_RELEASE *= -O3

android {
  DEFINES += ANDROID __ANDROID__ # So that moc knows about that funky platform
}

# ----------------------------------------------------------------------

windows {

  # This file should define HARDCODED_* variables and should not be
  # versionned, as it varies from one user to ther other...
  include(lag-common-windows.pri)


} else:mac {

  # Look for pkg-config in Fink, Macports and Homebrew (the last one we find wins)
  exists(/sw/bin/pkg-config) {
      QMAKE_PKG_CONFIG = /sw/bin/pkg-config
      INCLUDEPATH += /sw/include            
  }
  exists(/opt/local/bin/pkg-config) {
	  QMAKE_PKG_CONFIG = /opt/local/bin/pkg-CONFIG
      INCLUDEPATH += /opt/local/include      
  }
  exists(/usr/local/bin/pkg-config) {
      QMAKE_PKG_CONFIG = /usr/local/bin/pkg-config
      INCLUDEPATH += /usr/local/include
  }
  message("Looking for dependencies using $$QMAKE_PKG_CONFIG")


} else:linux-*  {

  # Look for pkg-config in the usual places
  exists(/usr/bin/pkg-config) { QMAKE_PKG_CONFIG = /usr/bin/pkg-config }
  exists(/usr/local/bin/pkg-config) { QMAKE_PKG_CONFIG = /usr/local/bin/pkg-config }
  message("Looking for dependencies using $$QMAKE_PKG_CONFIG")

}

!ios:mac|linux-* {



}
