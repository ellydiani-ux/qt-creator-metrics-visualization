#ifndef VISUALIZATIONOUTPUTPANE_H
#define VISUALIZATIONOUTPUTPANE_H

#include <coreplugin/ioutputpane.h>

#include <QtCore/QObject>

#include <QtGui/QGraphicsRectItem>

class QGraphicsView;
class QGraphicsScene;
class QDeclarativeView;
class QToolButton;
class MetricCollector;

class VisualizationOutputPane : public Core::IOutputPane
{
    Q_OBJECT
public:
    VisualizationOutputPane(MetricCollector *metricCollector, QObject *parent);
    ~VisualizationOutputPane();

    // Virtual pure functions from Core::IOutputPane
    virtual QWidget *outputWidget(QWidget *parent);
    virtual QList<QWidget *> toolBarWidgets() const;
    virtual QString displayName() const;
    virtual int priorityInStatusBar() const;
    virtual void clearContents();
    virtual void visibilityChanged(bool visible);
    virtual void setFocus();
    virtual bool hasFocus() const;
    virtual bool canFocus() const;
    virtual bool canNavigate() const;
    virtual bool canNext() const;
    virtual bool canPrevious() const;
    virtual void goToNext();
    virtual void goToPrev();
    void updateSceneLegend();




public slots:
    void updateSceneQMC();
    void updateSceneQAC();
    void updateSceneMAM();
    void updateSceneQMP();
    void updateSceneMMC();
    void actionLegend();
    void updateSceneDetalhes();
    void updateSceneMTM();
    void updateSceneHelp();

private:
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    MetricCollector *m_metricCollector;
    QToolButton *m_reRunButtonQMC;
    QToolButton *m_reRunButtonQAC;
    QToolButton *m_reRunButtonMAM;
    QToolButton *m_reRunButtonMPG;
    QToolButton *m_reRunButtonQMP;
    QToolButton *m_reRunButtonMTM;
    QToolButton *m_reRunButtonLegenda;
    QToolButton * m_reRunButtonDetalhesMetricas;
    QToolButton * m_reRunButtonHelp;


};

#endif // VISUALIZATIONOUTPUTPANE_H
