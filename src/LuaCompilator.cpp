//
// Created by Opala Studios on 14/11/23.
//

#include "LuaCompilator.h"
#include <QDebug>
#include <lua5.4/lua.hpp>
#include <functional>
#include <iostream>
#include "Models.h"

std::string customRead() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::shared_ptr<LuaCompilator> LuaCompilator::_instance = nullptr;

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
        const char* str = lua_tostring(L, i);
        if (str) {
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

    emit buildFinished(message, line);

    return result.success;
}

void LuaCompilator::run(const QString& code) {
    if (build(code) && !code.isEmpty()) {
        const char* mainFunction = "calculate";
        lua_getglobal(L, mainFunction);

        // Pega quantos parâmetros tem a função calculte
//        lua_Debug ar;
//        lua_getglobal(L, mainFunction);
//        lua_getinfo(L, ">u", &ar);
//        int numArgs = ar.nparams;

//        lua_pushnumber(L, 10);
//        lua_pushnumber(L, 5);
//        lua_pushstring(L, "div");

//        std::vector<double> arr = {1, 35.4, 10, 8, 4};
//        lua_newtable(L);
//        for (size_t i = 0; i < arr.size(); ++i) {
//            lua_pushnumber(L, arr[i]);
//            lua_rawseti(L, -2, static_cast<int>(i) + 1); // Ajusta os índices para começar de 1
//        }

        pushTickDataListToLua(L);

        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
            QString message = lua_tostring(L, -1);
            auto lineError = getErrorLineNumber(message);
            emit runFinished(message + "\n", lineError);
            lua_pop(L, 1);
            return;
        }

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

        QString result;
        for (int i = 0; i < values.size(); i++) {
            auto tv = values[i];
            result += QString("TimeValue { time: %0, value: %1}\n").arg(tv.time).arg(tv.value);
        }
//        auto result = lua_tostring(L, -1);
        emit runFinished(result);

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
