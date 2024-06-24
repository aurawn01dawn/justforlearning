#ifndef DESIGNERTOOLBAR_H
#define DESIGNERTOOLBAR_H

#include <QToolBar>
#include <QToolButton>
#include <QPointer>

class DesignerView;

class DesignerToolBar : public QToolBar
{
    Q_OBJECT
public:
    static DesignerToolBar* getDesignerToolBar(QWidget *parent = nullptr);

    enum ToolState{
        MOUSE_TOOL,
        SCALE_TOOL,
        DELETE_TOOL,
        VIEW_TOOL,
        ROTATE_TOOL,
        UP_TOOL,
        DOWN_TOOL,

        SHOW_TOOL,
    };

    inline enum ToolState getCurToolState(){
        return _currentState;
    };

    inline void setShowToolState(){
        _currentState = SHOW_TOOL;
    };

    inline void setCreatorToolState(){
        _currentState = MOUSE_TOOL;
        _mouseButton->setChecked(true);
    }

    inline void setDesignerView(QPointer<DesignerView> designerView){
        _designerView = designerView;
    }


private:

    DesignerToolBar(QWidget *parent = nullptr);

    void createInit();
    void layoutInit();
    void connectInit();

    QPointer<DesignerView> _designerView;

    QAction *_mouseButton;
    QAction *_scaleButton;
    QAction *_deleteButton;
    QAction *_dragButton;
    QAction *_rotateButton;
    QAction *_upButton;
    QAction *_downButton;
    QActionGroup* _buttonGroup;

    ToolState _currentState = MOUSE_TOOL;

private slots:
    void actionTriggeredSlot(QAction*);
};

#endif // DESIGNERTOOLBAR_H
