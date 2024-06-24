#include "customdataitem.h"
#include "designertoolbar.h"
#include "config.h"
#include <QDebug>
#include <QPointer>

CustomDataItem::CustomDataItem(CustomDataItem::DataSource dataSource, QObject *parent) : QObject(parent)
{
    _dataSource = dataSource;
    _uuid = QUuid::createUuid();
    _updateTimer = new QTimer(this);
    _name = "data";
    _value = "null";
    _script = "value = value;";
    _type = STRING;
    _timeStamp = QDateTime::currentDateTime();
    connect(_updateTimer, &QTimer::timeout, this, &CustomDataItem::updateTimeoutSlot);
    _updateTimer->start(DATA_UPDATE_TIME);
}

QScriptEngine& CustomDataItem::getScriptEngine(){
    static QScriptEngine scriptEngine;
    return scriptEngine;
}

void CustomDataItem::updateTimeoutSlot(){
    if(DesignerToolBar::getDesignerToolBar()->getCurToolState() == DesignerToolBar::SHOW_TOOL&&
            _dataSource == DataSource::REDIS){
        //调用接口，读数据，更新时间戳，发送数据更新信号
        //读数据

        //更新时间戳
        _timeStamp = QDateTime::currentDateTime();
        //发送信号
        emit this->dataUpdated();
    }
}

void CustomDataItem::writeData(const QString& data){
    if(_dataSource == DataSource::LOCAL){
        //更新值
        _value = data;
        //更新时间戳
        _timeStamp = QDateTime::currentDateTime();
        //发送信号
        emit this->dataUpdated();
    }
    else if(_dataSource == DataSource::REDIS){
        //调用接口，写数据
    }
}

QString CustomDataItem::getDataAfterScript(){
    //空脚本直接返回
    if(_script == "value = value;" || _script == ""){
        switch (_type) {
        case BOOL:{
            static QString boolValue;
            boolValue = QString::number(_value.toLower() == "true" || _value == "1");
            return boolValue;
            break;
        }
        default:
            return _value;
            break;
        }
    }
    //脚本计算返回
    QScriptEngine& scriptEngine = this->getScriptEngine();
    scriptEngine.evaluate("function cal(value) { " + _script  +  " return value;}");
    QScriptValue cal = scriptEngine.globalObject().property("cal");
    QScriptValueList valueList;
    switch (_type) {
    case STRING:
        valueList << _value;
        break;
    case NUMBER:
        valueList << _value.toDouble();
        break;
    case BOOL:
        valueList <<(_value.toLower() == "true" || _value == "1");
        break;
    default:
        break;
    }
    QScriptValue returnValue = cal.call(QScriptValue(), valueList);
    return returnValue.toString();
}

//保存
QVariantMap CustomDataItem::save(){
    QVariantMap saveMap;

    saveMap["_name"] = _name;
    saveMap["_key"] = _key;
    saveMap["_uuid"] = _uuid;
    saveMap["_value"] = _value;
    saveMap["_dataSource"] = (int)_dataSource;
    saveMap["_timeStamp"] = _timeStamp;
    saveMap["_script"] = _script;
    saveMap["_type"] = (int)_type;

    return saveMap;
}

//读取
void CustomDataItem::load(const QVariantMap& loadMap){

    _name = loadMap["_name"].toString();
    _key = loadMap["_key"].toString();
    _uuid = loadMap["_uuid"].toString();
    _value = loadMap["_value"].toString();
    _dataSource = (DataSource)loadMap["_dataSource"].value<int>();
    _timeStamp = loadMap["_timeStamp"].value<QDateTime>();
    _script = loadMap["_script"].toString();
    _type = (DataType)loadMap["_type"].value<int>();
}
