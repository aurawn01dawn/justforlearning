#include "customfunction.h"
#include "absscenewidgetitem.h"
#include "scenesmanager.h"
#include "config.h"

#include <QPixmapCache>
#include <QDebug>

CustomFunction::CustomFunction(AbsSceneWidgetItem* sceneItem, QObject *parent) :QObject(parent),
    _sceneItem(sceneItem)
{
}

//运行函数
void CustomFunction::run(){
    switch (_funcType) {
    case TEST:
        this->testFunc();
        break;
    case SHOW:
        this->showFunc();
        break;
    case SET_VALUE:
        this->setValueFunc();
        break;
    case SET_VALUE_FROM_INPUT:
        this->setValueFromInputFunc();
        break;
    case SET_CUR_SCENE:
        this->setCurScene();
        break;
    case SWITCH_IMAGE:
        this->switchImage();
        break;
    case SWITCH_VALUE:
        this->switchValue();
        break;
    case NONE:
        qDebug() << "none func!";
        break;
    default:
        qDebug() << "bad func!";
        break;
    }
}

//test
void CustomFunction::testFunc(){
    qDebug() << "test Func run!";
}

//show
void CustomFunction::showFunc(){
    //取出参数
    if(_args.size() != 1 ||
            _args[0].value<QPointer<CustomDataItem>>().isNull() ||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    switch (_sceneItem->getType()) {
    case AbsSceneWidgetItem::PUSHBUTTON:
    {
        QPushButton *com = dynamic_cast<QPushButton*>(_sceneItem->widget());
        com->setText(_args[0].value<QPointer<CustomDataItem>>()->getDataAfterScript());
        break;
    }
    case AbsSceneWidgetItem::OUTPUT:
    {
        QLCDNumber *com = dynamic_cast<QLCDNumber*>(_sceneItem->widget());
        com->display(_args[0].value<QPointer<CustomDataItem>>()->getDataAfterScript());
        break;
    }
    case AbsSceneWidgetItem::TEXT:
    {
        QLabel *com = dynamic_cast<QLabel*>(_sceneItem->widget());
        com->setText(_args[0].value<QPointer<CustomDataItem>>()->getDataAfterScript());
        break;
    }
    default:
        break;
    }
}

//set value
void CustomFunction::setValueFunc(){
    //取出参数
    if(_args.size() != 2 ||
            _args[0].value<QPointer<CustomDataItem>>().isNull()||
            !_args[1].canConvert<QString>()||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    QPointer<CustomDataItem> dataItem = _args[0].value<QPointer<CustomDataItem>>();
    dataItem->writeData(_args[1].toString());
}

//set value from input
void CustomFunction::setValueFromInputFunc(){
    //取出参数
    if(_args.size() != 2 ||
           _args[0].value<QPointer<CustomDataItem>>().isNull()||
           _args[1].value<QPointer<AbsSceneWidgetItem>>().isNull()||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    QPointer<AbsSceneWidgetItem> sceneItem = _args[1].value<QPointer<AbsSceneWidgetItem>>();
    QLineEdit* inputEdit = dynamic_cast<QLineEdit*>(sceneItem->widget());
    if(inputEdit){
        QPointer<CustomDataItem> dataItem = _args[0].value<QPointer<CustomDataItem>>();
        dataItem->writeData(inputEdit->text());
    }
}

void CustomFunction::setCurScene(){
    //取出参数
    if(_args.size() != 1 ||
           _args[0].value<QPointer<CustomScene>>().isNull()||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    QPointer<CustomScene> destScene = _args[0].value<QPointer<CustomScene>>();
    //切换当前设计场景
    ScenesManager::getScenesManager()->setCurShowScene(destScene);
}

//switch image
void CustomFunction::switchImage(){
    //取出参数
    if(_args.size() != 3 ||
           _args[0].value<QPointer<CustomDataItem>>().isNull()||
           !_args[1].canConvert<QString>()||
           !_args[2].canConvert<QString>()||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    QPointer<CustomDataItem> dataItem = _args[0].value<QPointer<CustomDataItem>>();
    QString value = _args[1].toString();
    QString imagePath = _args[2].toString();

    //qDebug() << "run";

    //满足条件，切换图片
    if(dataItem->getDataAfterScript() == value){
        //判断是否需要更新图片
        if(_sceneItem->getImgPath() == imagePath)return;
        _sceneItem->setImgPath(imagePath);

        //qDebug() << "switch image";
        //更新图片
        QPixmap pixmap;
        if (!QPixmapCache::find(imagePath, &pixmap)) {
             pixmap.load(imagePath);
             QPixmapCache::insert(imagePath, pixmap);
        }
        QWidget *com = _sceneItem->widget();
        if(_sceneItem->getType() == AbsSceneWidgetItem::PUSHBUTTON){
            QPushButton* ptn = qobject_cast<QPushButton*>(com);
            if(ptn)ptn->setIcon(QIcon(pixmap));
        }else if(_sceneItem->getType() == AbsSceneWidgetItem::IMAGE){
            QLabel* img = qobject_cast<QLabel*>(com);
            if(img){
                img->setPixmap(pixmap);
                img->setFixedWidth(pixmap.size().width());
                img->setFixedHeight(pixmap.size().height());
                img->setStyleSheet("background-color: transparent;");
            }
        }
    }
}

void CustomFunction::switchValue(){
    //取出参数
    if(_args.size() != 2 ||
            _args[0].value<QPointer<CustomDataItem>>().isNull()||
            !_args[1].canConvert<QString>()||
            _sceneItem.isNull()){
        qDebug() << "args num error, num:" + QString::number(_args.size());
        return;
    }

    QPointer<CustomDataItem> dataItem = _args[0].value<QPointer<CustomDataItem>>();
    QStringList values = _args[1].toString().split('#');

    for(auto it = values.begin(); it != values.end(); ++it){
        if(*it == dataItem->getDataAfterScript()){
            it++;
            if(it == values.end()){
                dataItem->writeData(values.first());
            }else{
                dataItem->writeData(*it);
            }
            break;
        }
    }
}

//保存
QVariantMap CustomFunction::save(){
    QVariantMap saveMap;

    //将指针存储为对应的uuid
    int index = 0;
    QVariantMap argsMap;
    for(int i = 0; i < _args.size(); ++i){
        if(_args[i].canConvert<QString>()){
            argsMap.insert(QString::number(index++), _args[i].toString());
        }else if(_args[i].canConvert<QPointer<CustomDataItem>>()){
            argsMap.insert(QString::number(index++), _args[i].value<QPointer<CustomDataItem>>()->getUuid());
        }else if(_args[i].canConvert<QPointer<AbsSceneWidgetItem>>()){
            argsMap.insert(QString::number(index++), _args[i].value<QPointer<AbsSceneWidgetItem>>()->getUuid());
        }else if(_args[i].canConvert<QPointer<CustomScene>>()){
            argsMap.insert(QString::number(index++), _args[i].value<QPointer<CustomScene>>()->getUuid());
        }
    }
    saveMap["_args"] = argsMap;
    saveMap["_funcType"] = QVariant::fromValue((int)_funcType);

    return saveMap;
}

//读取
void CustomFunction::load(const QVariantMap& loadMap){
    //睡两秒，等其他对象存储完成将uuid转换为指针
    QTimer::singleShot(CONVERT_UUID_TO_POINTER_TIME, this, [=]{
        QVariantMap argsMap = loadMap["_args"].toMap();
        QVector<QVariant> argsVec;
        for(auto it = argsMap.begin(); it != argsMap.end(); ++it){
            if(it.value().canConvert<QString>()){
                QUuid uuid = it.value().value<QUuid>();
                QPointer<CustomDataItem> dataItem = DataManager::getDataManager()->getDataFromUuid(uuid);
                QPointer<CustomScene> sceneItem = ScenesManager::getScenesManager()->getSceneFromUuid(uuid);
                QPointer<AbsSceneWidgetItem> absSceneItem = ScenesManager::getScenesManager()->getSceneItemFromUuid(uuid);
                if(!dataItem.isNull())argsVec.push_back(QVariant::fromValue(dataItem));
                else if(!sceneItem.isNull())argsVec.push_back(QVariant::fromValue(sceneItem));
                else if(!absSceneItem.isNull())argsVec.push_back(QVariant::fromValue(absSceneItem));
                else argsVec.push_back(it.value());
            }
        }

        this->_args = argsVec;
        _funcType =(FuncType)loadMap["_funcType"].value<int>();
    });

}
