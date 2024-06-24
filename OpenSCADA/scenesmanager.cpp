#include "scenesmanager.h"


ScenesManager* ScenesManager::getScenesManager(QObject* parent){
    static ScenesManager* midWidgetView = new ScenesManager(parent);
    return midWidgetView;
}

ScenesManager::ScenesManager(QObject *parent) : QObject(parent)
{
    _curDesignScene = this->createScene();
    _curDesignScene->setName("场景");
    _curShowScene = _curDesignScene;
}

//场景数据保存
QVariantMap ScenesManager::save(){
    QVariantMap saveMap;

    QVariantMap sceneSaveMap;
    int index = 0;
    for(auto it = _scenesList.begin(); it != _scenesList.end(); ++it){
        sceneSaveMap.insert(QString::number(index++), (*it)->save());
    }

    saveMap["scenes"] = QVariant::fromValue(sceneSaveMap);

    return saveMap;
}

//场景数据加载
void ScenesManager::load(const QVariantMap& loadMap){

    //清空原有场景
    for(auto it = _scenesList.begin(); it != _scenesList.end(); ++it){
        (*it)->deleteAllItem();
        (*it)->deleteLater();
    }
    _scenesList.clear();

    //加载新场景
    const QVariantMap& sceneLoadMap = loadMap["scenes"].toMap();

    for(auto it = sceneLoadMap.begin(); it != sceneLoadMap.end(); ++it){
        QPointer<CustomScene> newScene = this->createScene();
        newScene->load((*it).toMap());
    }
    //通知场景切换栏更新场景按钮
    this->emit loadNewScenes();
    //更新展示场景
    if(!_scenesList.empty())this->emit curShowSceneChanged(_scenesList.first());
}
