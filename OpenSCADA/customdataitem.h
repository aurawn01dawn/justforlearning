#ifndef CUSTOMDATAITEM_H
#define CUSTOMDATAITEM_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QTimer>
#include <QUuid>
#include <QScriptEngine>

class CustomDataItem : public QObject
{
    Q_OBJECT
public: 
    enum DataSource{
        LOCAL = 0,
        REDIS = 1
    };

    enum DataType{
        STRING = 0,
        NUMBER = 1,
        BOOL = 2
    };


    explicit CustomDataItem(enum DataSource ,QObject *parent = nullptr);

    //获取数据源
    inline const DataSource& getDataSource(){
        return _dataSource;
    }

    //获取原始数据
    inline const QString& getData(){
        return _value;
    }

    //获取脚本处理后的数据
    QString getDataAfterScript();

    //设置数据
    void writeData(const QString& data);

    //获取时间戳
    inline QString getTimeStamp(){
        return _timeStamp.toString("yyyy-MM-dd HH:mm:ss");
    }

    //获取uuid
    inline const QUuid& getUuid(){
        return _uuid;
    }

    //获取key
    inline const QString& getKey(){
        return _key;
    }

    //设置key
    void setKey(const QString& key){
        _key = key;
    }

    //获取名称
    inline const QString& getName(){
        return _name;
    }

    //设置名称
    inline void setName(const QString& name){
        _name = name;
    }

    //获取脚本
    inline const QString& getScript(){return _script;}

    //设置脚本
    inline void setScript(const QString& script){_script = script;}

    //获取类型
    inline DataType getType(){return _type;}

    //设置类型
    inline void setType(DataType type){_type = type;}

    //保存
    QVariantMap save();

    //读取
    void load(const QVariantMap&);

private:
    //name
    QString _name;

    //key
    QString _key;

    //uuid
    QUuid _uuid;

    //data
    QString _value;

    //dataSource
    DataSource _dataSource;

    //timeStamp
    QDateTime _timeStamp;

    //timer
    QTimer * _updateTimer;

    //脚本执行器
    static QScriptEngine& getScriptEngine();
    //脚本
    QString _script;

    //type
    DataType _type;

private slots:
    void updateTimeoutSlot();

signals:
    //数据更新
    void dataUpdated();

};

#endif // CUSTOMDATAITEM_H
