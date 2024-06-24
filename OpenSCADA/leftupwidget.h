#ifndef SELECTCOMWIDGET_H
#define SELECTCOMWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class LeftUpWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftUpWidget(QWidget *parent = nullptr);


private:

    QTreeWidget* _treeWidget;

signals:

private slots:
    void dragStartSlot(QTreeWidgetItem*);

};

#endif // SELECTCOMWIDGET_H
