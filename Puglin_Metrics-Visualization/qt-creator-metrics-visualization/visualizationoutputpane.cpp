#include "visualizationoutputpane.h"

#include <QtCore/QHash>
#include <QtCore/QDebug>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsLineItem>
#include <QPushButton>
#include <QLayout>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#include <projectexplorer/projectnodes.h>

#include <cplusplus/Symbols.h>
#include <cplusplus/Literals.h>


#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <coreplugin/icore.h>

#include "metriccollector.h"
#include<QToolButton>
#include<projectexplorer/projectexplorerconstants.h>

VisualizationOutputPane::VisualizationOutputPane(MetricCollector *metricCollector, QObject *parent)
    : Core::IOutputPane(parent), m_scene(new QGraphicsScene), m_view(new QGraphicsView), m_metricCollector(metricCollector),
      m_reRunButtonQMC(new QToolButton), m_reRunButtonQAC(new QToolButton), m_reRunButtonMAM(new QToolButton),  m_reRunButtonMPG(new QToolButton),
      m_reRunButtonQMP(new QToolButton), m_reRunButtonDetalhesMetricas(new QToolButton),m_reRunButtonLegenda(new QToolButton),m_reRunButtonMTM(new QToolButton), m_reRunButtonHelp(new QToolButton)
{
    m_view->setScene(m_scene);
    m_reRunButtonQMC->setText("QMC");
    m_reRunButtonQMC->setToolTip(trUtf8("Quantidade de Métodos por Classe"));
    m_reRunButtonQMC->setAutoRaise(true);
    m_reRunButtonQMC->setEnabled(true);

    m_reRunButtonQAC->setText("QAC");
    m_reRunButtonQAC->setToolTip(tr("Quantidade de Atributos por Classe"));
    m_reRunButtonQAC->setAutoRaise(true);
    m_reRunButtonQAC->setEnabled(true);

    m_reRunButtonMAM->setText("MAM");
    m_reRunButtonMAM->setToolTip(trUtf8("Média de Atributos por Métodos em uma classe"));
    m_reRunButtonMAM->setAutoRaise(true);
    m_reRunButtonMAM->setEnabled(true);

    m_reRunButtonQMP->setText("QMP");
    m_reRunButtonQMP->setToolTip(trUtf8("Quantidade de Métodos Públicos por Classe"));
    m_reRunButtonQMP->setAutoRaise(true);
    m_reRunButtonQMP->setEnabled(true);

    m_reRunButtonMPG->setText("MMC");
    m_reRunButtonMPG->setToolTip(trUtf8("Média das Métricas por Classes"));
    m_reRunButtonMPG->setAutoRaise(true);
    m_reRunButtonMPG->setEnabled(true);

    m_reRunButtonMTM->setText("TMC");
    m_reRunButtonMTM->setToolTip(trUtf8("Média do Tamanho dos Métodos por Classe"));
    m_reRunButtonMTM->setAutoRaise(true);
    m_reRunButtonMTM->setEnabled(true);


    m_reRunButtonDetalhesMetricas->setText("Detalhes");
    m_reRunButtonDetalhesMetricas->setToolTip(trUtf8("Detalhes das Métricas"));
    m_reRunButtonDetalhesMetricas->setAutoRaise(true);
    m_reRunButtonDetalhesMetricas->setEnabled(true);


    m_reRunButtonLegenda->setText("Legenda");
    m_reRunButtonLegenda->setToolTip(trUtf8("Legenda das Métricas"));
    m_reRunButtonLegenda->setAutoRaise(true);
    m_reRunButtonLegenda->setEnabled(true);

    m_reRunButtonHelp->setText("Ajuda?");
    m_reRunButtonHelp->setToolTip(trUtf8("Ajuda sobre as Métricas"));
    m_reRunButtonHelp->setAutoRaise(true);
    m_reRunButtonHelp->setEnabled(true);


    connect(m_reRunButtonQMC, SIGNAL(clicked()),this,SLOT(updateSceneQMC()));
    connect(m_reRunButtonQAC, SIGNAL(clicked()),this,SLOT(updateSceneQAC()));
    connect(m_reRunButtonMAM, SIGNAL(clicked()),this,SLOT(updateSceneMAM()));
    connect(m_reRunButtonMPG, SIGNAL(clicked()),this,SLOT(updateSceneMMC()));
    connect(m_reRunButtonQMP, SIGNAL(clicked()),this,SLOT(updateSceneQMP()));
    connect(m_reRunButtonMTM, SIGNAL(clicked()),this,SLOT(updateSceneMTM()));


    connect(m_reRunButtonLegenda, SIGNAL(clicked()),this,SLOT(actionLegend()));
    connect(  m_reRunButtonDetalhesMetricas, SIGNAL(clicked()),this,SLOT(updateSceneDetalhes()));
    connect(m_reRunButtonHelp, SIGNAL(clicked()),this,SLOT(updateSceneHelp()));



}

VisualizationOutputPane::~VisualizationOutputPane()
{
    delete m_view;
    delete m_scene;

}

QWidget *VisualizationOutputPane::outputWidget(QWidget *parent)
{
    m_view->setParent(parent);
    return m_view;

}

QList<QWidget *> VisualizationOutputPane::toolBarWidgets() const
{
    return QList<QWidget *>()<< m_reRunButtonQMC << m_reRunButtonQAC << m_reRunButtonQMP << m_reRunButtonMAM<< m_reRunButtonMTM<<
                                m_reRunButtonMPG<<m_reRunButtonDetalhesMetricas<<m_reRunButtonLegenda<<m_reRunButtonHelp;
}

QString VisualizationOutputPane::displayName() const
{
    return tr("Visualization of Metrics");
}

int VisualizationOutputPane::priorityInStatusBar() const
{
    return 0;
}

void VisualizationOutputPane::clearContents()
{
}

void VisualizationOutputPane::visibilityChanged(bool visible)
{
    Q_UNUSED(visible)
}

void VisualizationOutputPane::setFocus()
{
}

bool VisualizationOutputPane::hasFocus() const
{
    return false;
}

bool VisualizationOutputPane::canFocus() const
{
    return false;
}

bool VisualizationOutputPane::canNavigate() const
{
    return false;
}

bool VisualizationOutputPane::canNext() const
{
    return false;
}

bool VisualizationOutputPane::canPrevious() const
{
    return false;
}

void VisualizationOutputPane::goToNext()
{
}

void VisualizationOutputPane::goToPrev()
{
}

void VisualizationOutputPane::updateSceneQMC()
{
    int i = 10;
    m_scene->clear();
    updateSceneLegend();

    m_scene->addSimpleText(trUtf8("QUANTIDADE DE MÉTODOS POR CLASSE (QMC)"))->setPos(-300,-120);

    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        if (!project) continue;
        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            if (!clazz) continue;
            if (clazz->isUnavailable()) continue;
            if (clazz->name() && clazz->name()->identifier() && m_metricCollector)
            {
                int numberOfMethods = m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project, clazz));

                QVariant v;
                v.setValue(numberOfMethods);
                QString s = v.toString();
                if(numberOfMethods <=20){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfMethods*10, 20, numberOfMethods*10, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());
                    m_scene->addSimpleText(s)->setPos(i+2,-numberOfMethods*9.5);

                }
                else
                    if(numberOfMethods > 20 && numberOfMethods<=40 ){
                        QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfMethods*10, 20, numberOfMethods*10, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect->setPos(i, 0);
                        rect->setToolTip(clazz->name()->identifier()->chars());
                        m_scene->addSimpleText(s)->setPos(i+2,-numberOfMethods*9.5);

                    }
                    else
                        if(numberOfMethods >40){
                            QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfMethods*10, 20, numberOfMethods*10, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect->setPos(i, 0);
                            rect->setToolTip(clazz->name()->identifier()->chars());
                            m_scene->addSimpleText(s)->setPos(i+2,-numberOfMethods*9.5);


                        }
                //horizontal
                m_scene->addLine(0, 0,80+i,0,QPen(QColor(Qt::black)));

               //vertical
                m_scene->addRect(10, 10, 0,-numberOfMethods*11,QPen(QColor(Qt::black)));

                if(numberOfMethods>=40){
                    QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect1->setPos(i, -400);
                    rect1->setToolTip("Limite = 40");
                }
                QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
               ellipse->setPos(i+7,3);
                ellipse->setToolTip(project->displayName());

                i += 22;

            }
        }
        i += 30;

    }

}

void VisualizationOutputPane::updateSceneQAC()
{
    m_scene->clear();
    updateSceneLegend();

    m_scene->addSimpleText(trUtf8("QUANTIDADE DE ATRIBUTOS POR CLASSE (QAC)"))->setPos(-300,-120);

    int i = 10;

    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        if (!project) continue;
        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            if (!clazz) continue;
            if (clazz->isUnavailable()) continue;
            if (clazz->name() && clazz->name()->identifier())
            {
                int numberOfAttributes = m_metricCollector->m_projectClassAttributes.count(MetricCollector::ProjectClass(project, clazz));

                QVariant v;
                v.setValue(numberOfAttributes);
                QString s = v.toString();

                if(numberOfAttributes <= 4){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfAttributes*10, 20, numberOfAttributes*10, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());
                    m_scene->addSimpleText(s)->setPos(i+2,-numberOfAttributes*9.5);


                }
                else
                    if(numberOfAttributes > 4 && numberOfAttributes<=9 ){
                        QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfAttributes*10, 20, numberOfAttributes*10, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect->setPos(i, 0);
                        rect->setToolTip(clazz->name()->identifier()->chars());
                        m_scene->addSimpleText(s)->setPos(i+2,-numberOfAttributes*9.5);

                    }
                    else
                        if(numberOfAttributes >9){
                            QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfAttributes*10, 20, numberOfAttributes*10, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect->setPos(i, 0);
                            rect->setToolTip(clazz->name()->identifier()->chars());
                            m_scene->addSimpleText(s)->setPos(i+2,-numberOfAttributes*9.5);

                        }

                //horizontal
                m_scene->addLine(0, 0, 50 +i,0,QPen(QColor(Qt::black)));
                //vertical
                m_scene->addRect(10, 10, 0,-numberOfAttributes*11,QPen(QColor(Qt::black)));

                if(numberOfAttributes>=9){
                    QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect1->setPos(i+1, -90);
                    rect1->setToolTip("Limite = 9");
                }
                QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
                ellipse->setPos(i+7,3);
                ellipse->setToolTip(project->displayName());

                i += 22;
            }
        }
        i += 22;

    }

}


void VisualizationOutputPane::updateSceneMAM()
{
    m_scene->clear();
    updateSceneLegend();

    m_scene->addSimpleText(trUtf8("% MÉDIA DE ATRIBUTOS POR MÉTODOS EM UMA CLASSE (MAM)"))->setPos(-360,-120);


    int i = 10;
    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        if (!project) continue;
        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            if (!clazz) continue;
            if (clazz->isUnavailable()) continue;
            if (clazz->name() && clazz->name()->identifier())
            {
                int numberOfAttributes = m_metricCollector->m_projectClassAttributes.count(MetricCollector::ProjectClass(project, clazz));
                int numberOfMehods= m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project, clazz));
                int meanAttributesPerClasses = (numberOfMehods!=0)?(numberOfAttributes *100)/ numberOfMehods:0;

                QVariant v;
                v.setValue(meanAttributesPerClasses);
                QString s = v.toString();

                if(meanAttributesPerClasses <=22){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanAttributesPerClasses*3, 20, meanAttributesPerClasses*3, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());
                    m_scene->addSimpleText(s)->setPos(i+2,-meanAttributesPerClasses*2.5);

                }
                else{
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanAttributesPerClasses*3, 20, meanAttributesPerClasses*3, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());}
                m_scene->addSimpleText(s)->setPos(i+2,-meanAttributesPerClasses*2.5);

                //horizontal
                m_scene->addLine(0, 0, 50 +i,0,QPen(QColor(Qt::black)));
                //vertical
                m_scene->addRect(10, 10, 0,-meanAttributesPerClasses*3,QPen(QColor(Qt::black)));
                if(meanAttributesPerClasses>=22){
                    QGraphicsRectItem *rect1=  m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect1->setPos(i+1, -66);
                    rect1->setToolTip("Limite = 22");

                }

                QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
                ellipse->setPos(i+7,3);
                ellipse->setToolTip(project->displayName());

                i += 22;
            }
        }
        i += 22;

    }
}



void VisualizationOutputPane::updateSceneQMP()
{
    int i = 10;
    m_scene->clear();
    updateSceneLegend();

    m_scene->addSimpleText(trUtf8("QUANTIDADE DE MÉTODOS PÚBLICOS (QMP)"))->setPos(-280,-120);

    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        if (!project) continue;
        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            if (!clazz) continue;
            if (clazz->isUnavailable()) continue;
            if (clazz->name() && clazz->name()->identifier() && m_metricCollector)
            {
                int numberOfFunctionsPublic = m_metricCollector->m_projectClassFunctionsPublic.count(MetricCollector::ProjectClass(project, clazz));
                QVariant v;
                v.setValue(numberOfFunctionsPublic);
                QString s = v.toString();
                if(numberOfFunctionsPublic <=20){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfFunctionsPublic*10, 20, numberOfFunctionsPublic*10, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());
                    m_scene->addSimpleText(s)->setPos(i+2,-numberOfFunctionsPublic*9.5);

                }
                else
                    if(numberOfFunctionsPublic > 20 && numberOfFunctionsPublic<=40 ){
                        QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfFunctionsPublic*10, 20, numberOfFunctionsPublic*10, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect->setPos(i, 0);
                        rect->setToolTip(clazz->name()->identifier()->chars());
                        m_scene->addSimpleText(s)->setPos(i+2,-numberOfFunctionsPublic*9.5);

                    }
                    else
                        if(numberOfFunctionsPublic >40){
                            QGraphicsRectItem *rect = m_scene->addRect(0, -numberOfFunctionsPublic*10, 20, numberOfFunctionsPublic*10, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect->setPos(i, 0);
                            rect->setToolTip(clazz->name()->identifier()->chars());
                            m_scene->addSimpleText(s)->setPos(i+2,-numberOfFunctionsPublic*9.5);

                        }
                //horizontal
                m_scene->addLine(0, 0, 50 +i,0,QPen(QColor(Qt::black)));

                //vertical
                m_scene->addRect(10, 10, 0,-numberOfFunctionsPublic*11,QPen(QColor(Qt::black)));
                if(numberOfFunctionsPublic>=40){
                    QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect1->setPos(i, -400);
                    rect1->setToolTip("Limite = 40");
                }
                QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
                ellipse->setPos(i+7,3);
                ellipse->setToolTip(project->displayName());

                i += 22;

            }
        }
        i += 22;

    }

}


void VisualizationOutputPane::updateSceneMTM()
{
    int i = 10;
    int j = 10 ;
    int numberLine = 0;
    int sumNumberLine = 0;
    int meanNumberMethodPerClass=0;
    int sumNumberFunction = 0;

    m_scene->clear();
    updateSceneLegend();

    m_scene->addSimpleText(trUtf8("MÉDIA DO TAMANHO DOS MÉTODOS POR CLASSE (MTMC)"))->setPos(-350,-120);

    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        if (!project) continue;
        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            QGraphicsRectItem *rect = m_scene->addRect(0, 0, 600, 30,QPen(QColor(220,0,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect->setPos(-300,j+70);

            m_scene->addText("PROJETO:")->setPos(-90,j+30);
            m_scene->addText(project->displayName())->setPos(-30,j+30);
            m_scene->addText("CLASSE:")->setPos(-90,j+50);
            m_scene->addText(clazz->name()->identifier()->chars())->setPos(-30,j+50);
            m_scene->addText(trUtf8("MÉTODOS"))->setPos(-100,j+70);
            m_scene->addText("LINHAS")->setPos(180,j+70);
            m_scene->addRect(150, j+70,0,30,QPen(QColor(220,0,0)));


            if (!clazz) continue;
            if (clazz->isUnavailable()) continue;
            if (clazz->name() && clazz->name()->identifier() && m_metricCollector)
            {

               int numberOfMethods = m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project, clazz));
                 foreach(const QString &method, m_metricCollector->m_projectClassFunctions.values(MetricCollector::ProjectClass(project,clazz)))
                {
                     numberLine =  m_metricCollector->m_projectClassFunctionInfo.value(method);
                      m_scene->addText(method)->setPos(-270,j+100);

                    QGraphicsRectItem *rect0 = m_scene->addRect(0, 0, 600, 30,QPen(QColor(220,0,0)));
                    rect0->setPos(-300,j+100);

                    m_scene->addRect(150, j+100,0,30,QPen(QColor(220,0,0)));

                    QVariant v1;
                    v1.setValue(numberLine);
                    QString s1 = v1.toString();
                    m_scene->addText(s1)->setPos(180,j+110);

                   sumNumberLine += numberLine;
                   sumNumberFunction ++;
                   QVariant v2;
                  v2.setValue( sumNumberFunction);
                  QString s2 = v2.toString();
                  m_scene->addText(s2)->setPos(-300,j+100);

             j+=30;
                }

                j+=100;

                meanNumberMethodPerClass = sumNumberLine/  numberOfMethods;

            sumNumberLine = 0;

                QVariant v;
                v.setValue(meanNumberMethodPerClass);
                QString s = v.toString();
                if(meanNumberMethodPerClass <=18){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanNumberMethodPerClass*10, 20, meanNumberMethodPerClass*10, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(clazz->name()->identifier()->chars());
                    m_scene->addSimpleText(s)->setPos(i+2,-meanNumberMethodPerClass*9.5);

                }
                else
                    if(meanNumberMethodPerClass > 18 && meanNumberMethodPerClass<=30 ){
                        QGraphicsRectItem *rect = m_scene->addRect(0, -meanNumberMethodPerClass*10, 20, meanNumberMethodPerClass*10, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect->setPos(i, 0);
                        rect->setToolTip(clazz->name()->identifier()->chars());
                        m_scene->addSimpleText(s)->setPos(i+2,-meanNumberMethodPerClass*9.5);

                    }
                    else
                        if(meanNumberMethodPerClass >30){
                            QGraphicsRectItem *rect = m_scene->addRect(0, -meanNumberMethodPerClass*10, 20, meanNumberMethodPerClass*10, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect->setPos(i, 0);
                            rect->setToolTip(clazz->name()->identifier()->chars());
                            m_scene->addSimpleText(s)->setPos(i+2,-meanNumberMethodPerClass*9.5);

                        }
                //horizontal
                m_scene->addLine(0, 0,80+i,0,QPen(QColor(Qt::black)));

                //vertical
                m_scene->addRect(10, 10, 0,-meanNumberMethodPerClass*11,QPen(QColor(Qt::black)));

                if(meanNumberMethodPerClass>=40){
                    QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect1->setPos(i, -350);
                    rect1->setToolTip("Limite = 30");
                }
                QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
               ellipse->setPos(i+4,3);
                ellipse->setToolTip(project->displayName());

                i += 22;

            }
             sumNumberFunction = 0;
        }
        i += 22;
 j+=40;
    }


}


//MMP
void VisualizationOutputPane::updateSceneMMC(){
    m_scene->clear();
    updateSceneLegend();

    int sumFunctionsProject = 0;
    int i = 20;
    int j = 20;
    int numberFunctionPerClass = 0;
    int meanFunctionPerClass = 0;
    int numberClassPerProject =0 ;
    int sumClassPerProject = 0;
    int numberAttibutesPerClass = 0;
    int sumAttibutesPerProject = 0;
    int meanAttributesPerClass = 0;
    int numberClassBasePerProject = 0;
    int numberFunctionPublicPerClass = 0;
    int sumFunctionsPublicProject= 0;
    int sumberClassBasePerProject = 0;
    int meanFunctionPublicPerClass= 0;

    int meanAttributesPeFunction = 0;
    int sumMeanAttributesPerFunction = 0;
    int meanOfMeanAttributesPerFunction = 0;

    int numberLine = 0;
    int sunNumberLine = 0;
    int meanSizeMethodPerClass=0;
    int sunMeanSizeMethodPerClass=0;
    int meanOfMeanSizeMethodPerClass = 0;


    m_scene->addSimpleText(trUtf8("MÉDIA DAS MÉTRICAS POR CLASSES(MMC)"))->setPos(-280,-120);
    QGraphicsRectItem *rect1 = m_scene->addRect(0, 0, 630, 30,QPen(QColor(220,0,0)), QBrush(QColor(254,150,0), Qt::SolidPattern));
    rect1->setPos(-320,i);
    m_scene->addText("PROJETO ")->setPos(-280,i+10);
    m_scene->addText("MQMC")->setPos(-120,i+10);
    m_scene->addText("MQAC")->setPos(-30,i+10);
    m_scene->addText("MQMP")->setPos(60,i+10);
    m_scene->addText("MMAM(%)")->setPos(140,i+10);
    m_scene->addText("MTMC")->setPos(230,i+10);

    m_scene->addRect(-150, i,0,30,QPen(QColor(220,0,0)));
    m_scene->addRect(-60, i,0,30,QPen(QColor(220,0,0)));
    m_scene->addRect(30, i,0,30,QPen(QColor(220,0,0)));
    m_scene->addRect(120, i,0,30,QPen(QColor(220,0,0)));
    m_scene->addRect(210, i,0,30,QPen(QColor(220,0,0)));

    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {
        sumFunctionsProject = 0;
        sumClassPerProject = 0;
        sumAttibutesPerProject= 0;
        sumFunctionsPublicProject= 0;
        sumberClassBasePerProject = 0;
        sunNumberLine = 0;
        sumMeanAttributesPerFunction=0;


        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {

            if (clazz->name() && clazz->name()->identifier())
              {

                numberAttibutesPerClass = m_metricCollector->m_projectClassAttributes.count(MetricCollector::ProjectClass(project,clazz));
                numberFunctionPerClass = m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project,clazz));
                numberClassPerProject =  m_metricCollector->m_projectClasses.count(project,clazz);
                numberFunctionPublicPerClass  = m_metricCollector->m_projectClassFunctionsPublic.count(MetricCollector::ProjectClass(project,clazz));
                meanAttributesPeFunction = (numberFunctionPerClass!=0)?(numberAttibutesPerClass *100)/ numberFunctionPerClass :0;


                foreach(const QString &lineMethod, m_metricCollector->m_projectClassFunctions.values(MetricCollector::ProjectClass(project,clazz)))
                {
                     numberLine =  m_metricCollector->m_projectClassFunctionInfo.value(lineMethod);

                      sunNumberLine += numberLine;

                }

                meanSizeMethodPerClass = ( numberFunctionPerClass!=0)?(sunNumberLine)/  numberFunctionPerClass :0;
                sunNumberLine = 0;

            }


            sumAttibutesPerProject+=numberAttibutesPerClass;
            sumFunctionsProject += numberFunctionPerClass;
            sumClassPerProject+=numberClassPerProject;
            sumFunctionsPublicProject+=numberFunctionPublicPerClass;
            sumMeanAttributesPerFunction+= meanAttributesPeFunction;
            sunMeanSizeMethodPerClass+=meanSizeMethodPerClass;

        }

        meanFunctionPerClass = sumFunctionsProject /  sumClassPerProject;
        meanAttributesPerClass = sumAttibutesPerProject / sumClassPerProject;
        meanFunctionPublicPerClass = sumFunctionsPublicProject/sumClassPerProject;
        meanOfMeanAttributesPerFunction = sumMeanAttributesPerFunction/sumClassPerProject;
        meanOfMeanSizeMethodPerClass  = sunMeanSizeMethodPerClass/sumClassPerProject;

        QVariant v;
        v.setValue(meanFunctionPerClass );
        QString s = v.toString();

        if( meanFunctionPerClass <=20){
            QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPerClass  *5, 20,meanFunctionPerClass  *5, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
            rect->setPos(i, 0);
            rect->setToolTip(project->displayName());
            m_scene->addSimpleText("QMC|")->setPos(i-6,0);
            m_scene->addSimpleText(s)->setPos(i+2,-meanFunctionPerClass*4);


        }

        else
            if(meanFunctionPerClass  > 20 && meanFunctionPerClass <=40 ){
                QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPerClass *5, 20, meanFunctionPerClass *5, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                rect->setPos(i, 0);
                rect->setToolTip(project->displayName());
                m_scene->addSimpleText("QMC|")->setPos(i-6,0);
                m_scene->addSimpleText(s)->setPos(i+2,-meanFunctionPerClass*4);


            }
            else
                if(meanFunctionPerClass  >40){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPerClass *5, 20, meanFunctionPerClass *5, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i, 0);
                    rect->setToolTip(project->displayName());
                    m_scene->addSimpleText("QMC|")->setPos(i-6,0);
                    m_scene->addSimpleText(s)->setPos(i+2,-meanFunctionPerClass*4);


                }

        if(meanFunctionPerClass>=40){
            QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 19, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect1->setToolTip("Limite = 40");
            rect1->setPos(i+1, -200);
        }
        QVariant v2;
        v2.setValue(meanAttributesPerClass);
        QString a = v2.toString();

        if(meanAttributesPerClass>0 && meanAttributesPerClass <= 4){
            QGraphicsRectItem *rect = m_scene->addRect(0, -meanAttributesPerClass*5, 20, meanAttributesPerClass*5, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
            rect->setPos(i+26, 0);
            rect->setToolTip(project->displayName());
            m_scene->addSimpleText("QAC|")->setPos(i+23, 0);
            m_scene->addSimpleText(a)->setPos(i+27,-meanAttributesPerClass*4);

        }
        else
            if(meanAttributesPerClass > 4 && meanAttributesPerClass<=9 ){
                QGraphicsRectItem *rect = m_scene->addRect(0, -meanAttributesPerClass*5, 20, meanAttributesPerClass*5, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                rect->setPos(i+26, 0);
                rect->setToolTip(project->displayName());
                m_scene->addSimpleText("QAC|")->setPos(i+23,0);
                m_scene->addSimpleText(a)->setPos(i+27,-meanAttributesPerClass*4);

            }
            else
                if(meanAttributesPerClass >9 || meanAttributesPerClass<=0){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanAttributesPerClass*5, 20, meanAttributesPerClass*5, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i+26, 0);
                    rect->setToolTip(project->displayName());
                    m_scene->addSimpleText("QAC|")->setPos(i+23,0);
                    m_scene->addSimpleText(a)->setPos(i+27,-meanAttributesPerClass*4);

                }
        if(meanAttributesPerClass>=9){
            QGraphicsRectItem *rect1=m_scene->addRect(0, 0, 18, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect1->setPos(i+27, -45);
            rect1->setToolTip("Limite = 9");

        }



        QVariant v1;
        v1.setValue(meanFunctionPublicPerClass);
        QString s1 = v1.toString();

        if( meanFunctionPublicPerClass<=20){
            QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPublicPerClass  *5, 20,meanFunctionPublicPerClass  *5, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
            rect->setPos(i+52, 0);
            rect->setToolTip(project->displayName());
            m_scene->addSimpleText("QMP|")->setPos(i+50,0);
            m_scene->addSimpleText(s1)->setPos(i+54,-meanFunctionPublicPerClass*4);

        }

        else
            if(meanFunctionPerClass  > 20 && meanFunctionPublicPerClass <=40 ){
                QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPublicPerClass *5, 20, meanFunctionPublicPerClass *5, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                rect->setPos(i+52, 0);
                rect->setToolTip(project->displayName());
                m_scene->addSimpleText("QMP|")->setPos(i+50,0);
                m_scene->addSimpleText(s1)->setPos(i+54,-meanFunctionPublicPerClass*4);
            }
            else
                if(meanFunctionPublicPerClass  >40 ){
                    QGraphicsRectItem *rect = m_scene->addRect(0, -meanFunctionPublicPerClass *5, 20, meanFunctionPublicPerClass *5, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i+52, 0);
                    rect->setToolTip(project->displayName());
                    m_scene->addSimpleText("QMP|")->setPos(i+50,0);
                    m_scene->addSimpleText(s1)->setPos(i+54,-meanFunctionPublicPerClass*4);
                }

        if(meanFunctionPublicPerClass>=40){
            QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 19, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect1->setToolTip("Limite = 40");
            rect1->setPos(i+53, -200);
        }

        QVariant v3;
        v3.setValue( meanOfMeanAttributesPerFunction);
        QString s3 = v3.toString();

        if(meanOfMeanAttributesPerFunction>=1 && meanOfMeanAttributesPerFunction <=22){
            QGraphicsRectItem *rect = m_scene->addRect(0, - meanOfMeanAttributesPerFunction  *5, 20, meanOfMeanAttributesPerFunction *5, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
            rect->setPos(i+78, 0);
            rect->setToolTip(project->displayName());
            m_scene->addSimpleText("MAM|")->setPos(i+80,0);
            m_scene->addSimpleText(s3)->setPos(i+80,10);

        }
            else
                if(meanOfMeanAttributesPerFunction  >22 || meanOfMeanAttributesPerFunction < 1){
                    QGraphicsRectItem *rect = m_scene->addRect(0, - meanOfMeanAttributesPerFunction *5, 20,  meanOfMeanAttributesPerFunction *5, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i+78, 0);
                    rect->setToolTip(project->displayName());
                    m_scene->addSimpleText("MAM|")->setPos(i+80,0);
                    m_scene->addSimpleText(s3)->setPos(i+80,- meanOfMeanAttributesPerFunction*4);
                }

        if( meanOfMeanAttributesPerFunction>=22){
            QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 19, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect1->setToolTip("Limite = 22%");
            rect1->setPos(i+79, -110);
        }


        QVariant v4;
        v4.setValue(meanOfMeanSizeMethodPerClass);
        QString s4 = v4.toString();

        if(meanOfMeanSizeMethodPerClass <=30){
            QGraphicsRectItem *rect = m_scene->addRect(0, - meanOfMeanSizeMethodPerClass*5, 20, meanOfMeanSizeMethodPerClass*5, QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
            rect->setPos(i+104, 0);
            rect->setToolTip(project->displayName());
            m_scene->addSimpleText("MTM|")->setPos(i+109,0);
            m_scene->addSimpleText(s4)->setPos(i+105,- meanOfMeanSizeMethodPerClass*4);

        }
            else
                if(meanOfMeanSizeMethodPerClass >30){
                    QGraphicsRectItem *rect = m_scene->addRect(0, - meanOfMeanSizeMethodPerClass*5, 20,meanOfMeanSizeMethodPerClass*5, QPen(Qt::yellow), QBrush(QColor(220,0,0), Qt::SolidPattern));
                    rect->setPos(i+104, 0);
                    rect->setToolTip(project->displayName());
                    m_scene->addSimpleText("MTM|")->setPos(i+109,0);
                    m_scene->addSimpleText(s4)->setPos(i+105,-meanOfMeanSizeMethodPerClass*4);
                }

        if(meanOfMeanSizeMethodPerClass>=30){
            QGraphicsRectItem *rect1= m_scene->addRect(0, 0, 19, -2,QPen(QColor(254,150,0)),QBrush(QColor(254,150,0), Qt::SolidPattern));
            rect1->setToolTip("Limite = 30");
            rect1->setPos(i+105, -150);
        }


        //horizontal
        m_scene->addLine(0, 0,150+i,0,QPen(QColor(Qt::black)));
        //vertical
        m_scene->addRect(10, 10, 0,-meanFunctionPerClass*12,QPen(QColor(Qt::black)));
        m_scene->addRect(10, 10, 0,-meanAttributesPerClass*12,QPen(QColor(Qt::black)));

        QGraphicsRectItem *rect = m_scene->addRect(0, 0, 630, 30,QPen(QColor(220,0,0)));
        rect->setPos(-320,j+30);
        m_scene->addText(project->displayName())->setPos(-300,j+30);
        m_scene->addRect(-150, j+30,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(-60, j+30,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(30, j+30,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(120, j+30,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(210, j+30,0,30,QPen(QColor(220,0,0)));

        m_scene->addText(s)->setPos(-110,j+30);
        m_scene->addText(a)->setPos(-20,j+30);
        m_scene->addText(s1)->setPos(70,j+30);
        m_scene->addText(s3)->setPos(160,j+30);
        m_scene->addText(s4)->setPos(240,j+30);
        QGraphicsEllipseItem *ellipse = m_scene->addEllipse(0, -10, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
        ellipse->setPos(i-4,3);
        ellipse->setToolTip(project->displayName());


        i += 150;
        j+= 30;

    }
}


void VisualizationOutputPane::updateSceneLegend(){
    m_scene->clear();

    m_scene->addSimpleText("______ Legenda ______")->setPos(-220,-90);

    m_scene->addRect(-200, -70, 10,10,QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern))->setToolTip(trUtf8("Nível BOM"));
    m_scene->addSimpleText(trUtf8("Nível BOM"))->setPos(-180,-70);

    m_scene->addRect(-200, -50, 10,10, QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern))->setToolTip(trUtf8("Nível RAZOÁVEL"));
    m_scene->addSimpleText(trUtf8("Nível RAZOÁVEL"))->setPos(-180,-50);

    m_scene->addRect(-200, -30,10,10,   QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern))->setToolTip(trUtf8("Nível RUIM"));
    m_scene->addSimpleText(trUtf8("Nível RUIM")) ->setPos(-180,-30);
    m_scene->addRect(-200, -10, 15,2, QPen(QColor(254,150,0)),QBrush(QColor(254,150,0),Qt::SolidPattern))->setToolTip(trUtf8("Limite MÁXIMO"));
    m_scene->addSimpleText(trUtf8("Limite MÁXIMO"))->setPos(-180,-15);
    m_scene->addEllipse(-197, 8, 7, 7, QPen(QColor(Qt::magenta)), QBrush(QColor(Qt::cyan),Qt::SolidPattern));
    m_scene->addSimpleText(trUtf8("Nome do Projeto"))->setPos(-180,5);



}

void VisualizationOutputPane::actionLegend()
{
    QMessageBox::information(Core::ICore::instance()->mainWindow(),
                             trUtf8("Legenda das Métricas"),
                             QString(trUtf8("QMC - Quantidade de Métodos por Classe\n\nQAC - Quantidade de Atributos por Classe\n\nQMP - Quantidade de Métodos Públicos por Classe\n\n"))+
                             QString(trUtf8("MAM - Média de Atributos por Métodos em uma Classe\n\nTMC - Tamanho dos Métodos por Classe\n\nMMC - Média Aritmética das Métricas por Classe\n\n"))+
                             QString(trUtf8("MQMC - Média de QMC por Classe \n\nMQAC - Média de QAC por Classe\n\nMQMP - Média de QPM por Classe\n\n"))+
                             QString(trUtf8("MMAM - Média de MAM por Classe\n\nMTMC - Média de TMC por Classe")));



}


void VisualizationOutputPane::updateSceneDetalhes()
{
    int numberClassPerProject =0 ;
    int sumClassPerProject = 0;
    int i = 20;
    int numberLine = 0;
    int sunNumberLine = 0;
    int meanSizeMethodPerClass=0;


    m_scene->clear();
    m_scene->addSimpleText(trUtf8("______________ DETALHES DAS MÉTRICAS POR CLASSE _____________ "))->setPos(-150,-230);


    foreach(ProjectExplorer::ProjectNode *project, m_metricCollector->m_projectClasses.uniqueKeys())
    {  sumClassPerProject = 0;

        QGraphicsRectItem *rect1 = m_scene->addRect(0, 0, 700, 30,QPen(QColor(220,0,0)), QBrush(QColor(254,150,0), Qt::SolidPattern));
        rect1->setPos(-300,i-200);
        m_scene->addText("PROJETO:")->setPos(-20,i-220);
        m_scene->addText(project->displayName())->setPos(50,i-220);
        m_scene->addText("CLASSES ")->setPos(-220,i-190);
        m_scene->addText("QMC")->setPos(-40,i-190);
        m_scene->addText("QAC")->setPos(50,i-190);
        m_scene->addText("MAM(%)")->setPos(140,i-190);
        m_scene->addText("QMP")->setPos(230,i-190);
        m_scene->addText("MTM")->setPos(330,i-190);

        m_scene->addRect(-60, i-200,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(30, i-200,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(120, i-200,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(210, i-200,0,30,QPen(QColor(220,0,0)));
        m_scene->addRect(300, i-200,0,30,QPen(QColor(220,0,0)));

        if (!project) continue;

        foreach(CPlusPlus::Class *clazz, m_metricCollector->m_projectClasses.values(project))
        {
            if (!clazz)continue;

            if (clazz->isUnavailable()) continue;

            if (clazz->name() && clazz->name()->identifier() && m_metricCollector)
            {

                numberClassPerProject =  m_metricCollector->m_projectClasses.count(project,clazz);

                int numberOfFunctionsPublic = m_metricCollector->m_projectClassFunctionsPublic.count(MetricCollector::ProjectClass(project, clazz));

                int numberOfMethods = m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project, clazz));


                int numberOfAttributes = m_metricCollector->m_projectClassAttributes.count(MetricCollector::ProjectClass(project, clazz));

                int numberOfMehods= m_metricCollector->m_projectClassFunctions.count(MetricCollector::ProjectClass(project, clazz));
                int meanAttributesPerClasses = (numberOfMehods!=0)?(numberOfAttributes *100)/ numberOfMehods:0;


                foreach(const QString &lineMethod, m_metricCollector->m_projectClassFunctions.values(MetricCollector::ProjectClass(project,clazz)))
                {
                     numberLine =  m_metricCollector->m_projectClassFunctionInfo.value(lineMethod);

                      sunNumberLine += numberLine;

                }

                meanSizeMethodPerClass = ( numberOfMethods!=0)?(sunNumberLine)/  numberOfMethods :0;
                sunNumberLine = 0;

                QVariant v;
                v.setValue(numberOfMethods);
                QString s = v.toString();

                QGraphicsRectItem *rect = m_scene->addRect(0, 0, 700, 30,QPen(QColor(220,0,0)));
                rect->setPos(-300,i-170);
                m_scene->addText(clazz->name()->identifier()->chars())->setPos(-280,i- 170);
                m_scene->addText(s)->setPos(-50,i-170);
                m_scene->addRect(-60, i-170,0,30,QPen(QColor(220,0,0)));
                m_scene->addRect(30, i-170,0,30,QPen(QColor(220,0,0)));
                m_scene->addRect(120, i-170,0,30,QPen(QColor(220,0,0)));
                m_scene->addRect(210, i-170,0,30,QPen(QColor(220,0,0)));
                m_scene->addRect(300, i-170,0,30,QPen(QColor(220,0,0)));

                if( numberOfMethods>0 &&numberOfMethods <= 20){
                    QGraphicsRectItem *rect2 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                    rect2->setPos(-55,i-170 );
                    rect2->setToolTip(trUtf8("Nível BOM"));
                }
                else
                    if(numberOfMethods> 20 && numberOfMethods <=40){
                        QGraphicsRectItem *rect3 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect3->setPos(-55,i-170 );
                        rect3->setToolTip(trUtf8("Nível RAZOÁVEL"));
                    }
                    else
                        if(numberOfMethods>40 || numberOfMethods==0){
                            QGraphicsRectItem *rect4 = m_scene->addRect(0, 8, 6, 6,QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect4->setPos(-55,i-170 );
                            rect4->setToolTip(trUtf8("Nível RUIM"));
                        }

                QVariant v1;
                v1.setValue(numberOfFunctionsPublic);
                QString s1 = v1.toString();

                m_scene->addText(s1)->setPos(230,i-170);
                if(numberOfFunctionsPublic>0 ||numberOfFunctionsPublic <= 20){
                    QGraphicsRectItem *rect2 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect2->setPos(220,i-170 );
                    rect2->setToolTip(trUtf8("Nível BOM"));
                }
                else
                    if(numberOfFunctionsPublic> 20 && numberOfFunctionsPublic <=40){
                        QGraphicsRectItem *rect3 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect3->setPos(220,i-170 );
                        rect3->setToolTip(trUtf8("Nível RAZOÁVEL"));
                    }
                    else
                        if(numberOfFunctionsPublic>40 || numberOfFunctionsPublic==0){
                            QGraphicsRectItem *rect4 = m_scene->addRect(0, 8, 6, 6,QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect4->setPos(220,i-170 );
                            rect4->setToolTip(trUtf8("Nível RUIM"));
                        }



                QVariant v2;
                v2.setValue(meanSizeMethodPerClass);
                QString s2 = v2.toString();
                m_scene->addText(s2)->setPos(320,i-170);

                if(meanSizeMethodPerClass<= 30){
                    QGraphicsRectItem *rect2 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect2->setPos(310,i-170 );
                    rect2->setToolTip(trUtf8("Nível BOM"));
                }
                   else
                        if(meanSizeMethodPerClass>30){
                            QGraphicsRectItem *rect4 = m_scene->addRect(0, 8, 6, 6,QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect4->setPos(310,i-170 );
                            rect4->setToolTip(trUtf8("Nível RUIM"));
                 }


                QVariant v3;
                v3.setValue(numberOfAttributes);
                QString s3 = v3.toString();
                m_scene->addText(s3)->setPos(50,i-170);


                if(numberOfAttributes>0 && numberOfAttributes <= 4){
                    QGraphicsRectItem *rect2 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect2->setPos(40,i-170 );
                    rect2->setToolTip(trUtf8("Nível BOM"));
                }
                else
                    if(numberOfAttributes >4 && numberOfAttributes <=9){
                        QGraphicsRectItem *rect3 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(QColor(254,150,0), Qt::SolidPattern));
                        rect3->setPos(40,i-170 );
                        rect3->setToolTip(trUtf8("Nível RAZOÁVEL"));
                    }
                    else
                        if(numberOfAttributes>9 || numberOfAttributes ==0){
                            QGraphicsRectItem *rect4 = m_scene->addRect(0, 8, 6, 6,QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern));
                            rect4->setPos(40,i-170 );
                            rect4->setToolTip(trUtf8("Nível RUIM"));
                        }

                QVariant v4;
                v4.setValue(meanAttributesPerClasses);
                QString s4 = v4.toString();
                m_scene->addText(s4)->setPos(140,i-170);

                if(meanAttributesPerClasses>0 && meanAttributesPerClasses <= 22){
                    QGraphicsRectItem *rect2 = m_scene->addRect(0, 8, 6, 6,QPen(Qt::darkGreen), QBrush(Qt::green, Qt::SolidPattern));
                    rect2->setPos(130,i-170 );
                    rect2->setToolTip(trUtf8("Nível BOM"));
                }
                else
                    if(meanAttributesPerClasses<1 || meanAttributesPerClasses>22 ){
                        QGraphicsRectItem *rect4 = m_scene->addRect(0, 8, 6, 6,QPen(QColor(220,0,0)), QBrush(QColor(220,0,0), Qt::SolidPattern));
                        rect4->setPos(130,i-170 );
                        rect4->setToolTip(trUtf8("Nível RUIM"));
                    }

                i += 30;
            }
            sumClassPerProject+=numberClassPerProject;
            QVariant v5;
            v5.setValue( sumClassPerProject);
            QString s5 = v5.toString();
            m_scene->addText(s5)->setPos(-298,i-200);

        }

        i += 50;
    }

}


void VisualizationOutputPane::updateSceneHelp()
{
     m_scene->clear();

    m_scene->addSimpleText(trUtf8("________ AJUDA SOBRE AS MÉTRICAS ________"))->setPos(-100,-200);
    QGraphicsRectItem *rect1 = m_scene->addRect(0, 0, 700, 200,QPen(QColor(254,150,0)));
    rect1->setPos(-300,-150);
    QGraphicsRectItem *rect1a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(254,150,0), Qt::SolidPattern));
    rect1a->setPos(-250,-160);
    m_scene->addSimpleText("QMC")->setPos(-220,-155);
    m_scene->addSimpleText(trUtf8("A Métrica QMC (Quantidade de Métodos por Classe), calcula o número de métodos existentes em uma classe. As"))->setPos(-280,-110);
    m_scene->addSimpleText(trUtf8("classes com um número grande de métodos tendem a ser específicas para os seus objetivos, e geralmente as com uma"))->setPos(-290,-90);
    m_scene->addSimpleText(trUtf8("quantidade menor tendem a ser mais reutilizáveis. O número de métodos de uma classe não deveria passar de 20,"))->setPos(-290,-70);
    m_scene->addSimpleText(trUtf8("contudo aceita-se que esta quantidade chegue a 40."))->setPos(-290,-50);


    QGraphicsRectItem *rect2 = m_scene->addRect(0, 0, 700, 200,QPen(QColor(250,0,0)));
    rect2->setPos(-300,150);
    QGraphicsRectItem *rect2a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(250,0,0), Qt::SolidPattern));
    rect2a->setPos(-250,140);
    m_scene->addSimpleText("QAC")->setPos(-220,140);
    m_scene->addSimpleText(trUtf8("A Métrica QAC (Quantidade de Atributos por Classe),medi o tamanho da classe através da quantidade de atributos."))->setPos(-280,180);
    m_scene->addSimpleText(trUtf8("Ao contar a quantidade de atributos das classes, é possível ter um indicador da qualidade do projeto com outros"))->setPos(-290,200);
    m_scene->addSimpleText(trUtf8("objetos do sistema. Uma classe com um número grande de atributos indica que tem muitos relacionamentos. As classes"))->setPos(-290,220);
    m_scene->addSimpleText(trUtf8("com mais de três ou quatro atributos mascaram o problema de acoplamento da aplicação. Classes de interface com o"))->setPos(-290,240);
    m_scene->addSimpleText(trUtf8("usuário os números de atributos podem chegar a nove, pois essas classes necessitam de mais atributos para lidar"))->setPos(-290,260);
    m_scene->addSimpleText(trUtf8("com componentes da tela." ))->setPos(-290,280);

    QGraphicsRectItem *rect3 = m_scene->addRect(0, 0, 700, 200,QPen(Qt::green));
    rect3->setPos(-300,450);
    QGraphicsRectItem *rect3a = m_scene->addRect(0, 0, 100, 20,QPen(Qt::darkGreen),QBrush(Qt::green, Qt::SolidPattern));
    rect3a->setPos(-250,440);
    m_scene->addSimpleText("MAM")->setPos(-220,440);
    m_scene->addSimpleText(trUtf8("A Métrica MAM (Média de Atributos por Métodos em uma Classe),é obtida pelo total de atributos em uma classe dividido "))->setPos(-280,480);
    m_scene->addSimpleText(trUtf8("pelo total de métodos da mesma classe. Muitos atributos indicam a possibilidade das classes estarem fazendo além do"))->setPos(-290,500);
    m_scene->addSimpleText(trUtf8("que deveriam. O valor desta métrica é representado em porcentagem. Para obter o seu valor é necessário combinar os"))->setPos(-290,520);
    m_scene->addSimpleText(trUtf8("valores limites das outras métricas envolvidas. A quantidade limite de Atributos em uma classe é 9 e a de métodos é 40,"))->setPos(-290,540);
    m_scene->addSimpleText(trUtf8("dividindo um pelo outro obtemos o limite máximo da média de atributos por métodos em uma classe, o que dá "))->setPos(-290,560);
    m_scene->addSimpleText(trUtf8("aproximadamente 22%."))->setPos(-290,580);

    QGraphicsRectItem *rect4 = m_scene->addRect(0, 0, 700, 200,QPen(QColor(254,150,0)));
    rect4->setPos(-300,750);
    QGraphicsRectItem *rect4a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(254,150,0), Qt::SolidPattern));
    rect4a->setPos(-250,740);
    m_scene->addSimpleText("QMP")->setPos(-220,740);
    m_scene->addSimpleText(trUtf8("A Métrica QMP (Quantidade de Métodos Públicos por Classe), calcula o número de métodos públicos que está sendo"))->setPos(-280,780);
    m_scene->addSimpleText(trUtf8("usado na classe, esta e uma boa medida da responsabilidade total da classe. Os métodos públicos são aqueles"))->setPos(-290,800);
    m_scene->addSimpleText(trUtf8("serviços que estão disponíveis como serviços para outras classes. Métodos públicos são indicadores do trabalho"))->setPos(-290,820);
    m_scene->addSimpleText(trUtf8("total feito por uma classe, uma vez que eles são serviços utilizados por outras classes."))->setPos(-290,840);

    QGraphicsRectItem *rect5 = m_scene->addRect(0, 0, 700, 200,QPen(QColor(250,0,0)));
    rect5->setPos(-300,1050);
    QGraphicsRectItem *rect5a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(250,0,0), Qt::SolidPattern));
    rect5a->setPos(-250,1040);
    m_scene->addSimpleText("TMC")->setPos(-220,1040);
    m_scene->addSimpleText(trUtf8("A Métrica TMC (Tamanho do Método por Classe), é uma das formas existentes de obter o valor do tamanho dos  métodos de"))->setPos(-290,1070);
    m_scene->addSimpleText(trUtf8("uma classe. Se o tamanho dos métodos forem muito altos indica uma grande probabilidade que o código orientado a função"))->setPos(-290,1090);
    m_scene->addSimpleText(trUtf8("está sendo escrito, e não orientado a objetos. Esta métrica conta o número de linhas físicas de códigos ativo que estão em"))->setPos(-290,1110);
    m_scene->addSimpleText(trUtf8("um método, para criar métodos fáceis de manter eles devem ser pequenos. Deve-se esperar 30 linhas para códigos C++."))->setPos(-290,1130);

    QGraphicsRectItem *rect6 = m_scene->addRect(0, 0, 700, 200,QPen(Qt::green));
    rect6->setPos(-300,1350);
    QGraphicsRectItem *rect6a = m_scene->addRect(0, 0, 100, 20,QPen(Qt::darkGreen),QBrush(Qt::green, Qt::SolidPattern));
    rect6a->setPos(-250,1340);
    m_scene->addSimpleText(trUtf8("MMC"))->setPos(-220,1340);
    m_scene->addSimpleText(trUtf8("MMC (Média das Métricas por Classe), calcula a média aritmética das métricas por classe de todo projeto. Para obter "))->setPos(-290,1370);
    m_scene->addSimpleText(trUtf8("este valor soma o resultado de cada métrica equivalente para cada classe e divide o resultado pela quantidade de classes "))->setPos(-290,1390);
    m_scene->addSimpleText(trUtf8("totais do projeto, um exemplo é somando todos os valores da métrica QMC de todas as classes do projeto e dividindo o "))->setPos(-290,1410);
    m_scene->addSimpleText(trUtf8("resultado pelo total de classes.Com isso é possível ter uma visão geral das métricas no projeto, obtendo uma média "))->setPos(-290,1430);
    m_scene->addSimpleText(trUtf8("resultados globais das métricas. "))->setPos(-290,1450);

    QGraphicsRectItem *rect7 = m_scene->addRect(0, 0, 700, 200,QPen(QColor(254,150,0)));
    rect7->setPos(-300,1650);
    QGraphicsRectItem *rect7a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(254,150,0), Qt::SolidPattern));
    rect7a->setPos(-250,1640);
    m_scene->addSimpleText("Detalhes")->setPos(-220,1640);
    m_scene->addSimpleText(trUtf8("A opção Detalhes exibe todos os valores das métricas por classe de todo o projeto através de uma tabela. Essa opção"))->setPos(-290,1670);
    m_scene->addSimpleText(trUtf8("traz uma visão detalhada de todos os valores das métricas como forma de obter um resultado geral de todas as métricas."))->setPos(-290,1690);
    m_scene->addSimpleText(trUtf8("Apesar de não ter o desenho específico do gráfico, este é substituído por um quandrado pequeno exibido ao lado de cada"))->setPos(-290,1710);
    m_scene->addSimpleText(trUtf8("valor, que informa o nível de qualidade das métricas por classe. Ao passar o mouse em cima do quadrado exibe uma"))->setPos(-290,1730);
    m_scene->addSimpleText(trUtf8("mensagem definindo o nível da métrica, e além disso,representa o nível através das cores, quando o valor da métrica"))->setPos(-290,1750);
    m_scene->addSimpleText(trUtf8("está bom o quadrado fica verde, quando laranja o nível é razoável e vermelho a valor da métrica está com um nível"))->setPos(-290,1770);
    m_scene->addSimpleText(trUtf8("ruim e precisa ser melhorado."))->setPos(-290,1790);


    QGraphicsRectItem *rect8 = m_scene->addRect(0, 0, 700, 100,QPen(QColor(250,0,0)));
    rect8->setPos(-300,1950);
    QGraphicsRectItem *rect8a = m_scene->addRect(0, 0, 100, 20,QPen(QColor(220,0,-100)),QBrush(QColor(250,0,0), Qt::SolidPattern));
    rect8a->setPos(-250,1940);
    m_scene->addSimpleText("Legenda")->setPos(-220,1940);
    m_scene->addSimpleText(trUtf8("A legenda informa todas as siglas das métricas, é emitida uma janela com os dados toda vez que precionaro botão legenda, "))->setPos(-290,1970);
    m_scene->addSimpleText(trUtf8("deixando bem detalhado o que significa cada sigla, e ajudando o usuário a se situar."))->setPos(-290,1990);

}

