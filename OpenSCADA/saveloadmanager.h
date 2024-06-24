#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

#include <QObject>

#include "datamanager.h"
#include "scenesmanager.h"

class SaveLoadManager : public QObject
{
    Q_OBJECT
public:
    explicit SaveLoadManager(QObject *parent = nullptr);

    void load(const QString& filePath);
    void save(const QString& filePath);

private:
    DataManager* _dataManager;
    ScenesManager* _scenesManager;
signals:

};

#endif // SAVELOADMANAGER_H
