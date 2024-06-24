#ifndef SHOWWIDGETVIEW_H
#define SHOWWIDGETVIEW_H

#include <QGraphicsView>

#include "scenesmanager.h"

class ShowWidgetView : public QGraphicsView
{
    Q_OBJECT
public:
    ShowWidgetView(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void wheelEvent(QWheelEvent *wheelEvent)override;

private:

    ScenesManager* _sceneManager;

    QPoint _lastPos;
    double _viewScale = 1;


    void createInit();
    void layoutInit();
    void connectInit();
};

#endif // SHOWWIDGETVIEW_H
