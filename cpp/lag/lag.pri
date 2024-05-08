# lag/lag.pri --
#
# Authors: Nicolas Roussel
#
# Copyright © Inria
#
# .pro files including this one should first define LIBLAG_SRC and LIBLAG_DST

# message("Reading lag.pri")

INCLUDEPATH += $$LIBLAG_SRC

windows {
    Debug:LIBLAG_DLL = $$LIBLAG_DST/lag/debug/lag.dll
    Release:LIBLAG_DLL = $$LIBLAG_DST/lag/release/lag.dll
    PRE_TARGETDEPS += $$LIBLAG_DLL    
    LIBS += $$LIBLAG_DLL
    Debug:EXEC_DIR = $$OUT_PWD/debug
    Release:EXEC_DIR = $$OUT_PWD/release
    copydll.commands = $(COPY_DIR) $$shell_path($$LIBLAG_DLL) $$shell_path($$EXEC_DIR)
    QMAKE_EXTRA_TARGETS += copydll
    POST_TARGETDEPS += copydll
} else {
    LIBS += -L$$LIBLAG_DST/lag -llag
    # link_prl is required when using a static library
    CONFIG += link_prl
    # Recompile apps when the library has changed    
    PRE_TARGETDEPS += $$LIBLAG_DST/lag/liblag.a
}

include(lag-common.pri)
