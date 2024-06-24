#ifndef MIDWIDGET_H
#define MIDWIDGET_H

#include <QWidget>
#include "designerview.h"
#include "designertoolbar.h"
#include "sceneselecttoolbar.h"

class MidWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MidWidget(QWidget *parent = nullptr);

private:
    DesignerView* _designerView;
    DesignerToolBar* _designerToolBar;
    SceneSelectToolBar* _selectBar;

signals:

};

#endif // MIDWIDGET_H
