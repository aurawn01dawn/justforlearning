#include "datawidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

DataWidget::DataWidget(QWidget *parent) : QFrame(parent)
{
    _tabWidget = new QTabWidget(this);
    _localTableWidget = new QTableWidget(this);
    _redisTableWidget = new QTableWidget(this);
    _tabWidget->addTab(_localTableWidget, "本地数据");
    _tabWidget->addTab(_redisTableWidget, "redis数据");
    _dataManager = DataManager::getDataManager();

    _addPtn = new QPushButton("添加", this);
    _delPtn = new QPushButton("删除", this);
    _addPtn->setFixedWidth(150);
    _delPtn->setFixedWidth(150);
    connect(_addPtn, &QPushButton::clicked, this, &DataWidget::addPtnClickedSlot);
    connect(_delPtn, &QPushButton::clicked, this, &DataWidget::delPtnClickedSlot);

    _localTableWidget->setColumnCount(6);
    _localTableWidget->setHorizontalHeaderLabels(QStringList() << "名称"  << "时间戳" <<"值" << "绑定键" << "值调整" << "类型");
    _redisTableWidget->setColumnCount(6);
    _redisTableWidget->setHorizontalHeaderLabels(QStringList() << "名称"  << "时间戳" <<"值" << "绑定键" << "值调整" << "类型");

    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(_addPtn);
    hBoxLayout->addWidget(_delPtn);


    vBoxLayout->addWidget(_tabWidget);
    vBoxLayout->addLayout(hBoxLayout);

    this->setLayout(vBoxLayout);

    //layout
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->setLineWidth(1);

    //加载页面数据
    this->loadTabWidget();

    connect(_dataManager, &DataManager::loadNewData, this, &DataWidget::updataDataWidgetSlot);
}

//加载页面数据
void DataWidget::loadTabWidget(){
    //localTableWidget
    const QList<QPointer<CustomDataItem>>& localDataList = _dataManager->getLocalDataList();
    for(auto it =localDataList.begin(); it != localDataList.end(); ++it){
        _localTableWidget->setRowCount(_localTableWidget->rowCount()+1);

        //name
        QLineEdit *nameEdit = new QLineEdit(this);
        nameEdit->setText((*it)->getName());
        _localTableWidget->setCellWidget(_localTableWidget->rowCount()-1, 0, nameEdit);
        connect(nameEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        //timeStamp and newDataItem
        QTableWidgetItem* timeStampAndData = new QTableWidgetItem();
        timeStampAndData->setText((*it)->getTimeStamp());
        timeStampAndData->setData(Qt::UserRole, QVariant::fromValue((*it)));
        _localTableWidget->setItem(_localTableWidget->rowCount()-1, 1, timeStampAndData);
        connect((*it), &CustomDataItem::dataUpdated,  this, [=]{
            timeStampAndData->setText((*it)->getTimeStamp());
        });

        //value
        QLabel* value = new QLabel(this);
        value->setText((*it)->getData());
        _localTableWidget->setCellWidget(_localTableWidget->rowCount()-1, 2, value);
        connect((*it), &CustomDataItem::dataUpdated,  this, [=]{
            value->setText((*it)->getData());
        });

        //key
        QLineEdit *keyEdit = new QLineEdit(this);
        keyEdit->setText((*it)->getKey());
        _localTableWidget->setCellWidget(_localTableWidget->rowCount()-1, 3, keyEdit);
        connect(keyEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        //script
        QLineEdit *scriptEdit = new QLineEdit(this);
        scriptEdit->setText((*it)->getScript());
        _localTableWidget->setCellWidget(_localTableWidget->rowCount()-1, 4, scriptEdit);
        connect(scriptEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        //type
        QComboBox *typeBox = new QComboBox(this);
        typeBox->addItems(QStringList() << "STRING" << "NUMBER" << "BOOL");
        typeBox->setCurrentIndex((*it)->getType());
        _localTableWidget->setCellWidget(_localTableWidget->rowCount()-1, 5, typeBox);
        connect(typeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DataWidget::setCurRowDataSlot);


    }

    //redisTableWidget
    const QList<QPointer<CustomDataItem>>& redisDataList = _dataManager->getRedisDataList();
    for(auto it =redisDataList.begin(); it != redisDataList.end(); ++it){
        _redisTableWidget->setRowCount(_redisTableWidget->rowCount()+1);

        //name
        QLineEdit *nameEdit = new QLineEdit(this);
        nameEdit->setText((*it)->getName());
        _redisTableWidget->setCellWidget(_redisTableWidget->rowCount()-1, 0, nameEdit);
        connect(nameEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        //timeStamp and newDataItem
        QTableWidgetItem* timeStampAndData = new QTableWidgetItem();
        timeStampAndData->setText((*it)->getTimeStamp());
        timeStampAndData->setData(Qt::UserRole, QVariant::fromValue((*it)));
        _redisTableWidget->setItem(_redisTableWidget->rowCount()-1, 1, timeStampAndData);
        connect((*it), &CustomDataItem::dataUpdated,  this, [=]{
            timeStampAndData->setText((*it)->getTimeStamp());
        });

        //value
        QLabel* value = new QLabel(this);
        value->setText((*it)->getData());
        _redisTableWidget->setCellWidget(_redisTableWidget->rowCount()-1, 2, value);
        connect((*it), &CustomDataItem::dataUpdated,  this, [=]{
            value->setText((*it)->getData());
        });

        //key
        QLineEdit *keyEdit = new QLineEdit(this);
        keyEdit->setText((*it)->getKey());
        _redisTableWidget->setCellWidget(_redisTableWidget->rowCount()-1, 3, keyEdit);
        connect(keyEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        QLineEdit *scriptEdit = new QLineEdit(this);
        scriptEdit->setText((*it)->getScript());
        _redisTableWidget->setCellWidget(_redisTableWidget->rowCount()-1, 4, scriptEdit);
        connect(scriptEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

        QComboBox *typeBox = new QComboBox(this);
        typeBox->addItems(QStringList() << "STRING" << "NUMBER" << "BOOL");
        typeBox->setCurrentIndex((*it)->getType());
        _redisTableWidget->setCellWidget(_redisTableWidget->rowCount()-1, 5, typeBox);
        connect(typeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DataWidget::setCurRowDataSlot);

    }
}

void DataWidget::addPtnClickedSlot(){
    QTableWidget *curTableWidget = dynamic_cast<QTableWidget *>(_tabWidget->currentWidget());
    if(!curTableWidget)return;
    //创建新的dataItem
    QPointer<CustomDataItem> newDataItem = nullptr;
    if(curTableWidget == _localTableWidget){
        newDataItem = _dataManager->createData(CustomDataItem::LOCAL);
    }else{
        newDataItem = _dataManager->createData(CustomDataItem::REDIS);
    }

    curTableWidget->setRowCount(curTableWidget->rowCount()+1);

    //name
    QLineEdit *nameEdit = new QLineEdit(this);
    nameEdit->setText(newDataItem->getName());
    curTableWidget->setCellWidget(curTableWidget->rowCount()-1, 0, nameEdit);
    connect(nameEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

    //timeStamp and newDataItem
    QTableWidgetItem* timeStampAndData = new QTableWidgetItem();
    timeStampAndData->setText(newDataItem->getTimeStamp());
    timeStampAndData->setData(Qt::UserRole, QVariant::fromValue(newDataItem));
    curTableWidget->setItem(curTableWidget->rowCount()-1, 1, timeStampAndData);
    connect(newDataItem, &CustomDataItem::dataUpdated,  this, [=]{
        timeStampAndData->setText(newDataItem->getTimeStamp());
    });

    //value
    QLabel* value = new QLabel(this);
    value->setText(newDataItem->getData());
    curTableWidget->setCellWidget(curTableWidget->rowCount()-1, 2, value);
    connect(newDataItem, &CustomDataItem::dataUpdated, this, [=]{
        value->setText(newDataItem->getData());
    });

    //key
    QLineEdit *keyEdit = new QLineEdit(this);
    keyEdit->setText(newDataItem->getKey());
    curTableWidget->setCellWidget(curTableWidget->rowCount()-1, 3, keyEdit);
    connect(keyEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

    //script
    QLineEdit *scriptEdit = new QLineEdit(this);
    scriptEdit->setText(newDataItem->getScript());
    curTableWidget->setCellWidget(curTableWidget->rowCount()-1, 4, scriptEdit);
    connect(scriptEdit, &QLineEdit::editingFinished, this, &DataWidget::setCurRowDataSlot);

    QComboBox *typeBox = new QComboBox(this);
    typeBox->addItems(QStringList() << "STRING" << "NUMBER" << "BOOL");
    typeBox->setCurrentIndex(newDataItem->getType());
    curTableWidget->setCellWidget(curTableWidget->rowCount()-1, 5, typeBox);
    connect(typeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DataWidget::setCurRowDataSlot);
}

void DataWidget::delPtnClickedSlot(){
    QTableWidget *curTableWidget = dynamic_cast<QTableWidget *>(_tabWidget->currentWidget());
    if(!curTableWidget)return;
    //获取当前行
    int curRow = curTableWidget->currentRow();
    if(curRow == -1)return;
    //删除该行widget
    for(int i =0 ;i < curTableWidget->columnCount(); ++i){
        QWidget* widget = curTableWidget->cellWidget(curRow, i);
        if(widget)widget->deleteLater();
    }
    QTableWidgetItem* timeStampAndData = curTableWidget->takeItem(curRow, 1);
    QPointer<CustomDataItem> dataItem = timeStampAndData->data(Qt::UserRole).value<QPointer<CustomDataItem>>();
    delete timeStampAndData;
    //删除行数据
    _dataManager->deleteData(dataItem);
    curTableWidget->removeRow(curRow);
}

void DataWidget::setCurRowDataSlot(){
    QTableWidget *curTableWidget = dynamic_cast<QTableWidget *>(_tabWidget->currentWidget());
    if(!curTableWidget || curTableWidget->rowCount() == 0)return;
    //获取当前行
    int curRow = curTableWidget->currentRow();
    if(curRow == -1)return;
    //获取对象
    QLineEdit* nameEdit = dynamic_cast<QLineEdit *>(curTableWidget->cellWidget(curRow, 0));
    QTableWidgetItem* timeStampAndData = curTableWidget->item(curRow, 1);
    //QLabel* value = dynamic_cast<QLabel *>(curTableWidget->cellWidget(curRow, 3));
    QLineEdit* keyEdit = dynamic_cast<QLineEdit *>(curTableWidget->cellWidget(curRow, 3));
    QLineEdit* scriptEdit = dynamic_cast<QLineEdit *>(curTableWidget->cellWidget(curRow, 4));
    QComboBox* typeBox = dynamic_cast<QComboBox *>(curTableWidget->cellWidget(curRow, 5));
    QPointer<CustomDataItem> dataItem = timeStampAndData->data(Qt::UserRole).value<QPointer<CustomDataItem>>();
    //保存当前行数据
    dataItem->setName(nameEdit->text());
    dataItem->setKey(keyEdit->text());
    dataItem->setScript(scriptEdit->text());
    dataItem->setType((CustomDataItem::DataType) typeBox->currentIndex());
}

void DataWidget::updataDataWidgetSlot(){
    //清空已有布局
    for(int i = 0; i < _localTableWidget->rowCount(); ++i){
        //删除该行widget
        for(int j =0 ;j < _localTableWidget->columnCount(); ++j){
            QWidget* widget = _localTableWidget->cellWidget(i, j);
            if(widget)widget->deleteLater();
        }
        QTableWidgetItem* timeStampAndData = _localTableWidget->takeItem(i, 1);
        delete timeStampAndData;
    }
    _localTableWidget->clear();
    _localTableWidget->setRowCount(0);

    for(int i = 0; i < _redisTableWidget->rowCount(); ++i){
        //删除该行widget
        for(int j =0 ;j < _redisTableWidget->columnCount(); ++j){
            QWidget* widget = _redisTableWidget->cellWidget(i, j);
            if(widget)widget->deleteLater();
        }
        QTableWidgetItem* timeStampAndData = _redisTableWidget->takeItem(i, 1);
        delete timeStampAndData;
    }
    _redisTableWidget->clear();
    _redisTableWidget->setRowCount(0);


    //加载布局
    this->loadTabWidget();
}
