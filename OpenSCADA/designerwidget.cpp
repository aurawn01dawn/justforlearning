#include "designerwidget.h"

#include <QHBoxLayout>

DesignerWidget::DesignerWidget(QWidget* parent):QFrame(parent)
{

    //create
    _leftWidget = new QWidget(this);
    _leftUpWidget = new LeftUpWidget(this);
    _leftDownWidget = new LeftDownWidget(this);
    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    QLabel* label1 = new QLabel(this);
    label1->setText("控件选择栏");
    QLabel* label2 = new QLabel(this);
    label2->setText("场景控件栏");
    vBoxLayout->addWidget(label1);
    vBoxLayout->addWidget(_leftUpWidget);
    vBoxLayout->addWidget(label2);
    vBoxLayout->addWidget(_leftDownWidget);
    _leftWidget->setLayout(vBoxLayout);
    _midWidget = new MidWidget(this);
    _rightWidget = new RightWidget(this);

    _splitter = new QSplitter(this);
    QHBoxLayout* hBoxLayout = new QHBoxLayout();


    //layout
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->setLineWidth(1);

    _splitter->addWidget(_leftWidget);
    _splitter->addWidget(_midWidget);
    _splitter->addWidget(_rightWidget);
    _splitter->setStretchFactor(0,2);
    _splitter->setStretchFactor(1,5);
    _splitter->setStretchFactor(2,3);
    _splitter->setStyleSheet("QSplitter::handle { background-color: black }");
    _splitter->setHandleWidth(1);

    hBoxLayout->addWidget(_splitter);

    this->setLayout(hBoxLayout);

    //connext
}
