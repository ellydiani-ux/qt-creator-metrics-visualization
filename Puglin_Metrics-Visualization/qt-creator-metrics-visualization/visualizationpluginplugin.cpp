#include "visualizationpluginplugin.h"

#include <QtCore/QtPlugin>

#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>

#include <cplusplus/ModelManagerInterface.h>

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/itexteditor.h>

#include <cplusplus/TranslationUnit.h>

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/session.h>

#include "metriccollector.h"
#include "visualizationoutputpane.h"
#include "visualizationpluginconstants.h"

using namespace VisualizationPlugin::Internal;

VisualizationPluginPlugin::VisualizationPluginPlugin()
{
    // Create your members
}

VisualizationPluginPlugin::~VisualizationPluginPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool VisualizationPluginPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // connect to other plugins' signals
    // "In the initialize method, a plugin can be sure that the plugins it
    //  depends on have initialized their members."

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)
    Core::ActionManager *am = Core::ICore::instance()->actionManager();
    
    QAction *action = new QAction(tr("VisualizationPlugin action"), this);
    Core::Command *cmd = am->registerAction(action, Constants::ACTION_ID,
                                            Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    
    Core::ActionContainer *menu = am->createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("VisualizationPlugin"));
    menu->addAction(cmd);
    am->actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
    
    ProjectExplorer::SessionManager *sessionManager = ProjectExplorer::ProjectExplorerPlugin::instance()->session();
    MetricCollector *metricCollector = MetricCollector::instance();
    connect(sessionManager, SIGNAL(projectAdded(ProjectExplorer::Project*)),
            metricCollector, SLOT(projectAdded(ProjectExplorer::Project*)));
    connect(sessionManager, SIGNAL(projectRemoved(ProjectExplorer::Project*)),
            metricCollector, SLOT(projectRemoved(ProjectExplorer::Project*)));

    connect(CPlusPlus::CppModelManagerInterface::instance(), SIGNAL(documentUpdated(CPlusPlus::Document::Ptr)),
            metricCollector, SLOT(documentUpdated(CPlusPlus::Document::Ptr)), Qt::DirectConnection);

    m_visualizationOutputPane = new VisualizationOutputPane(metricCollector, this);
    addAutoReleasedObject(m_visualizationOutputPane);

    return true;
}

void VisualizationPluginPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // "In the extensionsInitialized method, a plugin can be sure that all
    //  plugins that depend on it are completely initialized."
}

ExtensionSystem::IPlugin::ShutdownFlag VisualizationPluginPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void VisualizationPluginPlugin::action()
{
}

Q_EXPORT_PLUGIN2(VisualizationPlugin, VisualizationPluginPlugin)

