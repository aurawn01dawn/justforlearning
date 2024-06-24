#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QGraphicsScene>
#include <QUuid>

#include "designertoolbar.h"
#include "absscenewidgetitem.h"

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = nullptr);
    ~CustomScene();

    //添加控件
    inline QPointer<AbsSceneWidgetItem> createItem(AbsSceneWidgetItem::ComType comType){

        QPointer<AbsSceneWidgetItem> newItem = new AbsSceneWidgetItem();
        switch (comType)
        {
        case AbsSceneWidgetItem::PUSHBUTTON:
            newItem->setWidget(comType,new QPushButton());
            newItem->setObjectName("按钮" + newItem->getUuid().toString());
            break;
        case AbsSceneWidgetItem::INPUT:
            newItem->setWidget(comType,new QLineEdit());
            newItem->setObjectName("输入" + newItem->getUuid().toString());
            break;
        case AbsSceneWidgetItem::OUTPUT:
            newItem->setWidget(comType,new QLCDNumber());
            newItem->setObjectName("输出" + newItem->getUuid().toString());
            break;
        case AbsSceneWidgetItem::TEXT:
            newItem->setWidget(comType,new QLabel());
            newItem->setObjectName("文本" + newItem->getUuid().toString());
            break;
        case AbsSceneWidgetItem::IMAGE:
            newItem->setWidget(comType,new QLabel());
            newItem->setObjectName("图片" + newItem->getUuid().toString());
            break;
         default:
            qDebug() << "Insert Item error, sceneId:" + _uuid.toString();
            newItem->deleteLater();
            return nullptr;
        }

        this->addItem(newItem);
        //传递子物体改变的信号
        connect(newItem, &AbsSceneWidgetItem::itemStateChange, this, [=](QPointer<AbsSceneWidgetItem> item){
            //qDebug() << QTime::currentTime();
            emit this->itemAtrChanged(item);});

        return newItem;
    }

    //删除当前场景中某个控件
    inline void deleteSceneItem(QPointer<AbsSceneWidgetItem> sceneItem){
        this->removeItem(sceneItem);
        sceneItem->deleteLater();
    }

    //删除场景中所有控件
    inline void deleteAllItem(){
        QList<QGraphicsItem*> items = this->items();
        for(auto it = items.begin(); it != items.end(); ++it){
            AbsSceneWidgetItem* sceneItem = dynamic_cast<AbsSceneWidgetItem*>(*it);
            if(sceneItem)this->deleteSceneItem(sceneItem);
        }
    }

    //设置名称
    inline void setName(const QString& name){_name = name;}

    //获取名称
    inline const QString& getName(){return _name;}

    //获取Uuid
    const QUuid& getUuid(){return _uuid;}

    //获取背景物体
    QGraphicsPathItem* getBackItem(){return _backItem;}

    //保存
    QVariantMap save();

    //读取
    void load(const QVariantMap&);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent  *event)override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event)override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)override;


private:
    //场景id
    QUuid _uuid;
    DesignerToolBar* _designerToolBar;
    //场景名称
    QGraphicsPathItem* _backItem;
    QString _name;

private slots:

signals:

    //属性改变信号
    void itemAtrChanged(QPointer<AbsSceneWidgetItem>);

};

#endif // CUSTOMSCENE_H
