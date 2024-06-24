#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>

#include "showwidgetview.h"

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = nullptr);

    inline void updateShowViewFitIn(){
        _showView->fitInView(ScenesManager::getScenesManager()->getCurShowScene()->getBackItem(), Qt::KeepAspectRatio);
    }

private:
    ShowWidgetView* _showView;

signals:

};

#endif // SHOWWIDGET_H
