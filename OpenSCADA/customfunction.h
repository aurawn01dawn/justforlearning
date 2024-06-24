#ifndef CUSTOMFUNCTION_H
#define CUSTOMFUNCTION_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QPointer>

#include "datamanager.h"


class AbsSceneWidgetItem;

class CustomFunction : public QObject
{
    Q_OBJECT
public:
    enum FuncType{
        NONE = -1,
        TEST = 0,
        SHOW = 1,
        SET_VALUE = 2,
        SET_VALUE_FROM_INPUT = 3,
        SET_CUR_SCENE = 4,
        SWITCH_IMAGE = 5,
        SWITCH_VALUE = 6
    };

    explicit CustomFunction(AbsSceneWidgetItem*_sceneItem, QObject *parent = nullptr);

    //运行函数
    void run();

    //获取函数参数
    inline const QVector<QVariant>& getFuncArgs(){return _args;}

    //设置函数参数
    inline void setFuncArgs(const QVector<QVariant>& funcArgs){_args = funcArgs;}

    //获取函数类型
    inline FuncType getFuncType(){return _funcType;}

    //设置函数类型
    inline void setFuncType(FuncType type){_funcType = type;}

    //获取对应物体
    inline QPointer<AbsSceneWidgetItem> getSceneItem(){return _sceneItem;}

    //保存
    QVariantMap save();

    //读取
    void load(const QVariantMap& loadMap);

private:
    //函数参数
    QVector<QVariant> _args;
    //函数类型
    FuncType _funcType = NONE;
    //函数所属物体
    QPointer<AbsSceneWidgetItem> _sceneItem;

    //test
    void testFunc();

    //show
    void showFunc();

    //set value
    void setValueFunc();

    //set value from input
    void setValueFromInputFunc();

    //set cur scene
    void setCurScene();

    //switch image
    void switchImage();

    //switch value
    void switchValue();

signals:

};

#endif // CUSTOMFUNCTION_H
