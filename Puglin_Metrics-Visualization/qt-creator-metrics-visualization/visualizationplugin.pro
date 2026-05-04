TARGET = VisualizationPlugin
TEMPLATE = lib

QT += declarative

DEFINES += VISUALIZATIONPLUGIN_LIBRARY

# VisualizationPlugin files

SOURCES += visualizationpluginplugin.cpp \
    visualizationoutputpane.cpp \
    metriccollector.cpp

HEADERS += visualizationpluginplugin.h\
        visualizationplugin_global.h\
        visualizationpluginconstants.h \
    visualizationoutputpane.h \
    metriccollector.h

OTHER_FILES = VisualizationPlugin.pluginspec \
    metrics.qml \
    Button.qml

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/elidiane/TCC/devel/src/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/elidiane/TCC/devel/build/qt-creator

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\Nokia\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/Nokia/qtcreator" or "~/.local/share/Nokia/qtcreator" on Linux
##    "~/Library/Application Support/Nokia/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = LiveBlue

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/coreplugin/coreplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/projectexplorer/projectexplorer.pri)
include($$IDE_SOURCE_TREE/src/libs/cplusplus/cplusplus.pri)

LIBS += -L$$IDE_PLUGIN_PATH/Nokia
