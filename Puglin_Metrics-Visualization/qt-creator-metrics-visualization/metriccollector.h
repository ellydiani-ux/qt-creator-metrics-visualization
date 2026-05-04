#ifndef METRICCOLLECTOR_H
#define METRICCOLLECTOR_H

#include <QtCore/QObject>
#include <QtCore/QMultiHash>
#include <QtCore/QVariantList>

#include <cplusplus/TypeHierarchyBuilder.h>
#include <cplusplus/CppDocument.h>
#include <cplusplus/SymbolVisitor.h>

#include <projectexplorer/nodesvisitor.h>

namespace ProjectExplorer
{
class Project;
class FolderNode;
class ProjectNode;
}

class MetricCollector : public QObject, public ProjectExplorer::NodesVisitor, public CPlusPlus::SymbolVisitor
{
    Q_OBJECT
public:
    static MetricCollector *instance();

    virtual void visitProjectNode(ProjectExplorer::ProjectNode *projectNode);
    virtual void visitFolderNode(ProjectExplorer::FolderNode *folderNode);

    virtual bool visit(CPlusPlus::Class *clazz);
    virtual bool visit(CPlusPlus::Function *function);
    virtual bool visit(CPlusPlus::Declaration *function);

    virtual void postVisit(CPlusPlus::Symbol *symbol);

    // QML binding functions
    Q_INVOKABLE QVariantList projectClassesToVariantList();
    Q_INVOKABLE QVariantMap classFunctionsToVariantMap();

protected:
    explicit MetricCollector(QObject *parent = 0);

public slots:
    void projectAdded(ProjectExplorer::Project *project);
    void projectRemoved(ProjectExplorer::Project *project);

    void documentUpdated(CPlusPlus::Document::Ptr doc);
public:
    QMultiHash<ProjectExplorer::ProjectNode *, QString> m_projectFiles;
    QMultiHash<ProjectExplorer::ProjectNode *, CPlusPlus::Class *> m_projectClasses;
    typedef QPair<ProjectExplorer::ProjectNode *, CPlusPlus::Class *> ProjectClass;
    QMultiHash<ProjectClass, QString> m_projectClassFunctions;

    QHash<QString, int> m_projectClassFunctionInfo;
    QMultiHash<ProjectClass, CPlusPlus::Declaration *> m_projectClassAttributes;

    QMultiHash<ProjectClass, CPlusPlus::Function *> m_projectClassFunctionsPublic;

    CPlusPlus::Symbol*m_symbol;

    int m_sumFunctionsAllClasses;
    int m_sumClassFunctions;

private:
    static MetricCollector *m_self;

    ProjectExplorer::ProjectNode *m_currentProjectNode;
    CPlusPlus::Class *m_currentClass;
    QByteArray m_currentSource;
    CPlusPlus::Overview m_overview;
};

#endif // METRICCOLLECTOR_H
