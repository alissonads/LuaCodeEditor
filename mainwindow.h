#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LuaCompilator;
class LuaProjectManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<LuaCompilator> _luaCompilator;
    std::shared_ptr<LuaProjectManager> _luaProjectManager;
};
#endif // MAINWINDOW_H
