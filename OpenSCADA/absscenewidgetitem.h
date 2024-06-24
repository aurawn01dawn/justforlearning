#ifndef ABSSCENEWIDGETITEM_H
#define ABSSCENEWIDGETITEM_H

#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QLCDNumber>
#include <QDebug>
#include <QTimer>
#include <QUuid>
#include <QPointer>



#include "designertoolbar.h"
#include "customfunction.h"

class AbsSceneWidgetItem : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    AbsSceneWidgetItem(QGraphicsItem *parent = nullptr);
    ~AbsSceneWidgetItem();
    //控件类型
    enum ComType{
        NULLCOM = -1,
        PUSHBUTTON = 0,
        INPUT = 1,
        OUTPUT = 2,
        TEXT = 3,
        IMAGE = 4
    };

    //设置控件
    inline void setWidget(ComType comtype ,QWidget* widget){
        _comType = comtype;
        this->QGraphicsProxyWidget::setWidget(widget);
    }

    //返回uuid
    inline const QUuid& getUuid(){
        return _uuid;
    }

    //返回类型
    inline ComType getType(){
        return _comType;
    }

    //返回点击函数
    inline const QList<QPointer<CustomFunction>>& getPressFuncList(){return _mousePressFuncList;}

    //返回释放函数
    inline const QList<QPointer<CustomFunction>>& getReleaseFuncList(){return _mouseReleaseFuncList;}

    //返回定时函数
    inline const QList<QPointer<CustomFunction>>& getTimerFuncList(){return _timerFuncList;}

    //创建函数类型
    enum FunctionType{
        PRESS = 0,
        RELEASE = 1,
        TIMER = 2
    };
    //创建空函数
    inline QPointer<CustomFunction> createFunction(FunctionType type){
        QPointer<CustomFunction> newFunction = new CustomFunction(this, this);
        switch (type) {
        case PRESS:
            _mousePressFuncList.push_back(newFunction);
            break;
        case RELEASE:
            _mouseReleaseFuncList.push_back(newFunction);
            break;
        case TIMER:
            _timerFuncList.push_back(newFunction);
            break;
        default:
            break;
        }
        return newFunction;
    }

    //删除函数
    inline void deleteFunction(QPointer<CustomFunction> deleteFunc){
        _timerFuncList.removeAll(deleteFunc);
        _mousePressFuncList.removeAll(deleteFunc);
        _mouseReleaseFuncList.removeAll(deleteFunc);
        deleteFunc->deleteLater();
    }

    //获取图标/图片路径
    inline const QString& getImgPath(){return _imgPath;}

    //设置图标/图片路径
    inline void setImgPath(const QString& imgPath){_imgPath = imgPath;}

    //保存
    QVariantMap save();

    //读取
    void load(const QVariantMap& loadMap);


protected:
    //重写
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    //自身
    QUuid _uuid;
    ComType _comType = NULLCOM;
    QString _imgPath;
    DesignerToolBar* _designerToolBar;
    QPointF _moveOffset; //用于拖动时鼠标补差

    //定时器，用于显示轮询读取
    QTimer * _timer;
    //显示函数，定时器执行
    QList<QPointer<CustomFunction>> _timerFuncList;

    //点击函数
    QList<QPointer<CustomFunction>> _mousePressFuncList;

    //释放函数
    QList<QPointer<CustomFunction>> _mouseReleaseFuncList;


private slots:

    void timeoutSlot();

signals:
    void itemStateChange(QPointer<AbsSceneWidgetItem>);
};

#endif // ABSSCENEWIDGETITEM_H
