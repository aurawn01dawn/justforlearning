#include "showwidgetview.h"

#include <QScrollBar>

ShowWidgetView::ShowWidgetView(QWidget *parent):QGraphicsView(parent)
{
    createInit();
    layoutInit();
    connectInit();
}

void  ShowWidgetView::createInit(){

    _sceneManager = ScenesManager::getScenesManager();

    // 设置抗锯齿
    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform |
                         QPainter::TextAntialiasing);


    this->setScene(_sceneManager->getCurShowScene());
}

void  ShowWidgetView::layoutInit(){
    this->setStyleSheet("MidWidgetGraphicsView {background-color:white}");
}

void  ShowWidgetView::connectInit(){
    connect(_sceneManager, &ScenesManager::curShowSceneChanged, this, [this](QPointer<CustomScene> scene){
        this->setScene(scene);
        this->fitInView(scene->getBackItem(), Qt::KeepAspectRatio);
    });
}

void ShowWidgetView::mousePressEvent(QMouseEvent *event){

    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mousePressEvent(event);

}

void ShowWidgetView::mouseMoveEvent(QMouseEvent *event){

    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mouseMoveEvent(event);

}

void ShowWidgetView::mouseReleaseEvent(QMouseEvent *event){
    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mouseReleaseEvent(event);
}

void ShowWidgetView::wheelEvent(QWheelEvent *wheelEvent){

    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::wheelEvent(wheelEvent);
}
