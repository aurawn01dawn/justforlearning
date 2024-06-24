#ifndef FUNCCONFIGWIDGET_H
#define FUNCCONFIGWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QPushButton>

#include "absscenewidgetitem.h"

class FuncConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FuncConfigWidget(QWidget *parent = nullptr);

    //加载并显示当前窗口
    void loadFuncShow(AbsSceneWidgetItem* , AbsSceneWidgetItem::FunctionType type);


private:
    AbsSceneWidgetItem::FunctionType _type;
    AbsSceneWidgetItem* _sceneItem;
    QListWidget *_listWidget;
    QPushButton *_addPtn;
    QPushButton *_delPtn;

private slots:

    //添加行
    void addPtnClickedPtn();

    //删除行
    void delPtnClickedPtn();

signals:

};

#endif // FUNCCONFIGWIDGET_H
