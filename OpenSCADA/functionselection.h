#ifndef FUNCTIONSELECTION_H
#define FUNCTIONSELECTION_H

#include <QWidget>
#include <QStackedWidget>
#include <QComboBox>
#include <QLineEdit>

#include "customfunction.h"
#include "datamanager.h"
#include "scenesmanager.h"

class FunctionSelection : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionSelection(CustomFunction* function, QWidget *parent = nullptr);

private:
    //函数对象
    CustomFunction* _customFunc;
    //选择栏
    QComboBox* _selectBox;
    QStackedWidget* _stackedWidget;

    //不同页面
    QWidget *_testWidget;

    //show
    QWidget *_showWidget;
    QComboBox *_showComboBox;
    DataManager* _dataManager;
    ScenesManager* _sceneManager;
    const QList<QPointer<CustomDataItem>>& _allDataItemList;

    //setValue
    QWidget *_setValueWidget;
    QComboBox *_setValueBox;
    QLineEdit *_valueEdit;

    //setValueFromInput
    QWidget* _setValueFromIputWidget;
    QComboBox *_setValueFromInputBox;
    QComboBox *_inputBox;

    //setCurScene
    QWidget* _setCurSceneWidget;
    QComboBox *_curSceneInput;

    //switch image
    QWidget* _switchImageWidget;
    QComboBox *_setDataBox;
    QLineEdit *_valueInputEdit;
    QPushButton *_imageSelectPtn;

    //switchValue
    QWidget *_switchValueWidget;
    QComboBox *_switchValueBox;
    QLineEdit *_switchValueEdit;

    void loadFromFunction();

private slots:
    void selectBoxChangedSlot(int);


signals:

};

#endif // FUNCTIONSELECTION_H
