#ifndef SCENESELECTTOOLBAR_H
#define SCENESELECTTOOLBAR_H

#include <QToolBar>
#include <QPushButton>

#include "scenesmanager.h"

class DesignerView;
class SceneSelectToolBar : public QToolBar
{
    Q_OBJECT
public:
    SceneSelectToolBar(DesignerView* ,QWidget *parent = nullptr);

private:
    void createInit();
    void layoutInit();
    void connectInit();

    //场景管理
    ScenesManager* _scenesManager;
    DesignerView * _designerView;
    //默认场景按钮
    QActionGroup* _scenePtnGroup;
    //添加按钮
    QPushButton* _addPtn;
    QAction* _addAtn;
    //删除按钮
    QPushButton* _delPtn;
    QAction* _delAtn;
    //更新icon的定时器
    QTimer* _timer;

    /**
     * @brief getViewSceneIcon 根据view对象当前的scene对象导出场景图标
     */
    void updateViewSceneIcon();

private slots:
    /**
     * @brief actionTriggeredSlot scene场景按钮切换的槽函数
     */
    void actionTriggeredSlot(QAction*);
    /**
     * @brief addPtnClickedSlot 添加场景按钮点击的槽函数
     */
    void addPtnClickedSlot();
    void delPtnClickedSlot();

    /**
     * @brief updateScenePtn 从sceneManager中重新获取场景信息，设置按钮个数
     */
    void updateScenePtn();
};

#endif // SCENESELECTTOOLBAR_H
