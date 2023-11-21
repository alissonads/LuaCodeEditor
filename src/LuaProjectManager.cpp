//
// Created by Opala Studios on 20/11/23.
//

#include "LuaProjectManager.h"
#include <QFileDialog>

LuaProjectManager::LuaProjectManager() :
QObject(),
_baseCode("function calculate(arr)\n    --add your code here\nend")
{
}

void LuaProjectManager::newFile() {
    emit optNewFile(_baseCode);
}

void LuaProjectManager::openFile(const QUrl& file) {
    if (!file.isEmpty()) {
        auto fileName = file.toString();
#ifdef __APPLE__
        fileName.replace("file:///", "/");
#else
        fileName.replace("file:///", "");
#endif
        if (!QFileInfo::exists(fileName))
            return;

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
            return;

        QByteArray res = file.readAll();

        file.close();

        emit optOpenFile(res);
    }
}

void LuaProjectManager::save(const QUrl& file, const QString& code) {
    if (!file.isEmpty()) {
        auto fileName = file.toString();
#ifdef __APPLE__
        fileName.replace("file:///", "/");
#else
        fileName.replace("file:///", "");
#endif
//        if (!QFileInfo::exists(fileName))
//            return;

        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        file.write(code.toUtf8());

        file.close();

//        emit optOpenFile(res);
    }
}
