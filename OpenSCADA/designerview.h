#ifndef DESIGNERVIEW_H
#define DESIGNERVIEW_H

#include <QGraphicsView>

#include "designertoolbar.h"
#include "scenesmanager.h"

class DesignerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DesignerView(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void wheelEvent(QWheelEvent *wheelEvent)override;

    void dragMoveEvent(QDragMoveEvent *event) override;

private:


    DesignerToolBar* _designertoolbar;
    ScenesManager* _sceneManager;

    QPoint _lastPos;
    double _viewScale = 1;


    void createInit();
    void layoutInit();
    void connectInit();

signals:
    void mouseMovePos(QPoint);
    void mouseWheelMove(double);

};

#endif // DESIGNERVIEW_H
