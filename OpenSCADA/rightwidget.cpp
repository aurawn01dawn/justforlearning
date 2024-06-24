#include "rightwidget.h"
#include "scenesmanager.h"


#include <QVBoxLayout>
#include <QHeaderView>
#include <QFontDatabase>
#include <QFileDialog>
#include <QPointer>

RightWidget::RightWidget(QWidget *parent)
    : QWidget{parent}
{
    createInit();
    layoutInit();
    connectInit();
}

void RightWidget::createInit(){
    _sceneManager = ScenesManager::getScenesManager();

    _treeWidget = new QTreeWidget(this);

    _pixTopItem = new QTreeWidgetItem(_treeWidget,QStringList()<<"图元属性");
    _comTopItem = new QTreeWidgetItem(_treeWidget,QStringList()<<"控件属性");
    _InteTopItem = new QTreeWidgetItem(_treeWidget,QStringList()<<"交互属性");

    _posXSubItem = new QTreeWidgetItem(_pixTopItem,QStringList()<<"位置x"<<"");
    _posYSubItem = new QTreeWidgetItem(_pixTopItem,QStringList()<<"位置y"<<"");
    _scaleSubItem = new QTreeWidgetItem(_pixTopItem,QStringList()<<"大小"<<"");
    _rotateSubItem = new QTreeWidgetItem(_pixTopItem,QStringList()<<"旋转"<<"");

    _posXLineEdit = new QSpinBox(this);
    _posXLineEdit->setMaximum(10000);
    _posXLineEdit->setMinimum(-10000);
    _posYLineEdit = new QSpinBox(this);
    _posYLineEdit->setMaximum(10000);
    _posYLineEdit->setMinimum(-10000);
    _scaleLineEdit = new QDoubleSpinBox(this);
    _scaleLineEdit->setSingleStep(0.1);
    _rotateLineEdit = new QDoubleSpinBox(this);
    _rotateLineEdit->setSingleStep(1);
    _rotateLineEdit->setMaximum(10000);
    _rotateLineEdit->setMinimum(-10000);

    _imageSelectWidget = ImageSelectWidget::instance(this);
}

void RightWidget::layoutInit(){
    _treeWidget->setColumnCount(2);
    _treeWidget->setHeaderLabels(QStringList() << "属性" << "值");

    QHeaderView *header = _treeWidget->header();
    // 设置列宽比例，这里设置为30%, 40%, 30%
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->resizeSection(0, header->length() * 0.5);
    header->resizeSection(1, header->length() * 0.5);

    _treeWidget->setItemWidget(_posXSubItem,1,_posXLineEdit);
    _treeWidget->setItemWidget(_posYSubItem,1,_posYLineEdit);
    _treeWidget->setItemWidget(_scaleSubItem,1,_scaleLineEdit);
    _treeWidget->setItemWidget(_rotateSubItem,1,_rotateLineEdit);

    _funcConfigWidget = new FuncConfigWidget(this);
    QTreeWidgetItem* tempSubItem = new QTreeWidgetItem(_InteTopItem, QStringList()<<"按下事件"<<"");
    _pressConfigPtn = new QPushButton("配置", this);
    _treeWidget->setItemWidget(tempSubItem,1, _pressConfigPtn);
    connect(_pressConfigPtn, &QPushButton::clicked, this, [this]{
        if(_currentItem == nullptr || _currentItem.isNull())return;
        _funcConfigWidget->loadFuncShow(_currentItem, AbsSceneWidgetItem::PRESS);
    });

    QTreeWidgetItem* tempSubItem1 = new QTreeWidgetItem(_InteTopItem, QStringList()<<"释放事件"<<"");
    _releaseConfigPtn = new QPushButton("配置", this);
    _treeWidget->setItemWidget(tempSubItem1,1, _releaseConfigPtn);
    connect(_releaseConfigPtn, &QPushButton::clicked, this, [this]{
        if(_currentItem == nullptr || _currentItem.isNull())return;
        _funcConfigWidget->loadFuncShow(_currentItem, AbsSceneWidgetItem::RELEASE);
    });

    QTreeWidgetItem* tempSubItem2 = new QTreeWidgetItem(_InteTopItem,QStringList()<<"定时事件"<<"");
    _timerConfigPtn = new QPushButton("配置", this);
    _treeWidget->setItemWidget(tempSubItem2,1, _timerConfigPtn);
    connect(_timerConfigPtn, &QPushButton::clicked, this, [this]{
        if(_currentItem == nullptr || _currentItem.isNull())return;
        _funcConfigWidget->loadFuncShow(_currentItem, AbsSceneWidgetItem::TIMER);
    });



    QTreeWidgetItem* newSubItem = new QTreeWidgetItem(_comTopItem,QStringList()<<"ObjectName"<<"");
    _comObjNameEdit = new QLineEdit(this);
    _treeWidget->setItemWidget(newSubItem,1,_comObjNameEdit);

    QTreeWidgetItem* newSubItem1 = new QTreeWidgetItem(_comTopItem,QStringList()<<"Text"<<"");
    _textEdit = new QLineEdit(this);
    _treeWidget->setItemWidget(newSubItem1,1,_textEdit);

    QTreeWidgetItem* newSubItem2 = new QTreeWidgetItem(_comTopItem,QStringList()<<"Font"<<"");
    _fontBox = new QComboBox(this);
    _fontBox->addItems(QFontDatabase().families());
    _fontBox->setCurrentText("Arial");
    _treeWidget->setItemWidget(newSubItem2,1, _fontBox);

    QTreeWidgetItem* newSubItem3 = new QTreeWidgetItem(_comTopItem,QStringList()<<"FontWidth"<<"");
    _fontWidthBox = new QSpinBox(this);
    _treeWidget->setItemWidget(newSubItem3,1, _fontWidthBox);

    QTreeWidgetItem* newSubItem4 = new QTreeWidgetItem(_comTopItem,QStringList()<<"Icon/Image"<<"");
    _iconImagePtn = new QPushButton("配置", this);
    _treeWidget->setItemWidget(newSubItem4,1, _iconImagePtn);

    QTreeWidgetItem* newSubItem4_1 = new QTreeWidgetItem(_comTopItem,QStringList()<<"IconSize"<<"");
    _iconSizeBox = new QSpinBox(this);
    _iconSizeBox->setMaximum(9999);
    _treeWidget->setItemWidget(newSubItem4_1,1, _iconSizeBox);

    QTreeWidgetItem* newSubItem5 = new QTreeWidgetItem(_comTopItem,QStringList()<<"Width"<<"");
    _widthBox = new QSpinBox(this);
    _widthBox->setMaximum(9999);
    _treeWidget->setItemWidget(newSubItem5,1, _widthBox);

    QTreeWidgetItem* newSubItem6 = new QTreeWidgetItem(_comTopItem,QStringList()<<"Height"<<"");
    _heightBox = new QSpinBox(this);
    _heightBox->setMaximum(9999);
    _treeWidget->setItemWidget(newSubItem6,1, _heightBox);

    QTreeWidgetItem* newSubItem7 = new QTreeWidgetItem(_comTopItem,QStringList()<<"StyleSheet"<<"");
    _styleSheetEdit = new QLineEdit(this);
    _treeWidget->setItemWidget(newSubItem7,1, _styleSheetEdit);


    _treeWidget->setItemsExpandable(true);		//子Item是否可以被展开
    _treeWidget->expandAll();


    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    QLabel* label = new QLabel(this);
    label->setText("控件属性栏");
    vBoxLayout->addWidget(label);
    vBoxLayout->addWidget(_treeWidget);
    this->setLayout(vBoxLayout);
}

void RightWidget::connectInit(){
    connect(_sceneManager, &ScenesManager::sceneItemChange, this, &RightWidget::updataShowSlot);
    connect(_posXLineEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_posYLineEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_scaleLineEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_rotateLineEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_comObjNameEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_textEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_fontWidthBox, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_widthBox, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_heightBox, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_fontBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateComSlot()));
    connect(_iconImagePtn, &QPushButton::clicked, this, [this]{
        if(_currentItem == nullptr || _currentItem.isNull())return;
        _imageSelectWidget->showFromSceneItem(_currentItem);
    });
    connect(_iconSizeBox, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));
    connect(_styleSheetEdit, SIGNAL(editingFinished()), this, SLOT(updateComSlot()));


}

void RightWidget::updataShowSlot(QPointer<AbsSceneWidgetItem> sceneItem){
    if(sceneItem == nullptr || sceneItem.isNull())return;
    _currentItem = sceneItem;
    //显示图元属性
    _posXLineEdit->setValue(_currentItem->x());
    _posYLineEdit->setValue(_currentItem->y());
    _scaleLineEdit->setValue(_currentItem->scale());
    _rotateLineEdit->setValue(_currentItem->rotation());

    //显示通用属性
    _comObjNameEdit->setText(sceneItem->objectName());

    //依据类别显示不同的固定属性
    QWidget* item = _currentItem->widget();
    if(_currentItem->getType() == AbsSceneWidgetItem::PUSHBUTTON){
        QPushButton* com = dynamic_cast<QPushButton*>(item);
        //text
        _textEdit->setText(com->text());
        //font
        _fontBox->setCurrentText(com->font().family());
        //fontWidth
        _fontWidthBox->setValue(com->font().weight());
        //iconSize
        _iconSizeBox->setValue(com->iconSize().width());
        //width
        _widthBox->setValue(com->width());
        //height
        _heightBox->setValue(com->height());
        //styleSheet
        _styleSheetEdit->setText(com->styleSheet());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::INPUT){
        QLineEdit* com = dynamic_cast<QLineEdit*>(item);
        //text
        _textEdit->setText(com->text());
        //font
        _fontBox->setCurrentText(com->font().family());
        //fontWidth
        _fontWidthBox->setValue(com->font().weight());
        //width
        _widthBox->setValue(com->width());
        //height
        _heightBox->setValue(com->height());
        //styleSheet
        _styleSheetEdit->setText(com->styleSheet());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::OUTPUT){
        QLCDNumber* com = dynamic_cast<QLCDNumber*>(item);
        //font
        _fontBox->setCurrentText(com->font().family());
        //fontWidth
        _fontWidthBox->setValue(com->font().weight());
        //width
        _widthBox->setValue(com->width());
        //height
        _heightBox->setValue(com->height());
        //styleSheet
        _styleSheetEdit->setText(com->styleSheet());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::TEXT){
        QLabel* com = dynamic_cast<QLabel*>(item);
        //text
        _textEdit->setText(com->text());
        //font
        _fontBox->setCurrentText(com->font().family());
        //fontWidth
        _fontWidthBox->setValue(com->font().weight());
        //width
        _widthBox->setValue(com->width());
        //height
        _heightBox->setValue(com->height());
        //styleSheet
        _styleSheetEdit->setText(com->styleSheet());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::IMAGE){
        QLabel* com = dynamic_cast<QLabel*>(item);
//        //width
//        _widthBox->setValue(com->width());
//        //height
//        _heightBox->setValue(com->height());
        //styleSheet
        _styleSheetEdit->setText(com->styleSheet());
    }

}


void RightWidget::updateComSlot(){
    if(_currentItem == nullptr || _currentItem.isNull())return;

    //依据类别设置不同的固定属性
    QWidget* item = _currentItem->widget();
    if(_currentItem->getType() == AbsSceneWidgetItem::PUSHBUTTON){
        QPushButton* com = dynamic_cast<QPushButton*>(item);
        //text
        com->setText(_textEdit->text());
        //font
        QFont font(com->font());
        font.setFamily(_fontBox->currentText());
        //fontWidth
        font.setWeight(_fontWidthBox->value());
        com->setFont(font);
        //iconsize
        com->setIconSize(QSize(_iconSizeBox->value(), _iconSizeBox->value()));
        //width
        com->setFixedWidth(_widthBox->value());
        //height
        com->setFixedHeight(_heightBox->value());
        //styleSheet
        com->setStyleSheet(_styleSheetEdit->text());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::INPUT){
        QLineEdit* com = dynamic_cast<QLineEdit*>(item);
        //text
        com->setText(_textEdit->text());
        //font
        QFont font(com->font());
        font.setFamily(_fontBox->currentText());
        //fontWidth
        font.setWeight(_fontWidthBox->value());
        com->setFont(font);
        //width
        com->setFixedWidth(_widthBox->value());
        //height
        com->setFixedHeight(_heightBox->value());
        //styleSheet
        com->setStyleSheet(_styleSheetEdit->text());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::OUTPUT){
        QLCDNumber* com = dynamic_cast<QLCDNumber*>(item);
        //font
        QFont font(com->font());
        font.setFamily(_fontBox->currentText());
        //fontWidth
        font.setWeight(_fontWidthBox->value());
        com->setFont(font);
        //width
        com->setFixedWidth(_widthBox->value());
        //height
        com->setFixedHeight(_heightBox->value());
        //styleSheet
        com->setStyleSheet(_styleSheetEdit->text());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::TEXT){
        QLabel* com = dynamic_cast<QLabel*>(item);
        //text
        com->setText(_textEdit->text());
        //font
        QFont font(com->font());
        font.setFamily(_fontBox->currentText());
        //fontWidth
        font.setWeight(_fontWidthBox->value());
        com->setFont(font);
        //width
        com->setFixedWidth(_widthBox->value());
        //height
        com->setFixedHeight(_heightBox->value());
        //styleSheet
        com->setStyleSheet(_styleSheetEdit->text());
    }

    else if(_currentItem->getType() == AbsSceneWidgetItem::IMAGE){
        QLabel* com = dynamic_cast<QLabel*>(item);
//        //width
//        com->setFixedWidth(_widthBox->value());
//        //height
//        com->setFixedHeight(_heightBox->value());
        //styleSheet
        com->setStyleSheet(_styleSheetEdit->text());
    }

    //设置图元属性
    _currentItem->setPos(_posXLineEdit->value(), _posYLineEdit->value());
    _currentItem->setScale(_scaleLineEdit->value());
    _currentItem->setRotation(_rotateLineEdit->value());

    //设置通用属性
    _currentItem->setObjectName(_comObjNameEdit->text());
}
