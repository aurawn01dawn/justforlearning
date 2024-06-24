#ifndef DESIGNERWIDGET_H
#define DESIGNERWIDGET_H

#include <QFrame>
#include <QSplitter>

#include "leftupwidget.h"
#include "leftdownwidget.h"
#include "midwidget.h"
#include "rightwidget.h"

class DesignerWidget : public QFrame
{
    Q_OBJECT
public:
    DesignerWidget(QWidget*);

private:
    QWidget *_leftWidget;
    LeftUpWidget* _leftUpWidget;
    LeftDownWidget* _leftDownWidget;
    MidWidget* _midWidget;
    RightWidget* _rightWidget;
    QSplitter *_splitter;
};

#endif // DESIGNERWIDGET_H
