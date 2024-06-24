#ifndef LEFTDOWNWIDGET_H
#define LEFTDOWNWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTimer>

#include "designertoolbar.h"
#include "scenesmanager.h"

class LeftDownWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftDownWidget(QWidget *parent = nullptr);

private:
    QListWidget* _listWidget;
    QTimer* _updateTimer;
    DesignerToolBar* _designerToolBar;
    ScenesManager* _sceneManager;

private slots:

    void updateTimeOutSlot();

    void itemClickedSlot(QListWidgetItem*);


signals:


};

#endif // LEFTDOWNWIDGET_H
