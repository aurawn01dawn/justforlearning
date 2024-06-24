#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createInit();
    layoutInit();
    connectInit();
}

MainWindow::~MainWindow()
{
}

// 创建和初始化主窗口中的各种部件和动作
void MainWindow::createInit(){
    // 创建中心部件和堆叠窗口
    _centerWidget = new QWidget(this);
    _stackWidget = new QStackedWidget(_centerWidget);

    // 创建并添加设计、展示和数据窗口到堆叠窗口
    _designerWidget = new DesignerWidget(_centerWidget);
    _stackWidget->insertWidget(0,_designerWidget);
    _showWidget = new ShowWidget(_centerWidget);
    _stackWidget->insertWidget(1,_showWidget);
    _dataWidget = new DataWidget(_centerWidget);
    _stackWidget->insertWidget(2, _dataWidget);
    //_showWidget = new ShowWidget(_centerWidget);
    //_stackWidget->insertWidget(1,_showWidget);

    // 获取设计工具栏实例
    _designerToolBar = DesignerToolBar::getDesignerToolBar();

    // 创建上方工具栏及其动作
    _upToolBar = new QToolBar(_centerWidget);
    _upToolBar->setOrientation(Qt::Vertical);
    _upToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _upActionGroup = new QActionGroup(_upToolBar);
    _upActionGroup->setExclusive(true);

    _creatorAtn = new QAction(QIcon(":/icon/sheji.png"),"设计",_upToolBar);
    _creatorAtn->setCheckable(true);
    _creatorAtn->setChecked(true);
    _upActionGroup->addAction(_creatorAtn);
    _upToolBar->addAction(_creatorAtn);

    _showAtn = new QAction(QIcon(":/icon/zhanshi.png"),"展示",_upToolBar);
    _showAtn->setCheckable(true);
    _showAtn->setEnabled(false);
    _upActionGroup->addAction(_showAtn);
    _upToolBar->addAction(_showAtn);

    _dataAtn = new QAction(QIcon(":/icon/shuju.png"),"数据",_upToolBar);
    _dataAtn->setCheckable(true);
    _dataAtn->setEnabled(true);
    _upActionGroup->addAction(_dataAtn);
    _upToolBar->addAction(_dataAtn);

    _loadAtn = new QAction(QIcon(":/icon/duqu.png"), "读取", _upToolBar);
    _upToolBar->addAction(_loadAtn);

    _saveAtn = new QAction(QIcon(":/icon/baocun.png"), "保存", _upToolBar);
    _upToolBar->addAction(_saveAtn);

    // 创建下方工具栏及其动作
    _downToolBar = new QToolBar(_centerWidget);
    _downToolBar->setOrientation(Qt::Vertical);
    _downToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QWidget * spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _saveLoadManager = new SaveLoadManager(this);
    _downToolBar->addWidget(spacer);

    _downActionGroup = new QActionGroup(_downToolBar);
    _downActionGroup->setExclusive(true);

    _startListenAtn = new QAction(QIcon(":/icon/kaishi.png"), "开始", _downToolBar);
    _startListenAtn->setCheckable(true);
    _downActionGroup->addAction(_startListenAtn);
    _downToolBar->addAction(_startListenAtn);

    _stopListenAtn = new QAction(QIcon(":/icon/tingzhi.png"), "停止", _downToolBar);
    _stopListenAtn->setCheckable(true);
    _stopListenAtn->setChecked(true);
    _downActionGroup->addAction(_stopListenAtn);
    _downToolBar->addAction(_stopListenAtn);
}

// 设置主窗口的布局
void MainWindow::layoutInit(){
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    // 将上方和下方工具栏添加到垂直布局中
    vBoxLayout->addWidget(_upToolBar);
    vBoxLayout->addWidget(_downToolBar);

    // 将垂直布局和堆叠窗口添加到水平布局中
    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addWidget(_stackWidget);

     // 设置中心部件的布局
    _centerWidget->setLayout(hBoxLayout);

    // 将中心部件设置为主窗口的中心部件
    this->setCentralWidget(_centerWidget);
}

// 连接信号和槽
void MainWindow::connectInit(){
    connect(_upToolBar, &QToolBar::actionTriggered, this, &MainWindow::upToolBarActionTriggerSlot);
    connect(_downToolBar, &QToolBar::actionTriggered, this, &MainWindow::downToolBarActionTriggerSlot);

    connect(_saveAtn, &QAction::triggered, this, [this]{
        QString filePath = QFileDialog::getSaveFileName(
                this, "选择保存位置",
                "/",
                "配置文件 (*.json);;");
        if(filePath == "")return;
        _saveLoadManager->save(filePath);
    });

    connect(_loadAtn, &QAction::triggered, this, [this]{
        QString filePath = QFileDialog::getOpenFileName(
            this, "选择配置文件",
            "/",
            "配置文件 (*.json);;");
        if(filePath == "")return;
        _saveLoadManager->load(filePath);
    });
}


void MainWindow::upToolBarActionTriggerSlot(QAction *action){
    if(action == _creatorAtn){
        _stackWidget->setCurrentWidget(_designerWidget);
    }
    else if(action == _showAtn){
        _stackWidget->setCurrentWidget(_showWidget);
        _showWidget->updateShowViewFitIn();
    }
    else if(action == _dataAtn){
        _stackWidget->setCurrentWidget(_dataWidget);
    }
}

// 自定义槽函数

void MainWindow::downToolBarActionTriggerSlot(QAction *action){
    if(action == _startListenAtn){
        // 更新左侧工具栏可选性
        _designerToolBar->setShowToolState();
        _creatorAtn->setEnabled(false);
        _loadAtn->setEnabled(false);
        _saveAtn->setEnabled(false);
        _showAtn->setEnabled(true);
        if(_upActionGroup->checkedAction() == _creatorAtn){
            _showAtn->setChecked(true);
            upToolBarActionTriggerSlot(_showAtn);
        }
    }else{
        // 更新左侧工具栏可选性
        _designerToolBar->setCreatorToolState();
        _creatorAtn->setEnabled(true);
        _loadAtn->setEnabled(true);
        _saveAtn->setEnabled(true);
        _showAtn->setEnabled(false);
        if(_upActionGroup->checkedAction() == _showAtn){
            _creatorAtn->setChecked(true);
            upToolBarActionTriggerSlot(_creatorAtn);
        }
    }
}
