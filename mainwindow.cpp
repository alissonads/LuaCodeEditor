#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QQmlContext>
#include "LuaCompilator.h"
#include "LuaProjectManager.h"
#include "FileManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _luaCompilator(LuaCompilator::getInstance())
    , _luaProjectManager(std::make_shared<LuaProjectManager>())
{
    ui->setupUi(this);

    auto context = ui->quickWidget->rootContext();
    context->setContextProperty("LuaCompilatorCpp", _luaCompilator.get());
    context->setContextProperty("LuaProjectManagerCpp", _luaProjectManager.get());
    ui->quickWidget->setSource(QUrl("qrc:/EditorScreen.qml"));

    connect(_luaCompilator.get(), &LuaCompilator::buildFinished, [&] (const QString& result, int lineError){
        auto target = (QObject*)ui->quickWidget->rootObject();
        QMetaObject::invokeMethod(target, "clearLog", Qt::QueuedConnection);
        QMetaObject::invokeMethod(target, "addLogValue", Qt::QueuedConnection, Q_ARG(QVariant, result));
        QMetaObject::invokeMethod(target, "setErrorLine", Qt::QueuedConnection, Q_ARG(QVariant, lineError));
    });
    connect(_luaCompilator.get(), &LuaCompilator::runFinished, [&] (const QString& result, int lineError){
        auto target = (QObject*)ui->quickWidget->rootObject();
        QMetaObject::invokeMethod(target, "addLogValue", Qt::QueuedConnection, Q_ARG(QVariant, result));
        QMetaObject::invokeMethod(target, "setErrorLine", Qt::QueuedConnection, Q_ARG(QVariant, lineError));
    });
    connect(_luaCompilator.get(), &LuaCompilator::printResult, [&] (const QString& result){
        auto target = (QObject*)ui->quickWidget->rootObject();
        QMetaObject::invokeMethod(target, "addLogValue", Qt::QueuedConnection, Q_ARG(QVariant, result));
    });

    connect(_luaProjectManager.get(), &LuaProjectManager::optNewFile, [&] (const QString& baseCode) {
        auto target = (QObject*)ui->quickWidget->rootObject();
        QMetaObject::invokeMethod(target, "setInitialCode", Qt::QueuedConnection, Q_ARG(QVariant, baseCode));
    });
    connect(_luaProjectManager.get(), &LuaProjectManager::optOpenFile, [&] (const QString& baseCode) {
        auto target = (QObject*)ui->quickWidget->rootObject();
        QMetaObject::invokeMethod(target, "setInitialCode", Qt::QueuedConnection, Q_ARG(QVariant, baseCode));
    });
    connect(_luaProjectManager.get(), &LuaProjectManager::optSaveFile, [&] (bool status) {
//        auto target = (QObject*)ui->quickWidget->rootObject();
//        QMetaObject::invokeMethod(target, "setInitialCode", Qt::QueuedConnection, Q_ARG(QVariant, baseCode));
    });

    FileManager fileManager;
    auto bytes = fileManager.loadCsv("BTCUSDT-1m-2023-09.csv");
    _luaCompilator->setTickDataList(bytes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

