#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QActionGroup>
#include <QToolBar>


#include "designerwidget.h"
#include "showwidget.h"
#include "datawidget.h"
#include "designertoolbar.h"
#include "saveloadmanager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void createInit();
    void layoutInit();
    void connectInit();

    // 几个界面
    QWidget *_centerWidget;
    QStackedWidget *_stackWidget;
    DesignerWidget *_designerWidget;
    ShowWidget *_showWidget;
    DataWidget* _dataWidget;

    // 上方工具栏
    QToolBar *_upToolBar;
    QActionGroup *_upActionGroup;
    QAction *_creatorAtn;
    QAction *_showAtn;
    QAction *_dataAtn;

    // 下方工具栏
    SaveLoadManager* _saveLoadManager;
    QToolBar *_downToolBar;
    QAction *_loadAtn;
    QAction *_saveAtn;
    QActionGroup *_downActionGroup;
    QAction *_startListenAtn;
    QAction *_stopListenAtn;

    //其他
    DesignerToolBar* _designerToolBar;

private slots:
    void upToolBarActionTriggerSlot(QAction *action);
    void downToolBarActionTriggerSlot(QAction *action);
};
#endif // MAINWINDOW_H
