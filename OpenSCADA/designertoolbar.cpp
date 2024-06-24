#include "designertoolbar.h"
#include "designerview.h"

DesignerToolBar* DesignerToolBar::getDesignerToolBar(QWidget *parent){
    static DesignerToolBar* customToolBar = new DesignerToolBar(parent);
    return customToolBar;
}

DesignerToolBar::DesignerToolBar(QWidget* parent):QToolBar(parent)
{
    createInit();
    layoutInit();
    connectInit();
}

void DesignerToolBar::createInit(){
    _buttonGroup = new QActionGroup(this);
    _buttonGroup->setExclusive(true);

    _mouseButton = new QAction(this);
    _mouseButton->setText("鼠标");
    _mouseButton->setIcon(QIcon(":/icon/shubiao.png"));
    _mouseButton->setCheckable(true);
    _mouseButton->setChecked(true);
    _buttonGroup->addAction(_mouseButton);

    _scaleButton = new QAction(this);
    _scaleButton->setText("缩放");
    _scaleButton->setIcon(QIcon(":/icon/suofang.png"));
    _scaleButton->setCheckable(true);
    _buttonGroup->addAction(_scaleButton);

    _deleteButton = new QAction(this);
    _deleteButton->setText("删除");
    _deleteButton->setIcon(QIcon(":/icon/shanchu.png"));
    _deleteButton->setCheckable(true);
    _buttonGroup->addAction(_deleteButton);

    _dragButton = new QAction(this);
    _dragButton->setText("视角");
    _dragButton->setIcon(QIcon(":/icon/shijiao.png"));
    _dragButton->setCheckable(true);
    _buttonGroup->addAction(_dragButton);

    _rotateButton = new QAction(this);
    _rotateButton->setText("旋转");
    _rotateButton->setIcon(QIcon(":/icon/xuanzhuan.png"));
    _rotateButton->setCheckable(true);
    _buttonGroup->addAction(_rotateButton);

    _upButton = new QAction(this);
    _upButton->setText("上移");
    _upButton->setIcon(QIcon(":/icon/shangyi.png"));
    _upButton->setCheckable(true);
    _buttonGroup->addAction(_upButton);

    _downButton = new QAction(this);
    _downButton->setText("下移");
    _downButton->setIcon(QIcon(":/icon/xiayi.png"));
    _downButton->setCheckable(true);
    _buttonGroup->addAction(_downButton);

    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void DesignerToolBar::layoutInit(){
    this->addAction(_mouseButton);
    this->addSeparator();
    this->addAction(_scaleButton);
    this->addSeparator();
    this->addAction(_deleteButton);
    this->addSeparator();
    this->addAction(_dragButton);
    this->addSeparator();
    this->addAction(_rotateButton);
    this->addSeparator();
    this->addAction(_upButton);
    this->addSeparator();
    this->addAction(_downButton);
    this->addSeparator();
}

void DesignerToolBar::connectInit(){
    connect(this, &DesignerToolBar::actionTriggered, this, &DesignerToolBar::actionTriggeredSlot);
}

void DesignerToolBar::actionTriggeredSlot(QAction* action){
    if(action == _mouseButton){
        _currentState = MOUSE_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
    else if(action == _scaleButton){
        _currentState = SCALE_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
    else if(action == _deleteButton){
        _currentState = DELETE_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
    else if(action == _dragButton){
        _currentState = VIEW_TOOL;
        _designerView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    }
    else if(action == _rotateButton){
        _currentState = ROTATE_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
    else if(action == _upButton){
        _currentState = UP_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
    else if(action == _downButton){
        _currentState = DOWN_TOOL;
        _designerView->setDragMode(QGraphicsView::NoDrag);
    }
}

