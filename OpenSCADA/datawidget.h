#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QTimer>
#include <QPushButton>

#include "datamanager.h"



class DataWidget : public QFrame
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget *parent = nullptr);
private:
    QTabWidget *_tabWidget;
    QTableWidget* _localTableWidget;
    QTableWidget* _redisTableWidget;
    DataManager *_dataManager;
    QTimer *_updateTimer;
    QPushButton* _addPtn;
    QPushButton* _delPtn;


    //加载页面数据
    void loadTabWidget();



private slots:
    void addPtnClickedSlot();
    void delPtnClickedSlot();
    void setCurRowDataSlot();
    void updataDataWidgetSlot();
signals:

};

#endif // DATAWIDGET_H
