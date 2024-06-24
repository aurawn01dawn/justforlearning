#include "leftupwidget.h"
#include "absscenewidgetitem.h"

#include <QMimeData>
#include <QDrag>
#include <QVBoxLayout>

LeftUpWidget::LeftUpWidget(QWidget *parent) : QWidget(parent)
{


    //create
    _treeWidget = new QTreeWidget(this);
    _treeWidget->setColumnCount(1);
    _treeWidget->setHeaderLabels(QStringList() << "控件类别");

    QTreeWidgetItem* top1Item = new QTreeWidgetItem(_treeWidget, QStringList() << "按钮控件");
    QTreeWidgetItem* ptnItem = new QTreeWidgetItem(top1Item, QStringList() << "按钮");
    ptnItem->setIcon(0, QIcon(":/icon/anniu.png"));
    ptnItem->setData(0, Qt::UserRole, QVariant(AbsSceneWidgetItem::ComType::PUSHBUTTON));

    QTreeWidgetItem* top2Item = new QTreeWidgetItem(_treeWidget, QStringList() << "输入控件");
    QTreeWidgetItem* inputItem = new QTreeWidgetItem(top2Item, QStringList() << "输入");
    inputItem->setIcon(0, QIcon(":/icon/shuru.png"));
    inputItem->setData(0, Qt::UserRole, QVariant(AbsSceneWidgetItem::ComType::INPUT));

    QTreeWidgetItem* top3Item = new QTreeWidgetItem(_treeWidget, QStringList() << "显示控件");
    QTreeWidgetItem* outputItem = new QTreeWidgetItem(top3Item, QStringList() << "LCD数字");
    outputItem->setIcon(0, QIcon(":/icon/LCDshuzi.png"));
    outputItem->setData(0, Qt::UserRole, QVariant(AbsSceneWidgetItem::ComType::OUTPUT));
    QTreeWidgetItem* textItem = new QTreeWidgetItem(top3Item, QStringList() << "文本数字");
    textItem->setIcon(0, QIcon(":/icon/wenbenshuzi.png"));
    textItem->setData(0, Qt::UserRole, QVariant(AbsSceneWidgetItem::ComType::TEXT));

    QTreeWidgetItem* top4Item = new QTreeWidgetItem(_treeWidget, QStringList() << "其他控件");
    QTreeWidgetItem* imageItem = new QTreeWidgetItem(top4Item, QStringList() << "图片");
    imageItem->setIcon(0, QIcon(":/icon/tupian.png"));
    imageItem->setData(0, Qt::UserRole, QVariant(AbsSceneWidgetItem::ComType::IMAGE));

    //layout
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(_treeWidget);
    this->setLayout(vBoxLayout);

    _treeWidget->setItemsExpandable(true);		//子Item是否可以被展开
    _treeWidget->expandAll();

    //connect
    connect(_treeWidget, &QTreeWidget::itemPressed, this, &LeftUpWidget::dragStartSlot);
}

void LeftUpWidget::dragStartSlot(QTreeWidgetItem* item){
    // 创建拖动传递数据
    QMimeData *mimeData = new QMimeData;
    mimeData->setImageData(item->data(0, Qt::UserRole));
    // 创建拖动对象
    QDrag *drag = new QDrag(this);
    connect(drag, &QDrag::objectNameChanged, this, [](const QString &objectName){
        if(objectName == "DropOnScene")
        qDebug()<<"!";});
    drag->setMimeData(mimeData);
    /* 设置拖动过程中鼠标跟随图片 */
    //drag->setPixmap(QPixmap(allComDataVec[comId]["激活图标"]).scaled(QSize(100,100),Qt::KeepAspectRatio));
    drag->exec();
}
