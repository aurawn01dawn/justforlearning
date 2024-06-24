#include "absscenewidgetitem.h"
#include "config.h"
#include "customscene.h"

#include <QGraphicsSceneMouseEvent>
#include <QPointer>
#include <QPixmapCache>

AbsSceneWidgetItem::AbsSceneWidgetItem(QGraphicsItem *parent):QGraphicsProxyWidget(parent)
{
    //设置uuid
    _uuid = QUuid::createUuid();
    _designerToolBar = DesignerToolBar::getDesignerToolBar();
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &AbsSceneWidgetItem::timeoutSlot);
    _timer->start(COM_UPDATE_TIME);

    this->setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemSendsGeometryChanges |
             QGraphicsItem::ItemUsesExtendedStyleOption |
                   QGraphicsItem::ItemClipsToShape);
}

AbsSceneWidgetItem::~AbsSceneWidgetItem(){
    //释放函数
    _timer->stop();

    for(auto it = _timerFuncList.begin(); it != _timerFuncList.end(); ++it){
        (*it)->deleteLater();
    }
    for(auto it = _mousePressFuncList.begin(); it != _mousePressFuncList.end(); ++it){
        (*it)->deleteLater();
    }
    for(auto it = _mouseReleaseFuncList.begin(); it != _mouseReleaseFuncList.end(); ++it){
        (*it)->deleteLater();
    }

    this->widget()->deleteLater();
}

void AbsSceneWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(_designerToolBar->getCurToolState() == DesignerToolBar::SHOW_TOOL){
        for(auto it = _mousePressFuncList.begin(); it != _mousePressFuncList.end(); ++it){
            QPointer<CustomFunction> func = *it;
            func->run();
        }
    }

    // 选择鼠标工具
    else if(_designerToolBar->getCurToolState() == DesignerToolBar::MOUSE_TOOL){
        // 如果鼠标左键按下  单击
        if(event->button()&Qt::LeftButton)
        {
            // 求差值=鼠标当前位置-窗口左上角点
            _moveOffset = event->scenePos()-this->scenePos();  //geometry()是矩形窗口，topLeft()是左上角的信息。
        }
    }
    if(_designerToolBar->getCurToolState() != DesignerToolBar::SHOW_TOOL)
        emit this->itemStateChange(QPointer<AbsSceneWidgetItem>(this));
    QGraphicsItem::mousePressEvent(event);
}

void AbsSceneWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(_designerToolBar->getCurToolState() == DesignerToolBar::SHOW_TOOL){
        for(auto it = _mouseReleaseFuncList.begin(); it != _mouseReleaseFuncList.end(); ++it){
            QPointer<CustomFunction> func = *it;
            func->run();
        }
    }
    // 选择删除工具
    else if(_designerToolBar->getCurToolState() == DesignerToolBar::DELETE_TOOL){
        // 如果鼠标左键按下  单击
        if(event->button()&Qt::LeftButton)
        {
            // 调用该对象析构
            QGraphicsScene* gScene = this->scene();
            QPointer<CustomScene> cScene = nullptr;
            if(gScene)cScene = dynamic_cast<CustomScene*>(gScene);
            if(!cScene.isNull()){
                cScene->deleteSceneItem(this);
            }
        }
    }
    else if(_designerToolBar->getCurToolState() == DesignerToolBar::UP_TOOL){
        // 如果鼠标左键按下  单击
        if(event->button()&Qt::LeftButton)
        {
            this->setZValue(this->zValue()+1);
        }
    }
    else if(_designerToolBar->getCurToolState() == DesignerToolBar::DOWN_TOOL){
        // 如果鼠标左键按下  单击
        if(event->button()&Qt::LeftButton)
        {
            this->setZValue(this->zValue()-1);
        }
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void AbsSceneWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    // 选择鼠标工具
    if(_designerToolBar->getCurToolState() == DesignerToolBar::MOUSE_TOOL){
        //只允许左键拖动   持续的动作
        if(event->buttons()&Qt::LeftButton)  //buttons处理的是长事件，button处理的是短暂的事件
        {
            //窗口跟随鼠标移动，窗口的新位置=鼠标当前位置-差值
            this->setPos(event->scenePos()-_moveOffset);

        }
    }

    // 选择缩放工具
     if(_designerToolBar->getCurToolState() == DesignerToolBar::SCALE_TOOL){
        // 如果鼠标左键按下  单击
        if(event->buttons()&Qt::LeftButton)
        {
            qreal offset = (event->lastScenePos().x() - event->scenePos().x()) / 200.0;
            this->setScale(this->scale()-offset);
        }
    }

    // 旋转旋转工具
    else if(_designerToolBar->getCurToolState() == DesignerToolBar::ROTATE_TOOL){
        // 如果鼠标左键按下  单击
        if(event->buttons()&Qt::LeftButton)
        {
            qreal offset = (event->lastScenePos().x() - event->scenePos().x()) / 2.0;
            this->setRotation(this->rotation() - offset);
        }
    }
    if(_designerToolBar->getCurToolState() != DesignerToolBar::SHOW_TOOL)
        emit this->itemStateChange(QPointer<AbsSceneWidgetItem>(this));
    QGraphicsItem::mouseMoveEvent(event);
}

void AbsSceneWidgetItem::timeoutSlot(){
    if(_designerToolBar->getCurToolState() == DesignerToolBar::SHOW_TOOL){
        for(auto it = _timerFuncList.begin(); it != _timerFuncList.end(); ++it){
            QPointer<CustomFunction>func = *it;
            func->run();
        }
    }
}

//保存
QVariantMap AbsSceneWidgetItem::save(){
    QVariantMap saveMap;

    saveMap["_uuid"] = QVariant::fromValue(_uuid);
    saveMap["_comType"] = QVariant::fromValue((int)_comType);
    saveMap["x"] = this->pos().x();
    saveMap["y"] = this->pos().y();
    saveMap["z"] = this->zValue();
    saveMap["rotation"] = this->rotation();
    saveMap["scale"] = this->scale();

    int index = 0;
    QVariantMap timerFuncMap;
    for(auto it = _timerFuncList.begin(); it != _timerFuncList.end(); ++it){
        timerFuncMap.insert(QString::number(index++),(*it)->save());
    }
    saveMap["_timerFuncMap"] = QVariant::fromValue(timerFuncMap);

    QVariantMap mousePressFuncMap;
    for(auto it = _mousePressFuncList.begin(); it != _mousePressFuncList.end(); ++it){
        mousePressFuncMap.insert(QString::number(index++), (*it)->save());
    }
    saveMap["_mousePressFuncMap"] = QVariant::fromValue(mousePressFuncMap);

    QVariantMap mouseReleaseFuncMap;
    for(auto it = _mouseReleaseFuncList.begin(); it != _mouseReleaseFuncList.end(); ++it){
        mouseReleaseFuncMap.insert(QString::number(index++), (*it)->save());
    }
    saveMap["_mouseReleaseFuncMap"] = QVariant::fromValue(mouseReleaseFuncMap);

    //不同QWidget需要保存到的属性
    QWidget* com = this->widget();
    saveMap["objectName"] = this->objectName();
    saveMap["width"] = com->width();
    saveMap["height"] = com->height();
    saveMap["_imgPath"] = _imgPath;
    saveMap["styleSheet"] = com->styleSheet();
    switch (_comType) {
    case PUSHBUTTON:{
        QPushButton* ptn = dynamic_cast<QPushButton*>(com);
        saveMap["text"] = QVariant::fromValue(ptn->text());
        saveMap["iconSize"] = QVariant::fromValue(ptn->iconSize().width());
        saveMap["font"] = QVariant::fromValue(ptn->font());
        break;
    }
    case INPUT:{
        QLineEdit* input = dynamic_cast<QLineEdit*>(com);
        saveMap["text"] = QVariant::fromValue(input->text());
        saveMap["font"] = QVariant::fromValue(input->font());
        break;
    }
    case OUTPUT:{
        break;
    }
    case TEXT:{
        QLabel* text = dynamic_cast<QLabel*>(com);
        saveMap["text"] = QVariant::fromValue(text->text());
        saveMap["font"] = QVariant::fromValue(text->font());
        break;
    }
    case IMAGE:{
        break;
    }
    default:
        break;
    }

    return saveMap;
}

//读取
void AbsSceneWidgetItem::load(const QVariantMap& loadMap){

    _uuid = loadMap["_uuid"].value<QUuid>();
    _comType =(ComType)loadMap["_comType"].value<int>();

    const QVariantMap& timerFuncMap = loadMap["_timerFuncMap"].toMap();
    for(auto it = timerFuncMap.begin(); it != timerFuncMap.end(); ++it){
        QPointer<CustomFunction> customFunc = new CustomFunction(this);
        customFunc->load(it.value().toMap());
        _timerFuncList.append(customFunc);
    }

    const QVariantMap& mousePressFuncMap = loadMap["_mousePressFuncMap"].toMap();
    for(auto it = mousePressFuncMap.begin(); it != mousePressFuncMap.end(); ++it){
        QPointer<CustomFunction> customFunc = new CustomFunction(this);
        customFunc->load(it.value().toMap());
        _mousePressFuncList.append(customFunc);
    }

    const QVariantMap& mouseReleaseFuncMap = loadMap["_mouseReleaseFuncMap"].toMap();
    for(auto it = mouseReleaseFuncMap.begin(); it != mouseReleaseFuncMap.end(); ++it){
        QPointer<CustomFunction> customFunc = new CustomFunction(this);
        customFunc->load(it.value().toMap());
        _mouseReleaseFuncList.append(customFunc);
    }

    //创建本身的QWidget
    switch (_comType) {
    case PUSHBUTTON:{
        QPushButton* ptn = new QPushButton();
        ptn->setText(loadMap["text"].toString());
        ptn->setFixedWidth(loadMap["width"].value<int>());
        ptn->setFixedHeight(loadMap["height"].value<int>());
        ptn->setFont(loadMap["font"].value<QFont>());
        _imgPath = loadMap["_imgPath"].value<QString>();
        if(_imgPath != ""){
            QPixmap pixmap;
            if (!QPixmapCache::find(_imgPath, &pixmap)) {
                QFile file(_imgPath);
                if(!file.open(QIODevice::ReadOnly)){
                    qDebug() << "image open error:" + _imgPath;
                  return;
                };
                pixmap.loadFromData(file.readAll());
                QPixmapCache::insert(_imgPath, pixmap);
            }
            ptn->setIcon(QIcon(pixmap));
        }
        ptn->setIconSize(QSize(loadMap["iconSize"].toInt(), loadMap["iconSize"].toInt()));
        ptn->setStyleSheet(loadMap["styleSheet"].value<QString>());
        this->QGraphicsProxyWidget::setWidget(ptn);
        break;
    }
    case INPUT:{
        QLineEdit* input = new QLineEdit();
        input->setFixedWidth(loadMap["width"].value<int>());
        input->setFixedHeight(loadMap["height"].value<int>());
        input->setFont(loadMap["font"].value<QFont>());
        input->setStyleSheet(loadMap["styleSheet"].value<QString>());
        this->QGraphicsProxyWidget::setWidget(input);
        break;
    }
    case OUTPUT:{
        QLCDNumber* output = new QLCDNumber();
        output->setFixedWidth(loadMap["width"].value<int>());
        output->setFixedHeight(loadMap["height"].value<int>());
        output->setFont(loadMap["font"].value<QFont>());
        output->setStyleSheet(loadMap["styleSheet"].value<QString>());
        this->QGraphicsProxyWidget::setWidget(output);
        break;
    }
    case TEXT:{
        QLabel* text = new QLabel();
        text->setFixedWidth(loadMap["width"].value<int>());
        text->setFixedHeight(loadMap["height"].value<int>());
        text->setFont(loadMap["font"].value<QFont>());
        text->setStyleSheet(loadMap["styleSheet"].value<QString>());
        text->setText(loadMap["text"].toString());
        this->QGraphicsProxyWidget::setWidget(text);
        break;
    }
    case IMAGE:{
        QLabel* image = new QLabel();
        image->setFixedWidth(loadMap["width"].value<int>());
        image->setFixedHeight(loadMap["height"].value<int>());
        _imgPath = loadMap["_imgPath"].value<QString>();
        if(_imgPath != ""){
            QPixmap pixmap;
            if (!QPixmapCache::find(_imgPath, &pixmap)) {
                QFile file(_imgPath);
                if(!file.open(QIODevice::ReadOnly)){
                    qDebug() << "image open error:" + _imgPath;
                  return;
                };
                pixmap.loadFromData(file.readAll());
                QPixmapCache::insert(_imgPath, pixmap);
            }
            image->setPixmap(pixmap);
            image->setFixedWidth(pixmap.size().width());
            image->setFixedHeight(pixmap.size().height());
        }
        image->setStyleSheet(loadMap["styleSheet"].value<QString>());
        this->QGraphicsProxyWidget::setWidget(image);
        break;
    }
    default:
        break;
    }

    this->setPos(loadMap["x"].toDouble(), loadMap["y"].toDouble());
    this->setZValue(loadMap["z"].toDouble());
    this->setRotation(loadMap["rotation"].toDouble());
    this->setScale(loadMap["scale"].toDouble());
    this->setObjectName(loadMap["objectName"].value<QString>());
}
