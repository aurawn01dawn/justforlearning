#ifndef SCENESMANAGER_H
#define SCENESMANAGER_H

#include <QObject>
#include <QDebug>

#include "customscene.h"

class ScenesManager : public QObject
{
    Q_OBJECT
public:
    static ScenesManager* getScenesManager(QObject* parent = nullptr);

    //添加场景
    inline QPointer<CustomScene> createScene(){
        QPointer<CustomScene> newScene = new CustomScene(this);
        _scenesList.append(newScene);
        connect(newScene, &CustomScene::itemAtrChanged, this, [this](QPointer<AbsSceneWidgetItem> item){
            emit sceneItemChange(item);
        });
        return newScene;
    }

    //删除场景
    inline void deleteScene(QPointer<CustomScene> scene){
        //if(_scenesList.size() == 1)return;
        if(!_scenesList.contains(scene))return;
        _scenesList.removeAll(scene);
        scene->deleteAllItem();
        scene->deleteLater();
    }

    //设置当前设计界面view的场景
    inline void setCurDesignerScene(QPointer<CustomScene> scene){
        if(scene.isNull() || !_scenesList.contains(scene))return;
        _curDesignScene = scene;
        emit curDesignerSceneChanged(_curDesignScene);
    }

    //返回当前设计场景
    inline QPointer<CustomScene> getCurDesignScene(){
        if(_scenesList.contains(_curDesignScene)){
            return _curDesignScene;
        }else{
            //qDebug() << "_scenesMap don't contains scene";
            if(_scenesList.size()!=0)return _scenesList.first();
            return nullptr;
        }
    }

    //设置当前展示界面view的场景
    inline void setCurShowScene(QPointer<CustomScene> scene){
        if(scene.isNull() || !_scenesList.contains(scene))return;
        _curShowScene = scene;
        emit curShowSceneChanged(_curShowScene);
    }

    //返回当前展示场景
    inline QPointer<CustomScene> getCurShowScene(){
        if(_scenesList.contains(_curShowScene)){
            return _curShowScene;
        }else{
            //qDebug() << "_scenesMap don't contains scene";
            if(_scenesList.size()!=0)return _scenesList.first();
            return nullptr;
        }
    }

    //返回所有场景
    inline const QList<QPointer<CustomScene>>& getAllSceneList(){
        return _scenesList;
    }

    //从uuid获取场景对象
    QPointer<CustomScene> getSceneFromUuid(const QUuid& uuid){
        for(auto it = _scenesList.begin(); it != _scenesList.end(); ++it){
            if((*it)->getUuid() == uuid){
                return *it;
            }
        }
        return nullptr;
    }

    //从uuid获取场景物体对象
    QPointer<AbsSceneWidgetItem> getSceneItemFromUuid(const QUuid& uuid){
        for(auto it = _scenesList.begin(); it != _scenesList.end(); ++it){
            const QList<QGraphicsItem*> items = (*it)->items();
            for(auto it1 = items.begin(); it1 != items.end(); ++it1){
                AbsSceneWidgetItem* item = dynamic_cast<AbsSceneWidgetItem*>(*it1);
                if(item && item->getUuid() == uuid){
                    return item;
                }
            }
        }
        return nullptr;
    }

    //场景数据保存
    QVariantMap save();

    //场景数据加载
    void load(const QVariantMap&);

private:
    //所有场景
    QList<QPointer<CustomScene>> _scenesList;
    //当前设计view场景
    QPointer<CustomScene> _curDesignScene;
    //当前展示view场景
    QPointer<CustomScene> _curShowScene;

    explicit ScenesManager(QObject *parent = nullptr);


signals:

    void sceneItemChange(QPointer<AbsSceneWidgetItem>);

    void curDesignerSceneChanged(QPointer<CustomScene> scene);

    void curShowSceneChanged(QPointer<CustomScene> scene);

    //从读取文件中加载了新场景
    void loadNewScenes();

};

#endif // SCENESMANAGER_H
