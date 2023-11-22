//
// Created by Opala Studios on 14/11/23.
//

#include "LuaCompilator.h"
#include <QDebug>
#include <lua5.4/lua.hpp>
#include <functional>
#include <iostream>
#include "Models.h"

#define MAX_BYTES 2048

std::string customRead() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::shared_ptr<LuaCompilator> LuaCompilator::_instance = nullptr;
QString LuaCompilator::_bytes = "";

LuaCompilator::LuaCompilator() :
QObject(),
_tickDataList(),
L(nullptr)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "print", LuaCompilator::customPrint);
//    lua_register(L, "io.read", LuaCompilator::customInput);
}

LuaCompilator::~LuaCompilator() {
    lua_close(L);
}

std::shared_ptr<LuaCompilator> LuaCompilator::getInstance() {
    if (!_instance)
        _instance.reset(new LuaCompilator());

    return _instance;
}

int LuaCompilator::customPrint(lua_State* L) {
    if (!_instance)
        return 0;

    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; ++i) {
        if (_bytes.size() >= MAX_BYTES)
            return 0;

        const char* str = lua_tostring(L, i);
        if (str) {
            _bytes += str;
            if (_bytes.size() >= MAX_BYTES)
                emit _instance->printResult("...\n");
            else
                emit _instance->printResult(str);
        }
    }

    return 0;
}

int LuaCompilator::customInput(lua_State* L) {
    std::string input = customRead();
    lua_pushstring(L, input.c_str());
    return 1;
}

void LuaCompilator::setTickDataList(const std::vector<TickData>& list) {
    _tickDataList = list;
}

void LuaCompilator::setExtraData(const QList<QVariant>& extraData) {
    _extraData = extraData;
}

bool LuaCompilator::build(const QString& code) {
    auto result = loadString(code.toStdString());
    int line = 0;
    QString message;
    if (!result.success) {
        line = getErrorLineNumber(result.message);
        message = "Build failed: " + result.message + "\n";
    }
    else {
        message = "Build finished\n\n";
    }
    _bytes.clear();
    emit buildFinished(message, line);

    return result.success;
}

void LuaCompilator::run(const QString& code) {
    if (build(code) && !code.isEmpty()) {
        const char* mainFunction = "calculate";
        lua_getglobal(L, mainFunction);

        pushTickDataListToLua(L);
        pushExtraDataToLua(L);

        int nValues = _extraData.size() + 1;
        if (lua_pcall(L, nValues, 1, 0) != LUA_OK) {
            QString message = lua_tostring(L, -1);
            auto lineError = getErrorLineNumber(message);
            emit runFinished(message + "\n", lineError);
            lua_pop(L, 1);
            return;
        }

        std::vector<TimeValue> values = dataOutput();

//        QString result;
//        for (int i = 0; i < 1000; i++) {
//            auto tv = values[i];
//            result += QString("TimeValue { time: %0, value: %1}\n").arg(tv.time).arg(tv.value);
//        }
//        auto result = lua_tostring(L, -1);
        emit runFinished("run successful\n");

        lua_pop(L, 1);
    }
}

LuaCompilator::BuildResult LuaCompilator::loadString(std::string_view code) {
    assert(L != nullptr);

    BuildResult result {true,"Build successful!"};
    int status = luaL_dostring(L, code.data());
    if (status != LUA_OK) {
        result.success = false;
        result.message = lua_tostring(L, -1);
    }

    return result;
}

int LuaCompilator::getErrorLineNumber(const QString& message) const {
    int number = 0;
    auto index = message.indexOf(':');
    auto res = message.mid(index + 1);
    index = res.indexOf(':');
    res = res.mid(0, index);

    bool ok = true;
    number = res.toInt(&ok);

    if (ok)
        return number;

    return 0;
}

void LuaCompilator::pushTickDataListToLua(lua_State* L) {
    lua_newtable(L);
    for (size_t i = 0; i < _tickDataList.size(); ++i) {
        pushTickDataToLua(L, _tickDataList[i]);
        lua_rawseti(L, -2, static_cast<int>(i) + 1);
    }
}

void LuaCompilator::pushTickDataToLua(lua_State* L, const TickData& obj) {
    lua_newtable(L);

    lua_pushstring(L, "time");
    lua_pushinteger(L, obj.time);
    lua_settable(L, -3);

    lua_pushstring(L, "value");
    lua_pushnumber(L, obj.value);
    lua_settable(L, -3);

    lua_pushstring(L, "volume");
    lua_pushnumber(L, obj.volume);
    lua_settable(L, -3);
}

void LuaCompilator::pushExtraDataToLua(lua_State* L) {
    for (auto& item : _extraData) {
        if (item.metaType() == QMetaType(QMetaType::Type::QVariantList)) {
            lua_newtable(L);
            auto temp = item.toList();
            for (size_t i = 0; i < temp.size(); ++i) {
                toLuaData(temp[i]);
                lua_rawseti(L, -2, static_cast<int>(i) + 1);
            }
        }
        else  {
            toLuaData(item);
        }
    }
}

void LuaCompilator::toLuaData(const QVariant& item) {
    if (item.metaType() == QMetaType(QMetaType::Type::Bool)) {
        lua_pushboolean(L, item.toBool());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::Int)) {
        lua_pushinteger(L, item.toInt());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::Long)) {
        lua_pushnumber(L, item.toLongLong());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::Double)) {
        lua_pushnumber(L, item.toDouble());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::Float)) {
        lua_pushnumber(L, item.toFloat());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::QString)) {
        lua_pushstring(L, item.toString().toUtf8().constData());
    }
    else if (item.metaType() == QMetaType(QMetaType::Type::QVariantMap)) {
        lua_newtable(L);
        auto temp = item.toMap();
        for (auto& kv : temp.toStdMap()) {
            lua_pushstring(L, kv.first.toUtf8().data());
            toLuaData(kv.second);
            lua_settable(L, -3);
        }
    }
}

std::vector<TimeValue> LuaCompilator::dataOutput() {
    std::vector<TimeValue> values;
    if (lua_istable(L, -1)) {
        int len = luaL_len(L, -1);
        for (int i = 1; i <= len; i++) {
            lua_pushnumber(L, i);
            lua_gettable(L, -2);

            if (lua_istable(L, -1)) {
                TimeValue tv;

                lua_pushstring(L, "time");
                lua_gettable(L, -2);
                tv.time = static_cast<longl>(lua_tonumber(L, -1));
                lua_pop(L, 1);

                lua_pushstring(L, "value");
                lua_gettable(L, -2);
                tv.value = static_cast<double>(lua_tonumber(L, -1));
                lua_pop(L, 1);

                values.push_back(tv);
            }
            lua_pop(L, 1);
        }
//            lua_pop(L, 1);
    }

    return values;
}
