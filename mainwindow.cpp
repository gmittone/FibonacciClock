#include "mainwindow.h"
#include "ui_mainwindow.h"

#define RED     "background-color:red;"
#define WHITE   "background-color:white;"
#define GREEN   "background-color:green;"
#define BLUE    "background-color:blue;"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize Fibonacci time
    _iMinute = 13;
    paintFibonacci(QTime::currentTime());

    // Timer startup
    connect(&_tTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    _tTimer.start(1000);
}

MainWindow::~MainWindow()
{
    // Stop timer
    _tTimer.stop();

    delete ui;
}

void MainWindow::timeout()
{
    QTime time = QTime::currentTime();
    // Paint Fibonacci time
    paintFibonacci(time);
    // Paint human readable time
    ui->orario->setText(time.toString());
}

void MainWindow::fibonacci(int value, bool array[5])
{
    bool last[5] = {false,false,false,false};
    fibonacci(value, array, last);
}

void MainWindow::fibonacci(int value, bool array[5], bool last[5])
{
    int f[5] = {1,1,2,3,5}, temp = value;
    // Optimize visualization
    for(int i = 4; i >= 0; i--)
    {
        if(!last[i] && (array[i] = (temp - f[i] >= 0)))
            temp -= f[i];
    }
    // Regular Fibonacci calculation
    if(temp != 0)
    {
        temp = value;
        for(int i = 4; i >= 0; i--)
        {
            if((array[i] = (temp - f[i] >= 0)))
                temp -= f[i];
        }
    }
}

void MainWindow::paintFibonacci(const QTime &time)
{
    int minute = time.minute() / 5;
    // Optimize paints
    if(minute == _iMinute)
        return;
    // Calculate Fibonacci arrays to paint
    _iMinute = minute;
    bool fH[5], fM[5];
    fibonacci(time.hour() % 12, fH);
    fibonacci(minute, fM, fH);
    // Paint colors
    for(int i = 0; i < 5; i++)
    {
        // Set colors
        QString stylesheet;
        if(!fH[i] && !fM[i])
            stylesheet = WHITE;
        else if(!fH[i] && fM[i])
            stylesheet = GREEN;
        else if(fH[i] && !fM[i])
            stylesheet = RED;
        else if(fH[i] && fM[i])
            stylesheet = BLUE;
        // Put colors to labels
        switch(i)
        {
        case 0: ui->l1_2->setStyleSheet(stylesheet);
        case 1: ui->l1->setStyleSheet(stylesheet);
        case 2: ui->l2->setStyleSheet(stylesheet);
        case 3: ui->l3->setStyleSheet(stylesheet);
        case 4: ui->l5->setStyleSheet(stylesheet);
        }
    }
}
