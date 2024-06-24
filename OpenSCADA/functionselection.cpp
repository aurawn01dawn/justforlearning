#include "functionselection.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCompleter>
#include <QGraphicsScene>
#include <QPointer>

#include "absscenewidgetitem.h"
#include "imageselectwidget.h"

FunctionSelection::FunctionSelection(CustomFunction* function, QWidget *parent) :
    QWidget(parent),
    _customFunc(function),
    _dataManager(DataManager::getDataManager()),
    _sceneManager(ScenesManager::getScenesManager()),
    _allDataItemList(_dataManager->getAllDataList())
{
    _selectBox = new QComboBox(this);
    _selectBox->addItems(QStringList() << "测试" << "显示" << "设置值" << "从输入设置值" << "切换场景" << "切换图片" << "切换值");
    _stackedWidget = new QStackedWidget(this);
    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(_selectBox);
    hBoxLayout->addWidget(_stackedWidget);
    this->setLayout(hBoxLayout);

    connect(_selectBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FunctionSelection::selectBoxChangedSlot);

    //testWidget
    _testWidget = new QWidget(this);
    _stackedWidget->insertWidget(0, _testWidget);

    //showWidget
    _showWidget = new QWidget(this);
    _showComboBox = new QComboBox(this);

    for(auto it = _allDataItemList.begin(); it != _allDataItemList.end(); ++it){
        _showComboBox->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomDataItem>(*it)));
    }
    _showComboBox->setEditable(true);
    QCompleter* pCompleter = new QCompleter(_showComboBox->model());
    pCompleter->setFilterMode(Qt::MatchContains);
    _showComboBox->setCompleter(pCompleter);
    _showComboBox->setCurrentIndex(-1);
    QLabel *label = new QLabel(this);
    label->setText("显示数据");
    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(label);
    vBoxLayout->addWidget(_showComboBox);
    _showWidget->setLayout(vBoxLayout);
    _stackedWidget->insertWidget(1, _showWidget);

    //setValueWidget
    _setValueWidget = new QWidget(this);
    _setValueBox = new QComboBox(this);
    _valueEdit = new QLineEdit(this);

    QGridLayout *gridLayout = new QGridLayout;
    QLabel* label1 = new QLabel(this);
    label1->setText("数据名");
    QLabel* label2 = new QLabel(this);
    label2->setText("设置值");
    gridLayout->addWidget(label1, 0, 0);
    gridLayout->addWidget(label2,0, 1);
    gridLayout->addWidget(_setValueBox ,1, 0);
    gridLayout->addWidget(_valueEdit ,1, 1);
    _setValueWidget->setLayout(gridLayout);

    for(auto it = _allDataItemList.begin(); it != _allDataItemList.end(); ++it){
        _setValueBox->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomDataItem>(*it)));
    }
    _setValueBox->setEditable(true);
    QCompleter* pCompleter1 = new QCompleter(_setValueBox->model());
    pCompleter1->setFilterMode(Qt::MatchContains);
    _setValueBox->setCompleter(pCompleter1);
    _setValueBox->setCurrentIndex(-1);

    _stackedWidget->insertWidget(2, _setValueWidget);

    //setValueFromInput
    _setValueFromIputWidget = new QWidget(this);
    _setValueFromInputBox = new QComboBox(this);
    _inputBox = new QComboBox(this);

    QGridLayout *gridLayout1 = new QGridLayout;
    QLabel* label3 = new QLabel(this);
    label3->setText("数据名");
    QLabel* label4 = new QLabel(this);
    label4->setText("输入控件");
    gridLayout1->addWidget(label3, 0, 0);
    gridLayout1->addWidget(label4,0, 1);
    gridLayout1->addWidget(_setValueFromInputBox ,1, 0);
    gridLayout1->addWidget(_inputBox ,1, 1);
    _setValueFromIputWidget->setLayout(gridLayout1);

    for(auto it = _allDataItemList.begin(); it != _allDataItemList.end(); ++it){
        _setValueFromInputBox->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomDataItem>(*it)));
    }
    _setValueFromInputBox->setEditable(true);
    QCompleter* pCompleter2 = new QCompleter(_setValueFromInputBox->model());
    pCompleter2->setFilterMode(Qt::MatchContains);
    _setValueFromInputBox->setCompleter(pCompleter2);
    _setValueFromInputBox->setCurrentIndex(-1);

    const QList<QGraphicsItem*> curSceneItemList = _customFunc->getSceneItem()->scene()->items();
    for(auto it = curSceneItemList.begin(); it != curSceneItemList.end(); ++it){
        AbsSceneWidgetItem* absItem = dynamic_cast<AbsSceneWidgetItem*>(*it);
        if(!absItem)continue;
        if(absItem->getType() == AbsSceneWidgetItem::INPUT){
            _inputBox->addItem(absItem->objectName(), QVariant::fromValue(QPointer<AbsSceneWidgetItem>(absItem)));
        }
    }
    _inputBox->setEditable(true);
    QCompleter* pCompleter3 = new QCompleter(_inputBox->model());
    pCompleter3->setFilterMode(Qt::MatchContains);
    _inputBox->setCompleter(pCompleter3);
    _inputBox->setCurrentIndex(-1);

    _stackedWidget->insertWidget(3, _setValueFromIputWidget);

    //setCurScene
    _setCurSceneWidget = new QWidget(this);
    _curSceneInput = new QComboBox(this);
    QLabel* label5 = new QLabel(this);
    label5->setText("目标场景");

    QVBoxLayout* vBoxLayout1 = new QVBoxLayout;
    vBoxLayout1->addWidget(label5);
    vBoxLayout1->addWidget(_curSceneInput);
    _setCurSceneWidget->setLayout(vBoxLayout1);

    const QList<QPointer<CustomScene>>& sceneList = _sceneManager->getAllSceneList();
    for(auto it = sceneList.begin(); it != sceneList.end(); ++it){
        _curSceneInput->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomScene>(*it)));
    }
    _curSceneInput->setEditable(true);
    QCompleter* pCompleter4 = new QCompleter(_curSceneInput->model());
    pCompleter2->setFilterMode(Qt::MatchContains);
    _curSceneInput->setCompleter(pCompleter4);
    _curSceneInput->setCurrentIndex(-1);

    _stackedWidget->insertWidget(4, _setCurSceneWidget);

    //switch image widget
    _switchImageWidget = new QWidget(this);
    _setDataBox = new QComboBox(this);
    _valueInputEdit = new QLineEdit(this);
    _imageSelectPtn = new QPushButton("配置", this);
    connect(_imageSelectPtn, &QPushButton::clicked, this, [this]{
        ImageSelectWidget::instance()->showFromPtn(_imageSelectPtn);
    });
    QLabel* label6 = new QLabel("数据", this);
    QLabel* label7 = new QLabel("数据值", this);
    QLabel* label8 = new QLabel("当前数据值的图片", this);
    QGridLayout* gridLayout2 = new QGridLayout;
    gridLayout2->addWidget(label6,0,0);
    gridLayout2->addWidget(label7,0,1);
    gridLayout2->addWidget(label8,0,2);
    gridLayout2->addWidget(_setDataBox,1,0);
    gridLayout2->addWidget(_valueInputEdit,1,1);
    gridLayout2->addWidget(_imageSelectPtn,1,2);
    _switchImageWidget->setLayout(gridLayout2);

    for(auto it = _allDataItemList.begin(); it != _allDataItemList.end(); ++it){
        _setDataBox->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomDataItem>(*it)));
    }
    _setDataBox->setEditable(true);
    QCompleter* pCompleter5 = new QCompleter(_setDataBox->model());
    pCompleter5->setFilterMode(Qt::MatchContains);
    _setDataBox->setCompleter(pCompleter5);
    _setDataBox->setCurrentIndex(-1);

     _stackedWidget->insertWidget(5, _switchImageWidget);

     //switchValueWidget
     _switchValueWidget = new QWidget(this);
     _switchValueBox = new QComboBox(this);
     _switchValueEdit = new QLineEdit(this);

     QGridLayout *gridLayout3 = new QGridLayout;
     QLabel* label9 = new QLabel(this);
     label9->setText("数据名");
     QLabel* label10 = new QLabel(this);
     label10->setText("切换值，用‘#’分割多个值");
     gridLayout3->addWidget(label9, 0, 0);
     gridLayout3->addWidget(label10,0, 1);
     gridLayout3->addWidget(_switchValueBox ,1, 0);
     gridLayout3->addWidget(_switchValueEdit ,1, 1);
     _switchValueWidget->setLayout(gridLayout3);

     for(auto it = _allDataItemList.begin(); it != _allDataItemList.end(); ++it){
         _switchValueBox->addItem((*it)->getName(), QVariant::fromValue(QPointer<CustomDataItem>(*it)));
     }
     _switchValueBox->setEditable(true);
     QCompleter* pCompleter6 = new QCompleter(_switchValueBox->model());
     pCompleter6->setFilterMode(Qt::MatchContains);
     _switchValueBox->setCompleter(pCompleter1);
     _switchValueBox->setCurrentIndex(-1);

     _stackedWidget->insertWidget(6, _switchValueWidget);

    //从布局中恢复
    this->loadFromFunction();


    //编辑连接，务必放在恢复后面，否则会造成覆盖函数的bug
    //show
    connect(_showComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_showComboBox->itemData(index));
       _customFunc->setFuncArgs(temp);
    });
    //set value
    connect(_setValueBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_setValueBox->itemData(index));
        temp.push_back(QVariant(_valueEdit->text()));
       _customFunc->setFuncArgs(temp);
    });
    connect(_valueEdit, &QLineEdit::textChanged, this, [this](){
        QVector<QVariant> temp;
        temp.push_back(_setValueBox->itemData(_setValueBox->currentIndex()));
        temp.push_back(QVariant(_valueEdit->text()));
       _customFunc->setFuncArgs(temp);
    });
    //setValueFromInput
    connect(_setValueFromInputBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_setValueFromInputBox->itemData(index));
        temp.push_back(_inputBox->itemData(_inputBox->currentIndex()));
       _customFunc->setFuncArgs(temp);
    });
    connect(_inputBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_setValueFromInputBox->itemData(_setValueFromInputBox->currentIndex()));
        temp.push_back(_inputBox->itemData(index));
       _customFunc->setFuncArgs(temp);
    });
    //setCurScene
    connect(_curSceneInput, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_curSceneInput->itemData(index));
       _customFunc->setFuncArgs(temp);
    });
    //switch image
    connect(_setDataBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_setDataBox->itemData(index));
        temp.push_back(_valueInputEdit->text());
        temp.push_back(_imageSelectPtn->objectName());
       _customFunc->setFuncArgs(temp);
    });
    connect(_valueInputEdit, &QLineEdit::textChanged, this, [this](){
        QVector<QVariant> temp;
        temp.push_back(_setDataBox->itemData(_setDataBox->currentIndex()));
        temp.push_back(_valueInputEdit->text());
        temp.push_back(_imageSelectPtn->objectName());
       _customFunc->setFuncArgs(temp);
    });
    connect(_imageSelectPtn, &QObject::objectNameChanged, this, [this](){
        QVector<QVariant> temp;
        temp.push_back(_setDataBox->itemData(_setDataBox->currentIndex()));
        temp.push_back(_valueInputEdit->text());
        temp.push_back(_imageSelectPtn->objectName());
       _customFunc->setFuncArgs(temp);
    });
    //switch value
    connect(_switchValueBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        QVector<QVariant> temp;
        temp.push_back(_switchValueBox->itemData(index));
        temp.push_back(QVariant(_switchValueEdit->text()));
       _customFunc->setFuncArgs(temp);
    });
    connect(_switchValueEdit, &QLineEdit::textChanged, this, [this](){
        QVector<QVariant> temp;
        temp.push_back(_switchValueBox->itemData(_switchValueBox->currentIndex()));
        temp.push_back(QVariant(_switchValueEdit->text()));
       _customFunc->setFuncArgs(temp);
    });
}

void FunctionSelection::loadFromFunction(){
    if(_customFunc->getFuncType() == CustomFunction::TEST){
        _selectBox->setCurrentText("测试");
    }
    else if(_customFunc->getFuncType() == CustomFunction::SHOW){
        _selectBox->setCurrentText("显示");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 1 ||
           args[0].value<QPointer<CustomDataItem>>().isNull()){
            qDebug() << "function args error! args size is:" + QString::number(args.size());
            return;
        }
        _showComboBox->setCurrentText(args[0].value<QPointer<CustomDataItem>>()->getName());
    }
    else if(_customFunc->getFuncType() == CustomFunction::SET_VALUE){
        _selectBox->setCurrentText("设置值");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 2 ||
               args[0].value<QPointer<CustomDataItem>>().isNull()||
                !args[1].canConvert<QString>()){
            qDebug() << "function args error! args size is:" + QString::number(args.size());
            return;
        }
        _setValueBox->setCurrentText(args[0].value<QPointer<CustomDataItem>>()->getName());
        _valueEdit->setText(args[1].toString());
    }
    else if(_customFunc->getFuncType() == CustomFunction::SET_VALUE_FROM_INPUT){
        _selectBox->setCurrentText("从输入设置值");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 2 ||
              args[0].value<QPointer<CustomDataItem>>().isNull() ||
              args[1].value<QPointer<AbsSceneWidgetItem>>().isNull() ){
            qDebug() << "function args error! args size is:" + QString::number(args.size());
            return;
        }
        _setValueFromInputBox->setCurrentText(args[0].value<QPointer<CustomDataItem>>()->getName());
        _inputBox->setCurrentText(args[1].value<QPointer<AbsSceneWidgetItem>>()->objectName());
    }
    else if(_customFunc->getFuncType() == CustomFunction::SET_CUR_SCENE){
        _selectBox->setCurrentText("切换场景");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 1 ||
              args[0].value<QPointer<CustomScene>>().isNull()){
            qDebug() << "function args error! args size is:" + QString::number(args.size());
            return;
        }
        _curSceneInput->setCurrentText(args[0].value<QPointer<CustomScene>>()->getName());
    }
    else if(_customFunc->getFuncType() == CustomFunction::SWITCH_IMAGE){
        _selectBox->setCurrentText("切换图片");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 3 ||
               args[0].value<QPointer<CustomDataItem>>().isNull()||
               !args[1].canConvert<QString>()||
               !args[2].canConvert<QString>()){
            qDebug() << "args num error, num:" + QString::number(args.size());
            return;
        }
        _setDataBox->setCurrentText(args[0].value<QPointer<CustomDataItem>>()->getName());
        _valueInputEdit->setText(args[1].toString());
        _imageSelectPtn->setObjectName(args[2].toString());
    }
    else if(_customFunc->getFuncType() == CustomFunction::SWITCH_VALUE){
        _selectBox->setCurrentText("切换值");
        const QVector<QVariant>& args = _customFunc->getFuncArgs();
        if(args.size() != 2 ||
               args[0].value<QPointer<CustomDataItem>>().isNull()||
                !args[1].canConvert<QString>()){
            qDebug() << "function args error! args size is:" + QString::number(args.size());
            return;
        }
        _switchValueBox->setCurrentText(args[0].value<QPointer<CustomDataItem>>()->getName());
        _switchValueEdit->setText(args[1].toString());
    }
}

void FunctionSelection::selectBoxChangedSlot(int index){
    //保存值
    _customFunc->setFuncType((CustomFunction::FuncType)index);
    //切换页面
    _stackedWidget->setCurrentIndex(index);
}


