//
// Created by Opala Studios on 14/11/23.
//

#ifndef LUACODEEDITOR_LUACOMPILATOR_H
#define LUACODEEDITOR_LUACOMPILATOR_H

#include <QObject>
#include <QObject>
#include <QVariant>
#include <string_view>
#include <memory>

class lua_State;
class TickData;
class TimeValue;

class LuaCompilator : public QObject, public std::enable_shared_from_this<LuaCompilator> {
    Q_OBJECT
public:
    struct BuildResult {
        bool success;
        QString message;
    };

private:
    explicit LuaCompilator();

public:
    ~LuaCompilator();

    static std::shared_ptr<LuaCompilator> getInstance();

    static int customPrint(lua_State* L);
    static int customInput(lua_State* L);

    void setTickDataList(const std::vector<TickData>& list);
    void setExtraData(const QList<QVariant>& extraData);

public slots:
    bool build(const QString& code);
    void run(const QString& code);

    signals:
    void buildFinished(const QString& code, int line = 0);
    void runFinished(const QString& result, int line = 0);
    void printResult(const QString& result);

private:
    BuildResult loadString(std::string_view code);

    int getErrorLineNumber(const QString& message) const;

    void pushTickDataListToLua(lua_State* L);
    void pushTickDataToLua(lua_State* L, const TickData& obj);
    void pushExtraDataToLua(lua_State* L);
    void toLuaData(const QVariant& data);
    std::vector<TimeValue> dataOutput();

    lua_State* L;
    std::vector<TickData> _tickDataList;
    QList<QVariant> _extraData;
    static std::shared_ptr<LuaCompilator> _instance;
    static QString _bytes;
};


#endif //LUACODEEDITOR_LUACOMPILATOR_H
