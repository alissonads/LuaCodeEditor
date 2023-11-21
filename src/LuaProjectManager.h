//
// Created by Opala Studios on 20/11/23.
//

#ifndef LUACODEEDITOR_LUAPROJECTMANAGER_H
#define LUACODEEDITOR_LUAPROJECTMANAGER_H

#include <QObject>

class LuaProjectManager : public QObject {
Q_OBJECT
public:
    LuaProjectManager();

public slots:
    void newFile();
    void openFile(const QUrl& file);
    void save(const QUrl& file, const QString& code);

    signals:
    void optNewFile(const QString& baseCode);
    void optOpenFile(const QString& baseCode);
    void optSaveFile(bool status);

private:
    QString _baseCode;
};


#endif //LUACODEEDITOR_LUAPROJECTMANAGER_H
