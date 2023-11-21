//
// Created by Opala Studios on 21/11/23.
//

#ifndef LUACODEEDITOR_FILEMANAGER_H
#define LUACODEEDITOR_FILEMANAGER_H

#include <QObject>
#include "Models.h"
#include <vector>

class FileManager : public QObject {
    Q_OBJECT
public:
    FileManager();

    std::vector<TickData> loadCsv(const QString& file);
};


#endif //LUACODEEDITOR_FILEMANAGER_H
