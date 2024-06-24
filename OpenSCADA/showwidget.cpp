#include "showwidget.h"

#include <QVBoxLayout>

ShowWidget::ShowWidget(QWidget *parent) : QWidget(parent)
{
    _showView = new ShowWidgetView(this);
    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(_showView);
    this->setLayout(vBoxLayout);
}
