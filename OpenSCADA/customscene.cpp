#include "customscene.h"
#include "config.h"

#include<QGraphicsSceneDragDropEvent>
#include<QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QTime>
#include <QPointer>

CustomScene::CustomScene(QObject *parent):QGraphicsScene(parent)
{
    _uuid = QUuid::createUuid();
    _designerToolBar = DesignerToolBar::getDesignerToolBar();


    //创建背景窗口物体
    QPainterPath path;
    path.addRoundedRect(VIEW_RECT_SIZE,1,1);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::DotLine);
    _backItem = this->addPath(path, pen);
    _backItem->setZValue(-99);
}

CustomScene::~CustomScene(){
    this->deleteAllItem();
}

void CustomScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){
}

void CustomScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
     event->accept();
}

void CustomScene::dropEvent(QGraphicsSceneDragDropEvent *event){
    AbsSceneWidgetItem::ComType comType = (AbsSceneWidgetItem::ComType)event->mimeData()->imageData().toInt();

    QPointer<AbsSceneWidgetItem> newItem = this->createItem(comType);
    newItem->setPos(event->scenePos());//设置位置
}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mousePressEvent(event);
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseMoveEvent(event);
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseReleaseEvent(event);
}

void CustomScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent){
    QGraphicsScene::wheelEvent(wheelEvent);
}

//保存
QVariantMap CustomScene::save(){
    QVariantMap saveMap;

    saveMap["_name"] = _name;
    saveMap["_uuid"] = _uuid;

    QVariantMap itemSaveMap;
    const QList<QGraphicsItem*> allItemList = this->items();
    int index = 0;
    for(auto it = allItemList.begin(); it != allItemList.end(); ++it){
        AbsSceneWidgetItem* item = dynamic_cast<AbsSceneWidgetItem*>(*it);
        if(!item)continue;
        itemSaveMap.insert(QString::number(index++),item->save());
    }

    saveMap["items"] = QVariant::fromValue(itemSaveMap);

    return saveMap;
}

//读取
void CustomScene::load(const QVariantMap& loadMap){

    _name = loadMap["_name"].toString();
    _uuid = loadMap["_uuid"].toString();

    const QVariantMap& itemLoadMap = loadMap["items"].toMap();
    for(auto it = itemLoadMap.begin(); it != itemLoadMap.end(); ++it){
        AbsSceneWidgetItem* newItem = new AbsSceneWidgetItem();
        this->addItem(newItem);
        newItem->load(it.value().toMap());
        connect(newItem, &AbsSceneWidgetItem::itemStateChange, this, [=](QPointer<AbsSceneWidgetItem> item){
            //qDebug() << QTime::currentTime();
            emit this->itemAtrChanged(item);});
    }

}


