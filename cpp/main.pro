#
# Authors: Géry Casiez
#
# Copyright © Inria

TEMPLATE  = app
CONFIG -= app_bundle
QT -= core

SOURCES  += main.cpp 

# ---------------------------------------------------------------------------
# liblag

LIBLAG_SRC=$$_PRO_FILE_PWD_
LIBLAG_DST=$$OUT_PWD
exists($$LIBLAG_SRC/lag/lag.pri) {
  include($$LIBLAG_SRC/lag/lag.pri)
} else {
  error("LibLag unavailable")
}


