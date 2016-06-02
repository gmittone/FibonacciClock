#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timeout();

private:
    Ui::MainWindow *ui;
    QTimer _tTimer;
    int _iMinute;

    void fibonacci(int value, bool array[5]);
    void fibonacci(int value, bool array[5], bool last[5]);
    void paintFibonacci(const QTime &time);
};

#endif // MAINWINDOW_H
