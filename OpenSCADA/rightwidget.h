#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <QWidget>

#include <QTreeWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPointer>


#include "absscenewidgetitem.h"
#include "scenesmanager.h"
#include "funcconfigwidget.h"
#include "imageselectwidget.h"

class RightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightWidget(QWidget *parent = nullptr);


private:
    QTreeWidget* _treeWidget;
    QPointer<AbsSceneWidgetItem> _currentItem = nullptr;
    ScenesManager* _sceneManager;

    //图元属性
    QTreeWidgetItem *_pixTopItem = nullptr;

    QTreeWidgetItem *_posXSubItem = nullptr;
    QTreeWidgetItem *_posYSubItem = nullptr;
    QTreeWidgetItem *_scaleSubItem = nullptr;
    QTreeWidgetItem *_rotateSubItem = nullptr;

    QSpinBox *_posXLineEdit = nullptr;
    QSpinBox *_posYLineEdit = nullptr;
    QDoubleSpinBox *_scaleLineEdit = nullptr;
    QDoubleSpinBox *_rotateLineEdit = nullptr;

    //控件属性
    QTreeWidgetItem *_comTopItem = nullptr;
    QLineEdit *_comObjNameEdit = nullptr;

    QLineEdit *_textEdit;
    QComboBox *_fontBox;
    QSpinBox *_fontWidthBox;
    QPushButton *_iconImagePtn;
    QSpinBox *_iconSizeBox;
    QSpinBox* _widthBox;
    QSpinBox* _heightBox;
    QLineEdit* _styleSheetEdit;

    //交互控制
    QTreeWidgetItem *_InteTopItem;
    QPushButton *_pressConfigPtn;
    QPushButton *_releaseConfigPtn;
    QPushButton *_timerConfigPtn;
    FuncConfigWidget* _funcConfigWidget;
    ImageSelectWidget* _imageSelectWidget;

    void createInit();
    void layoutInit();
    void connectInit();

private slots:
    void updataShowSlot(QPointer<AbsSceneWidgetItem>);
    void updateComSlot();

signals:

};

#endif // RIGHTWIDGET_H
