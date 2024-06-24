#include "datamanager.h"

DataManager* DataManager::getDataManager(QObject *parent){
    static DataManager* dataManager = new DataManager(parent);
    return dataManager;
}

DataManager::DataManager(QObject *parent) : QObject(parent)
{

}

//保存
QVariantMap DataManager::save(){
    QVariantMap saveMap;
    QVariantMap localSaveDataMap;
    int index = 0;
    for(auto it = _localList.begin(); it != _localList.end(); ++it){
        localSaveDataMap.insert(QString::number(index++), (*it)->save());
    }
    saveMap["localData"] = localSaveDataMap;

    QVariantMap redisSaveDataMap;
    for(auto it = _redisList.begin(); it != _redisList.end(); ++it){
        redisSaveDataMap.insert(QString::number(index++), (*it)->save());
    }
    saveMap["redisData"] = redisSaveDataMap;

    return saveMap;
}

//读取
void DataManager::load(const QVariantMap& loadMap){

    //清空已有数据
    for(auto it = _allList.begin(); it != _allList.end(); ++it){
        (*it)->deleteLater();
    }
    _allList.clear();
    _localList.clear();
    _redisList.clear();

    //加载保存数据
    const QVariantMap& localLoadDataMap = loadMap["localData"].toMap();
    for(auto it = localLoadDataMap.begin(); it != localLoadDataMap.end(); ++it){
        CustomDataItem* newData = this->createData(CustomDataItem::LOCAL);
        newData->load(it.value().toMap());
    }

    const QVariantMap& redisLoadDataMap = loadMap["redisData"].toMap();
    for(auto it = redisLoadDataMap.begin(); it != redisLoadDataMap.end(); ++it){
        CustomDataItem* newData = this->createData(CustomDataItem::REDIS);
        newData->load(it.value().toMap());
    }


    //通知数据页更新数据
    this->emit loadNewData();
}
