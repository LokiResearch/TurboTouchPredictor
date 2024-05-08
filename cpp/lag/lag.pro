# lag/lag.pro --
#
# Initial software
# Authors: Nicolas Roussel
# Copyright © INRIA

# message("Reading lag.pro")

TARGET   = lag

TEMPLATE = lib

# "widgets" is needed for QApplicationPlusPlus
QT += widgets 

windows {
  # Creating a static library on Windows causes QObjects to be created
  # before the QApplication instance, which creates a QCoreApplication
  # instead and prevents QTimers and GUI stuff to work properly. Just
  # create a dynamic library instead...
} else {
  CONFIG  += staticlib create_prl  
}

# In order for "#include <lag/....h>" to work as expected
INCLUDEPATH += ..

# ----------------------------------------------------------------------

include(lag-common.pri)

# ----------------------------------------------------------------------

HEADERS += 	utils/URI.h \
	utils/TimeStamp.h \
	utils/ConfigDict.h \
	utils/OneEuroFilter.h \
	utils/ExponentialSmoothing.h \
	input/InputEvent.h \
	input/processing/InputEventProcessor.h \
	input/processing/OneEuroProcessor.h \
	input/processing/OneEuroVectorProcessor.h \
	predictor/Predictor.h \
	predictor/TurbotouchPredictor.h 

SOURCES += 	utils/URI.cpp \
	utils/TimeStamp.cpp \
	utils/ConfigDict.cpp \
	utils/OneEuroFilter.cpp \
	utils/ExponentialSmoothing.cpp \
	input/InputEvent.cpp \
	input/processing/InputEventProcessor.cpp \
	input/processing/OneEuroProcessor.cpp \
	input/processing/OneEuroVectorProcessor.cpp \
	predictor/Predictor.cpp \
	predictor/TurbotouchPredictor.cpp 

# ----------------------------------------------------------------------
# Platform-specific

mac {
  LIBS += -framework CoreFoundation
}


