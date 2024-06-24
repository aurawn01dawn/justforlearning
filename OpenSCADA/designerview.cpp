#include "designerview.h"

#include <QMouseEvent>
#include <QScrollBar>

DesignerView::DesignerView(QWidget* parent):QGraphicsView(parent)
{
    createInit();
    layoutInit();
    connectInit();
}

void  DesignerView::createInit(){

    _designertoolbar = DesignerToolBar::getDesignerToolBar();
    _sceneManager = ScenesManager::getScenesManager();

    // 设置抗锯齿
    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform |
                         QPainter::TextAntialiasing);

    // 开启鼠标未点击时仍出发鼠标移动事件
    this->setMouseTracking(true);
    this->setScene(_sceneManager->getCurDesignScene());

    QTransform transform;
    transform.scale(0.5, 0.5);
    this->setTransform(transform);
}

void  DesignerView::layoutInit(){
    this->setStyleSheet("MidWidgetGraphicsView {background-color:white}");
}

void  DesignerView::connectInit(){
    connect(_sceneManager, &ScenesManager::curDesignerSceneChanged, this, [this](QPointer<CustomScene> scene){
        this->setScene(scene);
    });
}

void DesignerView::mousePressEvent(QMouseEvent *event){
//    if(_designertoolbar->getCurToolState() == DesignerToolBar::VIEW_TOOL){
//        //鼠标左键
//        if(event->button()&Qt::LeftButton){
//            _lastPos = event->pos();
//        }
//    }

    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mousePressEvent(event);

}

void DesignerView::mouseMoveEvent(QMouseEvent *event){
    // 发送信号通知标尺移动标尺轴
//    emit mouseMovePos(event->pos());

//    // 根据工具栏进行不同处理
//    if(_designertoolbar->getCurToolState() == DesignerToolBar::VIEW_TOOL){
//        //鼠标左键
//        if(event->buttons()&Qt::LeftButton){
//            // 计算鼠标偏移
//            QPoint offset = event->pos()-_lastPos;
//            _lastPos = event->pos();
//            auto vBar = this->verticalScrollBar();
//            auto hBar = this->horizontalScrollBar();
//            vBar->setValue(vBar->value()-offset.y());
//            hBar->setValue(hBar->value()-offset.x());
//        }
//    }

    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mouseMoveEvent(event);

}

void DesignerView::mouseReleaseEvent(QMouseEvent *event){
    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::mouseReleaseEvent(event);
}

void DesignerView::wheelEvent(QWheelEvent *wheelEvent){
    if(_designertoolbar->getCurToolState() != DesignerToolBar::SHOW_TOOL){
        // 缩放
        int wheelValue = wheelEvent->angleDelta().y();;
        if(wheelValue > 0)_viewScale += 0.25;
        else _viewScale -= 0.25;
        if(_viewScale < 0.25)_viewScale = 0.25;
        QTransform transform;
        transform.scale(_viewScale, _viewScale);
        this->setTransform(transform);

        // 通知标尺修改度量
       // emit mouseWheelMove(_viewScale);

    }
    // 执行默认事件处理函数，将event传递给下一层
    QGraphicsView::wheelEvent(wheelEvent);

}

void DesignerView::dragMoveEvent(QDragMoveEvent *event){
    // 发送信号通知标尺移动标尺轴
    //emit mouseMovePos(event->pos());

    //执行默认拖动函数
    QGraphicsView::dragMoveEvent(event);
}
