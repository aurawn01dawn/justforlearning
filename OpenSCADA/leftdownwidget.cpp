#include "leftdownwidget.h"
#include "config.h"

#include <QVBoxLayout>
#include <QListWidgetItem>
#include <algorithm>
#include <QGraphicsView>

LeftDownWidget::LeftDownWidget(QWidget *parent) : QWidget(parent)
{
    _listWidget = new QListWidget(this);
    _updateTimer = new QTimer(this);
    _designerToolBar = DesignerToolBar::getDesignerToolBar();
    _sceneManager = ScenesManager::getScenesManager();

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(_listWidget);
    this->setLayout(vBoxLayout);

    connect(_updateTimer, &QTimer::timeout, this, &LeftDownWidget::updateTimeOutSlot);
    _updateTimer->start(LEFTDOWNWIDGET_UPDATE_TIME);

    connect(_listWidget, &QListWidget::itemClicked, this, &LeftDownWidget::itemClickedSlot);
}

void LeftDownWidget::updateTimeOutSlot(){
    static int ItemCount = 0;
    static QPointer<CustomScene> lastScene = nullptr;
    if(_designerToolBar->getCurToolState() != DesignerToolBar::SHOW_TOOL){
        QPointer<CustomScene> scene = _sceneManager->getCurDesignScene();
        if(scene.isNull())return;
        QList<QGraphicsItem *> items = scene->items();
        if(ItemCount != items.count() ||
                lastScene != scene){
            //清空现有布局
            _listWidget->clear();
            //更新布局
            for(auto it = items.begin(); it != items.end(); ++it){
                QPointer<AbsSceneWidgetItem> item = dynamic_cast<AbsSceneWidgetItem*>(*it);
                if(item){
                    QListWidgetItem* newItem = new QListWidgetItem();
                    newItem->setText(item->objectName());
                    newItem->setData(Qt::UserRole, QVariant::fromValue(item));
                    _listWidget->insertItem(0,newItem);
                }
            }
            //更新计数
            ItemCount = items.count();
            lastScene = scene;
        }
    }
}

void LeftDownWidget::itemClickedSlot(QListWidgetItem* cilckedItem){
    //对象指针
    QPointer<AbsSceneWidgetItem> sceneItem = cilckedItem->data(Qt::UserRole).value<QPointer<AbsSceneWidgetItem>>();
    if(sceneItem.isNull())return;
    emit sceneItem->itemStateChange(sceneItem);
    QList <QGraphicsView *> views = sceneItem->scene()->views();
    for(auto it = views.begin(); it != views.end(); ++it){
        QGraphicsView *view = *it;
        view->centerOn(sceneItem);
    }
}
