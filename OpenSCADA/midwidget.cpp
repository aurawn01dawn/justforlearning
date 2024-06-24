#include "midwidget.h"

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>

MidWidget::MidWidget(QWidget *parent) : QWidget(parent)
{
    _designerView = new DesignerView(this);
    _designerToolBar = DesignerToolBar::getDesignerToolBar(this);
    _designerToolBar->setDesignerView(_designerView);
    _selectBar = new SceneSelectToolBar(_designerView, this);

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout;
    QLabel* label1 = new QLabel(this);
    label1->setText("工具选择栏");
    vBoxLayout1->addWidget(label1);
    vBoxLayout1->addWidget(_designerToolBar);
    vBoxLayout1->setMargin(0);

    QVBoxLayout *vBoxLayout2 = new QVBoxLayout;
    QLabel* label2 = new QLabel(this);
    label2->setText("场景选择栏");
    vBoxLayout2->addWidget(label2);
    vBoxLayout2->addWidget(_selectBar);
    vBoxLayout2->setMargin(0);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addLayout(vBoxLayout1);
    hBoxLayout->addLayout(vBoxLayout2);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addLayout(hBoxLayout);
    vBoxLayout->addWidget(_designerView);
    this->setLayout(vBoxLayout);

}
