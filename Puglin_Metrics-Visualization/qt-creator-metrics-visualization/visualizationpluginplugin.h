#ifndef VISUALIZATIONPLUGIN_H
#define VISUALIZATIONPLUGIN_H

#include "visualizationplugin_global.h"

#include <extensionsystem/iplugin.h>

class VisualizationOutputPane;

namespace VisualizationPlugin {
namespace Internal {

class VisualizationPluginPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    
public:
    VisualizationPluginPlugin();
    ~VisualizationPluginPlugin();
    
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

public Q_SLOTS:
    void action();

private:
    VisualizationOutputPane *m_visualizationOutputPane;
};

} // namespace Internal
} // namespace VisualizationPlugin

#endif // VISUALIZATIONPLUGIN_H

