#include "imageselectwidget.h"
#include "config.h"

#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QPixmapCache>

ImageSelectWidget* ImageSelectWidget::instance(QWidget *parent){
    static ImageSelectWidget* instance = new ImageSelectWidget(parent);
    return instance;
}

ImageSelectWidget::ImageSelectWidget(QWidget *parent) : QWidget(parent)
{
    //初始化存储文件夹
    QString folderName = "./resource";
    QDir currentDir = QDir::current();
    //检查文件夹是否存在
    if (!currentDir.exists(folderName)) {
       // 创建文件夹
       bool success = currentDir.mkdir(folderName);
       if (success) {
           qDebug() << "Folder created successfully:" << folderName;
       } else {
           qDebug() << "Failed to create folder:" << folderName;
       }
    } else {
       qDebug() << "Folder already exists:" << folderName;
    }

    //初始化布局
    _listWidget = new QListWidget(this);

    _addPtn = new QPushButton("添加",this);
    _delPtn = new QPushButton("删除",this);
    _yesPtn = new QPushButton("确认",this);
    _addPtn->setMaximumWidth(200);
    _delPtn->setMaximumWidth(200);
    _yesPtn->setMaximumWidth(200);

    QHBoxLayout* hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(_addPtn);
    hBoxLayout->addWidget(_delPtn);
    hBoxLayout->addWidget(_yesPtn);
    QVBoxLayout* vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(_listWidget);
    vBoxLayout->addLayout(hBoxLayout);
    this->setLayout(vBoxLayout);

    //设置图像缓存大小
    QPixmapCache::setCacheLimit(IMAGE_CACHE);

    //加载图片资源
    loadImageResource();

    //连接
    connect(_addPtn, &QPushButton::clicked, this, &ImageSelectWidget::addPtnClickedPtn);
    connect(_delPtn, &QPushButton::clicked, this, &ImageSelectWidget::delPtnClickedPtn);
    connect(_yesPtn, &QPushButton::clicked, this, &ImageSelectWidget::yesPtnClickedPtn);

    this->setWindowFlags(Qt::Dialog);
    this->setWindowTitle("资源管理");
    this->setWindowModality(Qt::WindowModal);
    this->setMinimumWidth(800);
    this->setMinimumHeight(800);
}

void ImageSelectWidget::loadImageResource(){
    // 设置目录路径
    QString directoryPath = "./resource";

    // 支持的图片文件扩展名列表
    QStringList imageExtensions;
    imageExtensions << "jpg" << "jpeg" << "png" << "webp";

    // 获取目录中所有文件和文件夹的列表
    QDir dir(directoryPath);
    QStringList entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // 遍历文件列表
    foreach (const QString &entry, entries) {
        // 获取文件的完整路径
        QString filePath = dir.absoluteFilePath(entry);

        // 检查文件扩展名是否在支持的列表中
        QFileInfo fileInfo(filePath);
        if (imageExtensions.contains(fileInfo.suffix().toLower())) {
            qDebug() << "Image file found:" << filePath;
            //加载进列表
            QListWidgetItem * newItem = new QListWidgetItem();
            newItem->setText(filePath.split('/').last());
            QPixmap pixmap;
            if (!QPixmapCache::find(filePath, &pixmap)) {
                QFile file(filePath);
                if(!file.open(QIODevice::ReadOnly)){
                    qDebug() << "image open error:" + filePath;
                  return;
                };
                pixmap.loadFromData(file.readAll());
            }
            newItem->setIcon(QIcon(pixmap));
            newItem->setData(Qt::UserRole, QVariant(filePath));
            _listWidget->addItem(newItem);
        }
    }
}

void ImageSelectWidget::addPtnClickedPtn(){
    QString filePath = QFileDialog::getOpenFileName(
        nullptr, "选择背景文件",
        "./image",
        "图像文件 (*.jpg *.jpeg *.png *.webp);; 所有文件 (*.*);; ");
    if(filePath == "")return;
    //拷贝文件
    QString destPath =  "./resource/" + filePath.split('/').last();
    bool success = QFile::copy(filePath, destPath);
    if (success) {
        qDebug() << "File copied successfully from" << filePath << "to" << destPath;
    } else {
        qDebug() << "Failed to copy file from" << filePath << "to" << destPath;
        return;
    }
    //加载进列表
    QListWidgetItem * newItem = new QListWidgetItem();
    newItem->setText(destPath.split('/').last());
    QPixmap pixmap;
    if (!QPixmapCache::find(filePath, &pixmap)) {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "image open error:" + filePath;
          return;
        };
        pixmap.loadFromData(file.readAll());
        QPixmapCache::insert(filePath, pixmap);
    }
    newItem->setIcon(QIcon(pixmap));
    newItem->setData(Qt::UserRole, QVariant(destPath));
    _listWidget->addItem(newItem);
}

void ImageSelectWidget::delPtnClickedPtn(){
    int curRow = _listWidget->currentRow();
    if(curRow == -1)return;
    //删除列表
    QString filePath = _listWidget->currentItem()->data(Qt::UserRole).toString();
    QPixmapCache::remove(filePath);

    bool success = QFile::remove(filePath);
    if (success) {
        qDebug() << "File removed successfully from" << filePath ;
    } else {
        qDebug() << "Failed to remove file from" << filePath;
        return;
    }
    delete _listWidget->takeItem(curRow);
}

void ImageSelectWidget::yesPtnClickedPtn(){
    int curRow = _listWidget->currentRow();
    if(curRow == -1)return;
    //设置图标
    QString filePath = _listWidget->currentItem()->data(Qt::UserRole).toString();

    //若由函数配置进入
    if(_curPtn != nullptr){
        if(_curPtn->objectName() != filePath){
            _curPtn->setObjectName(filePath);
        }
        this->close();
        return;
    }

    //若由右侧属性面板进入
    if(_curSceneItem.isNull())return;
    _curSceneItem->setImgPath(filePath);
    QWidget * com = _curSceneItem->widget();
    switch (_curSceneItem->getType()) {
    case AbsSceneWidgetItem::PUSHBUTTON:{
        QPushButton* ptn = qobject_cast<QPushButton*>(com);
        QPixmap pixmap;
        if (!QPixmapCache::find(filePath, &pixmap)) {
             pixmap.load(filePath);
             QPixmapCache::insert(filePath, pixmap);
        }
        ptn->setIcon(QIcon(pixmap));
        break;
    }
    case AbsSceneWidgetItem::IMAGE:{
        QLabel* img = qobject_cast<QLabel*>(com);
        QPixmap pixmap;
        if (!QPixmapCache::find(filePath, &pixmap)) {
             pixmap.load(filePath);
             QPixmapCache::insert(filePath, pixmap);
        }
        img->setPixmap(pixmap);
        img->setFixedWidth(pixmap.size().width());
        img->setFixedHeight(pixmap.size().height());
        img->setStyleSheet("background-color: transparent;");
        break;
    }
    default:
        break;
    }
    //关闭窗口
    this->close();
}
