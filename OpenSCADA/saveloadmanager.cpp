#include "saveloadmanager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QTextCodec>

SaveLoadManager::SaveLoadManager(QObject *parent) : QObject(parent)
{
    _scenesManager = ScenesManager::getScenesManager();
    _dataManager = DataManager::getDataManager();
}

void SaveLoadManager::load(const QString& filePath){
    // 打开保存的comJson文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly |
                   QIODevice::ExistingOnly)){
        qDebug() <<"布局文件打开失败：" << file.errorString();
        file.close();   // 关闭file
        return;
    }

    //反序列化
    QJsonParseError jerr;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jerr);
    file.close();   // 关闭file
    if (doc.isNull()){
        qDebug() <<"布局读取失败：" << filePath << " 错误原因：" << jerr.errorString();
        return;
    }
    QJsonObject jsonObj = doc.object();

    //读取哈希文件
    QVariantMap dataMap = jsonObj["dataMap"].toVariant().toMap();
    _dataManager->load(dataMap);

    QVariantMap scenesMap = jsonObj["scenesMap"].toVariant().toMap();
    _scenesManager->load(scenesMap);
}

void SaveLoadManager::save(const QString& filePath){
    //获取保存数据Map
    QVariantMap dataMap = _dataManager->save();
    QVariantMap scenesMap = _scenesManager->save();
    //序列化哈希数据
    QJsonObject jsonObj;
    jsonObj.insert("dataMap", QJsonValue(QJsonObject::fromVariantMap(dataMap)));
    jsonObj.insert("scenesMap", QJsonValue(QJsonObject::fromVariantMap(scenesMap)));
    QJsonDocument document;
    document.setObject(jsonObj);

    QByteArray jsonbyteArray = document.toJson(QJsonDocument::Indented);
    QFile file(filePath);
    file.remove();
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))qDebug() << "file error!";
    QTextStream in(&file);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    in.setCodec(codec); // 设置编码为UTF-8
    in << jsonbyteArray;
    file.close();   // 关闭file
}
