#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(uint nodes_num,QWidget *parent = nullptr);
    ~MainWindow();
uint nodes_num_;
private slots:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
