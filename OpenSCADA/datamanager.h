#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QDebug>
#include <QPointer>

#include "customdataitem.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager* getDataManager(QObject *parent = nullptr);

    //创建数据
    inline QPointer<CustomDataItem> createData(CustomDataItem::DataSource dataSource){
        QPointer<CustomDataItem> newData = new CustomDataItem(dataSource, this);
        if(newData->getDataSource() == CustomDataItem::LOCAL){
            _localList.append(newData);
        }
        else if(newData->getDataSource() == CustomDataItem::REDIS){
            _redisList.append(newData);
        }
        _allList.append(newData);
        return newData;
    }

    //删除数据
    inline void deleteData(QPointer<CustomDataItem> deleteItem){
        if(!_allList.contains(deleteItem)){
            qDebug() << "allMap don't contains DataItem :" + deleteItem->getName();
            return;
        }
        _allList.removeAll(deleteItem);
        if(_localList.contains(deleteItem)){
            deleteItem->deleteLater();
            _localList.removeAll(deleteItem);
        }
        if(_redisList.contains(deleteItem)){
            deleteItem->deleteLater();
            _redisList.removeAll(deleteItem);
        }
    }

    //获取本地数据
    const QList<QPointer<CustomDataItem>>& getLocalDataList(){
        return _localList;
    }

    //获取redis数据
    const QList<QPointer<CustomDataItem>>& getRedisDataList(){
        return _redisList;
    }

    //获取所有数据
    const QList<QPointer<CustomDataItem>>& getAllDataList(){
        return _allList;
    }

    //从Uuid获取对象
    QPointer<CustomDataItem> getDataFromUuid(const QUuid& uuid){
        for(auto it = _allList.begin(); it != _allList.end(); ++it){
            if((*it)->getUuid() == uuid){
                return *it;
            }
        }
        return nullptr;
    }

    //保存
    QVariantMap save();

    //读取
    void load(const QVariantMap&);

private:
    explicit DataManager(QObject *parent = nullptr);
    //reids
    QList<QPointer<CustomDataItem>> _redisList;
    //local
    QList<QPointer<CustomDataItem>> _localList;
    //all
    QList<QPointer<CustomDataItem>> _allList;

signals:

    void loadNewData();

};

#endif // DATAMANAGER_H
