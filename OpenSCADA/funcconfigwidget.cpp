#include "funcconfigwidget.h"
#include "functionselection.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

FuncConfigWidget::FuncConfigWidget(QWidget *parent) : QWidget(parent)
{
    _listWidget = new QListWidget(this);
    _addPtn = new QPushButton("添加", this);
    _delPtn = new QPushButton("删除", this);
    _addPtn->setFixedWidth(150);
    _delPtn->setFixedWidth(150);

    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(_addPtn);
    hBoxLayout->addWidget(_delPtn);

    QVBoxLayout* vBoxlayout = new QVBoxLayout;
    vBoxlayout->addWidget(_listWidget);
    vBoxlayout->addLayout(hBoxLayout);

    this->setLayout(vBoxlayout);

    this->setWindowFlags(Qt::Dialog);
    this->setWindowTitle("配置事件");
    this->setWindowModality(Qt::WindowModal);
    this->setMinimumWidth(800);
    this->setMinimumHeight(800);

    connect(_addPtn, &QPushButton::clicked, this, &FuncConfigWidget::addPtnClickedPtn);
    connect(_delPtn, &QPushButton::clicked, this, &FuncConfigWidget::delPtnClickedPtn);

}

//加载并显示当前窗口
void FuncConfigWidget::loadFuncShow(AbsSceneWidgetItem* sceneItem, AbsSceneWidgetItem::FunctionType type){
    //清空当前窗口显示中的widget
    for(int i =0; i < _listWidget->count(); ++i){
        QListWidgetItem* item = _listWidget->item(i);
        _listWidget->itemWidget(item)->deleteLater();
    }
    // 清空所有项
    _listWidget->clear();

    //重新加载
    QList<QPointer<CustomFunction>> funcList;
    _type = type;
    _sceneItem = sceneItem;
    switch (_type) {
    case AbsSceneWidgetItem::PRESS:
        funcList = _sceneItem->getPressFuncList();
        break;
    case AbsSceneWidgetItem::RELEASE:
        funcList = _sceneItem->getReleaseFuncList();
        break;
    case AbsSceneWidgetItem::TIMER:
        funcList = _sceneItem->getTimerFuncList();
        break;
    default:
        break;
    }

    //读取每一个函数对象并创建行Widget
    for(auto it = funcList.begin(); it != funcList.end(); ++it){
        QListWidgetItem* newListItem = new QListWidgetItem();
        newListItem->setData(Qt::UserRole, QVariant::fromValue(*it));
        _listWidget->addItem(newListItem);
        FunctionSelection* funcSelect = new FunctionSelection(*it, _listWidget);
        _listWidget->setItemWidget(newListItem, funcSelect);
        newListItem->setSizeHint(funcSelect->size());
    }
    this->show();
}

//添加行
void FuncConfigWidget::addPtnClickedPtn(){
    QListWidgetItem* newListItem = new QListWidgetItem();
    QPointer<CustomFunction> newFunction = _sceneItem->createFunction(_type);
    newListItem->setData(Qt::UserRole, QVariant::fromValue(newFunction));
    _listWidget->addItem(newListItem);
    FunctionSelection* funcSelect = new FunctionSelection(newFunction, _listWidget);
    _listWidget->setItemWidget(newListItem, funcSelect);
    newListItem->setSizeHint(funcSelect->size());
}

//删除行
void FuncConfigWidget::delPtnClickedPtn(){
    int row = _listWidget->currentRow();
    if(row == -1)return;

    QListWidgetItem* item = _listWidget->item(row);
    _listWidget->itemWidget(item)->deleteLater();
    QPointer<CustomFunction> function = item->data(Qt::UserRole).value<QPointer<CustomFunction>>();
    _sceneItem->deleteFunction(function);
    _listWidget->removeItemWidget(item);
    delete item;
}

