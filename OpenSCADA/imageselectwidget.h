#ifndef IMAGESELECTWIDGET_H
#define IMAGESELECTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>

#include "absscenewidgetitem.h"

class ImageSelectWidget : public QWidget
{
    Q_OBJECT
public:
    static ImageSelectWidget* instance(QWidget *parent = nullptr);

    inline void showFromSceneItem(QPointer<AbsSceneWidgetItem> sceneItem){
        _curSceneItem = sceneItem;
        _curPtn = nullptr;
        this->show();
    }

    inline void showFromPtn(QPointer<QPushButton> ptn){
        _curSceneItem = nullptr;
        _curPtn = ptn;
        this->show();
    }

private:
    explicit ImageSelectWidget(QWidget *parent = nullptr);

    QListWidget* _listWidget;
    QPointer<AbsSceneWidgetItem> _curSceneItem = nullptr;
    QPointer<QPushButton> _curPtn = nullptr;

    QPushButton* _addPtn;
    QPushButton* _delPtn;
    QPushButton* _yesPtn;

    void loadFromItem();
    void loadImageResource();

private slots:
    void addPtnClickedPtn();
    void delPtnClickedPtn();
    void yesPtnClickedPtn();
signals:

};

#endif // IMAGESELECTWIDGET_H
