#include "metriccollector.h"

#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>


#include <cplusplus/TypeHierarchyBuilder.h>
#include <cplusplus/Symbols.h>
#include <cplusplus/Literals.h>
#include <cplusplus/CppDocument.h>
#include <cplusplus/TranslationUnit.h>
#include <cplusplus/ModelManagerInterface.h>

#include <coreplugin/icore.h>

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

MetricCollector *MetricCollector::m_self = 0;

MetricCollector::MetricCollector(QObject *parent) :
    QObject(parent), m_sumFunctionsAllClasses(0)
{
    m_overview.setShowDefaultArguments(false);
}


MetricCollector *MetricCollector::instance()
{
    if (m_self == 0)
        m_self = new MetricCollector;
    return m_self;
}

void MetricCollector::visitProjectNode(ProjectExplorer::ProjectNode *projectNode)
{
    visitFolderNode(projectNode);
}

void MetricCollector::visitFolderNode(ProjectExplorer::FolderNode *folderNode)
{
    if (folderNode)
        foreach (const ProjectExplorer::FileNode *fileNode, folderNode->fileNodes())
        {
            if (fileNode)
                m_projectFiles.insertMulti(m_currentProjectNode, fileNode->path());
        }

}

bool MetricCollector::visit(CPlusPlus::Class *clazz)
{
    m_sumClassFunctions = 0;
    foreach(CPlusPlus::Class *clazz2, m_projectClasses.values(m_currentProjectNode))
    {
        if (!clazz2) continue;
        if(clazz2 && clazz2->name() && clazz2->name()->identifier() && clazz2->name()->identifier()->chars() &&
                clazz->name() && clazz->name()->identifier() && clazz->name()->identifier()->chars()){
            if (!strcmp(clazz2->name()->identifier()->chars(), clazz->name()->identifier()->chars()))
                m_projectClasses.remove(m_currentProjectNode, clazz2);
        }
    }

    m_projectClasses.remove(m_currentProjectNode, clazz);
    m_projectClasses.insertMulti(m_currentProjectNode, clazz);
    m_currentClass = clazz;
    return true;
}

bool MetricCollector::visit(CPlusPlus::Function *function)
{
    if (!function->isDeclaration() && !function->isGenerated())
    {
        m_projectClassFunctionInfo[m_overview.prettyName(function->unqualifiedName()) + m_overview.prettyType(function->type())] =
                m_currentSource.mid(function->startOffset(), function->endOffset()-function->startOffset()).split('\n').size();

    }

    return true;

}

bool MetricCollector::visit(CPlusPlus::Declaration *declaration)
{
    if (m_currentProjectNode && m_currentClass && m_currentClass->name() && m_currentClass->name()->identifier() &&  m_currentClass->name()->identifier() && declaration
            &&!declaration->isGenerated())
    {
        if (declaration->type() && declaration->type()->asFunctionType() && declaration->type()->asFunctionType()->name())
        {
            m_sumClassFunctions++;
            m_projectClassFunctions.insertMulti(ProjectClass(m_currentProjectNode, m_currentClass), m_overview.prettyName(declaration->name()) + m_overview.prettyType(declaration->type()));
             if (declaration->isPublic()){
                m_projectClassFunctionsPublic.insertMulti(ProjectClass(m_currentProjectNode, m_currentClass), declaration->type()->asFunctionType());

            }
        }

        else
            m_projectClassAttributes.insertMulti(ProjectClass(m_currentProjectNode, m_currentClass), declaration);

    }


    return true;

}

void MetricCollector::postVisit(CPlusPlus::Symbol *symbol)
{

    if (symbol && symbol->type() && symbol->type()->asClassType())
    {
        m_sumFunctionsAllClasses += m_sumClassFunctions;
        m_currentClass = 0;
    }



}


QVariantList MetricCollector::projectClassesToVariantList()
{
    QVariantList variantList;
    foreach(CPlusPlus::Class *clazz, m_projectClasses.values())
        if(clazz && clazz->name() && clazz->name()->identifier() && clazz->name()->identifier()->chars())
            variantList << clazz->name()->identifier()->chars();
    return variantList;
}


void MetricCollector::projectAdded(ProjectExplorer::Project *project)
{
    m_currentProjectNode = project->rootProjectNode();
    m_currentProjectNode->accept(this);
}

void MetricCollector::projectRemoved(ProjectExplorer::Project *project)
{
    ProjectExplorer::ProjectNode *projectNode = project->rootProjectNode();
    m_projectFiles.remove(projectNode);
    m_projectClasses.remove(projectNode);
}

void MetricCollector::documentUpdated(CPlusPlus::Document::Ptr doc)
{
    if (doc.isNull())
        return;
    if (m_projectFiles.values().contains(doc->fileName()))
    {
        //   qDebug() << "Processando doc " << doc->fileName();
        m_currentSource = doc->source();
        accept(doc->globalNamespace());
    }
}

QVariantMap MetricCollector::classFunctionsToVariantMap()
{
    QVariantMap variantMap;
    foreach(ProjectExplorer::ProjectNode *project, m_projectClasses.keys())
        foreach(CPlusPlus::Class *clazz, m_projectClasses.values(project))
            variantMap.insert(clazz->name()->identifier()->chars(), m_projectClassFunctions.count(ProjectClass(project, clazz)));
    return variantMap;
}
