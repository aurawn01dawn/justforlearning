#include "sceneselecttoolbar.h"
#include "designertoolbar.h"
#include "config.h"
#include "designerview.h"

#include <QTimer>
#include <QGraphicsView>
#include <QInputDialog>


SceneSelectToolBar::SceneSelectToolBar(DesignerView* designerView, QWidget *parent):QToolBar(parent)
{
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    _scenesManager = ScenesManager::getScenesManager();
    _designerView = designerView;

    _scenePtnGroup = new QActionGroup(this);
    _scenePtnGroup->setExclusive(true);

    //从sceneManager中加载场景
    this->addSeparator();
    const QList<QPointer<CustomScene>>& allSceneList = _scenesManager->getAllSceneList();
    for(auto it = allSceneList.begin(); it != allSceneList.end(); ++it){
        QAction* sceneAtn = new QAction(this);
        sceneAtn->setCheckable(true);
        _scenePtnGroup->addAction(sceneAtn);
        sceneAtn->setData(QVariant::fromValue(*it));
        sceneAtn->setText((*it)->getName());
        this->addAction(sceneAtn);
    }
    if(!_scenePtnGroup->actions().empty())_scenePtnGroup->actions().first()->setChecked(true);

    //添加按钮
    _addPtn = new QPushButton(QIcon(QString(":/icon/jia.png")),"", this);
    _delPtn = new QPushButton(QIcon(QString(":/icon/jian.png")),"", this);
    _addAtn = this->addWidget(_addPtn);
    _delAtn = this->addWidget(_delPtn);
    //定时器
    _timer = new QTimer(this);

    connect(this, &SceneSelectToolBar::actionTriggered, this, &SceneSelectToolBar::actionTriggeredSlot);
    connect(_addPtn, &QPushButton::clicked, this, &SceneSelectToolBar::addPtnClickedSlot);
    connect(_delPtn, &QPushButton::clicked, this, &SceneSelectToolBar::delPtnClickedSlot);
    connect(_timer, &QTimer::timeout, this, &SceneSelectToolBar::updateViewSceneIcon);
    connect(_scenesManager, &ScenesManager::loadNewScenes, this, &SceneSelectToolBar::updateScenePtn);
    _timer->start(SCENE_SELECT_ICON_UPDATE_TIME);
}


void SceneSelectToolBar::actionTriggeredSlot(QAction* action){
    //获取按钮对应的场景
    QPointer<CustomScene> scene = action->data().value<QPointer<CustomScene>>();
    //更换sceneManager对象中的场景对象
    _scenesManager->setCurDesignerScene(scene);
    _scenesManager->setCurShowScene(scene);
}

void SceneSelectToolBar::addPtnClickedSlot(){
    //获取新场景名
    bool ok;
    QString text = QInputDialog::getText(this, tr("设置新场景名称"),
                                             tr("场景名称:"), QLineEdit::Normal,
                                             "场景", &ok);
    if (!ok || text.isEmpty())return;

    //创建新场景与场景按钮
    QPointer<CustomScene> newScene = _scenesManager->createScene();
    QAction* sceneAtn = new QAction(this);
    sceneAtn->setCheckable(true);
    sceneAtn->setIcon(QIcon());
    _scenePtnGroup->addAction(sceneAtn);
    sceneAtn->setData(QVariant::fromValue(newScene));
    newScene->setName(text);
    sceneAtn->setText(text);

    //添加新场景的切换按钮
    this->insertAction(_addAtn, sceneAtn);

    //设置新场景为当前选择场景
    sceneAtn->setChecked(true);
    this->actionTriggeredSlot(sceneAtn);
}

void SceneSelectToolBar::delPtnClickedSlot(){
    //if(_scenePtnGroup->actions().size() == 1)return;
    //取出对应的场景对象
    QAction * sceneAtn = _scenePtnGroup->checkedAction();
    if(!sceneAtn)return;
    QPointer<CustomScene> deleteScene = sceneAtn->data().value<QPointer<CustomScene>>();
    //删除按钮
    _scenePtnGroup->removeAction(sceneAtn);
    this->removeAction(sceneAtn);
    sceneAtn->deleteLater();
    //删除场景
    _scenesManager->deleteScene(deleteScene);
    //更换当前场景
    if(_scenePtnGroup->actions().size() != 0){
        QAction* action = _scenePtnGroup->actions().first();
        action->setChecked(true);
        this->actionTriggeredSlot(action);
    }
}

void SceneSelectToolBar::updateViewSceneIcon(){
    if(DesignerToolBar::getDesignerToolBar()->getCurToolState() == DesignerToolBar::SHOW_TOOL)return;
    QRect rect = _designerView->viewport()->rect();
    QPixmap pixmap(rect.width(), rect.height());
    pixmap.fill(QColor(255,255,255));
    QPainter painter(&pixmap);
    _designerView->render(&painter);
    QIcon icon(pixmap);
    if(_scenePtnGroup->checkedAction())
        _scenePtnGroup->checkedAction()->setIcon(icon);
}

void SceneSelectToolBar::updateScenePtn(){
    //删除当前所有按钮
    const QList<QAction*>& actions = _scenePtnGroup->actions();
    for(auto it = actions.begin(); it != actions.end(); ++it){
        this->removeAction(*it);
        _scenePtnGroup->removeAction(*it);
        (*it)->deleteLater();
    }

    //获取当前所有场景
    const QList<QPointer<CustomScene>>& scenes = _scenesManager->getAllSceneList();
    for(auto it = scenes.begin(); it != scenes.end(); ++it){
        QAction* sceneAtn = new QAction(this);
        sceneAtn->setCheckable(true);
        _scenePtnGroup->addAction(sceneAtn);
        sceneAtn->setData(QVariant::fromValue(*it));
        sceneAtn->setText((*it)->getName());

        //添加新场景的切换按钮
        this->insertAction(_addAtn, sceneAtn);

        //设置新场景为当前选择场景
        sceneAtn->setChecked(true);
        this->actionTriggeredSlot(sceneAtn);
    }
}
