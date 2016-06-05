#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QList>
#include <QLabel>
#include <QWidget>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void paintFibonacci();
    void onPrecision5();
    void onPrecision1();
    void onHumanReadable();

private:
    enum PrecisionType {NONE, PRECISION_5MIN, PRECISION_1MIN};
    QTimer _tTimer;
    QWidget *_wCentral;
    QLabel **_lLabels, *_lHR, *_lHourColor, *_lHourLabel, *_lMinuteColor, *_lMinuteLabel, *_lBothColor, *_lBothLabel;
    QToolBar *_tToolbar;
    QAction *_a5M, *_a1M, *_aHR;
    int _iMinute;
    PrecisionType _pType;
    bool _bHumanReadable;
    static const int SEQUENCE[];

    void fibonacci(int value, bool array[5]);
    void fibonacci(int value, bool array[5], bool last[5]);
    void setMode(PrecisionType type);
    void setupUI();
    QLabel *createSquare(int x, int y, int width, int heigh);
};

#endif // MAINWINDOW_H
