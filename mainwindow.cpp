#include "mainwindow.h"
#include "ui_mainwindow.h"

#define COLOR_HOUR        "background-color:red;"
#define COLOR_NONE        "background-color:white;"
#define COLOR_MINUTE      "background-color:green;"
#define COLOR_BOTH        "background-color:blue;"
#define SIZE        15

const int MainWindow::SEQUENCE[] = {1,1,2,3,5,8,13,21,34};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();

    // Initialize Fibonacci time
    _pType = NONE;
    _bHumanReadable = false;
    _iMinute = 61;
    setMode(PRECISION_5MIN);
    paintFibonacci();

    // Timer startup
    connect(&_tTimer, SIGNAL(timeout()), this, SLOT(paintFibonacci()));
    _tTimer.start(1000);
}

MainWindow::~MainWindow()
{
    // Stop timer
    _tTimer.stop();

    delete _lLabels;
}

void MainWindow::fibonacci(int value, bool array[])
{
    bool last[] = {false,false,false,false,false,false};
    fibonacci(value, array, last);
}

void MainWindow::fibonacci(int value, bool array[], bool last[])
{
    int temp = value, start;
    switch(_pType)
    {
    case PRECISION_5MIN:
        start = 4;
        break;
    case PRECISION_1MIN:
        start = 8;
        break;
    default:
        return;
    }
    // Optimize visualization
    for(int i = start; i >= 0; i--)
    {
        if(last[i])
            array[i] = false;
        else if((array[i] = (temp - SEQUENCE[i] >= 0)))
            temp -= SEQUENCE[i];
    }
    // Regular Fibonacci calculation
    if(temp != 0)
    {
        temp = value;
        for(int i = start; i >= 0; i--)
        {
            if((array[i] = (temp - SEQUENCE[i] >= 0)))
                temp -= SEQUENCE[i];
        }
    }
}

void MainWindow::paintFibonacci()
{
    QTime time = QTime::currentTime();
    int hour, minute, realMinute, end;
    switch(_pType)
    {
    case PRECISION_5MIN:
        hour = time.hour() % 12;
        minute = time.minute() / 5;
        realMinute = minute * 5;
        end = 5;
        break;
    case PRECISION_1MIN:
        hour = time.hour();
        realMinute = minute = time.minute();
        end = 9;
        break;
    default:
        return;
    }
    // Optimize paints
    if(minute == _iMinute)
        return;
    // Calculate Fibonacci arrays to paint
    _iMinute = minute;
    bool fH[9], fM[9];
    fibonacci(hour, fH);
    fibonacci(minute, fM, fH);
    // Paint colors
    for(int i = 0; i < end; i++)
    {
        // Set colors
        QString stylesheet;
        if(!fH[i] && !fM[i])
            stylesheet = COLOR_NONE;
        else if(!fH[i] && fM[i])
            stylesheet = COLOR_MINUTE;
        else if(fH[i] && !fM[i])
            stylesheet = COLOR_HOUR;
        else if(fH[i] && fM[i])
            stylesheet = COLOR_BOTH;
        // Put colors to labels
        _lLabels[i]->setStyleSheet(stylesheet);
    }

    // Paint human readable time
    _lHR->setText(QString("%1:%2").arg(hour, 2, 10, QChar('0')).arg(realMinute, 2, 10, QChar('0')));
}

void MainWindow::setMode(PrecisionType type)
{
    if((type == PRECISION_5MIN) || (type == PRECISION_1MIN))
    {
        // Update label size and visibility
        if(type != _pType)
        {
            _lLabels[0]->setGeometry(QRect(SIZE * 4, 0, SIZE * 2, SIZE * 2));
            _lLabels[1]->setGeometry(QRect(SIZE * 4, SIZE * 2, SIZE * 2, SIZE * 2));
            _lLabels[2]->setGeometry(QRect(0, 0, SIZE * 4, SIZE * 4));
            _lLabels[3]->setGeometry(QRect(0, SIZE * 4, SIZE * 6, SIZE * 6));
            _lLabels[4]->setGeometry(QRect(SIZE * 6, 0, SIZE * 10, SIZE * 10));
            _lLabels[5]->setVisible(false);
            _lLabels[6]->setVisible(false);
            _lLabels[7]->setVisible(false);
            _lLabels[8]->setVisible(false);

        }
        // Update window size
        if(type == PRECISION_5MIN)
        {
            setMinimumSize(QSize((SIZE * 16) + (_bHumanReadable?200:0), (SIZE * 10) + _tToolbar->size().height()));
            setMaximumSize(minimumSize());
            setGeometry(QRect(geometry().left(), geometry().top(), (SIZE * 16) + (_bHumanReadable?200:0), (SIZE * 10) + _tToolbar->size().height()));
            _wCentral->setMaximumSize(QSize((SIZE * 16) + (_bHumanReadable?200:0), SIZE * 10));
            _wCentral->setGeometry(QRect(0, _tToolbar->size().height(), (SIZE * 16) + (_bHumanReadable?200:0), SIZE * 10));
            _lHR->setGeometry(QRect(SIZE * 16, 0, 200, 60));
            _lHourColor->setGeometry(QRect((SIZE * 16) + 4, 64, 20, 24));
            _lHourLabel->setGeometry(QRect((SIZE * 16) + 30, 64, 170, 24));
            _lMinuteColor->setGeometry(QRect((SIZE * 16) + 4, 94, 20, 24));
            _lMinuteLabel->setGeometry(QRect((SIZE * 16) + 30, 94, 170, 24));
            _lBothColor->setGeometry(QRect((SIZE * 16) + 4, 124, 20, 24));
            _lBothLabel->setGeometry(QRect((SIZE * 16) + 30, 124, 170, 24));
            _a5M->setIcon(QIcon(":/toolbar/resources/5b.png"));
            _a1M->setIcon(QIcon(":/toolbar/resources/1.png"));
        }
    }
    if(type == PRECISION_1MIN)
    {
        // Update label size and visibility
        if(type != _pType)
        {
            _lLabels[0]->setGeometry(QRect(SIZE * 2, 0, SIZE, SIZE));
            _lLabels[1]->setGeometry(QRect(SIZE * 2, SIZE, SIZE, SIZE));
            _lLabels[2]->setGeometry(QRect(0, 0, SIZE * 2, SIZE * 2));
            _lLabels[3]->setGeometry(QRect(0, SIZE * 2, SIZE * 3, SIZE * 3));
            _lLabels[4]->setGeometry(QRect(SIZE * 3, 0, SIZE * 5, SIZE * 5));
            _lLabels[5]->setVisible(true);
            _lLabels[6]->setVisible(true);
            _lLabels[7]->setVisible(true);
            _lLabels[8]->setVisible(true);
        }
        // Update window size
        setMinimumSize(QSize((SIZE * 55) + (_bHumanReadable?200:0), (SIZE * 34) + _tToolbar->size().height()));
        setMaximumSize(minimumSize());
        setGeometry(QRect(geometry().left(), geometry().top(), (SIZE * 55) + (_bHumanReadable?200:0), (SIZE * 34) + _tToolbar->size().height()));
        _wCentral->setMaximumSize(QSize((SIZE * 55) + (_bHumanReadable?200:0), SIZE * 34));
        _wCentral->setGeometry(QRect(0, _tToolbar->size().height(), (SIZE * 55) + (_bHumanReadable?200:0), SIZE * 34));
        _lHR->setGeometry(QRect(SIZE * 55, 0, 200, 60));
        _lHourColor->setGeometry(QRect((SIZE * 55) + 4, 64, 20, 24));
        _lHourLabel->setGeometry(QRect((SIZE * 55) + 30, 64, 170, 24));
        _lMinuteColor->setGeometry(QRect((SIZE * 55) + 4, 94, 20, 24));
        _lMinuteLabel->setGeometry(QRect((SIZE * 55) + 30, 94, 170, 24));
        _lBothColor->setGeometry(QRect((SIZE * 55) + 4, 124, 20, 24));
        _lBothLabel->setGeometry(QRect((SIZE * 55) + 30, 124, 170, 24));
        _a5M->setIcon(QIcon(":/toolbar/resources/5.png"));
        _a1M->setIcon(QIcon(":/toolbar/resources/1b.png"));
    }
    _pType = type;

    // Show or hide human readable time
    _lHR->setVisible(_bHumanReadable);
    if(_bHumanReadable)
    {
        for(int i = 0; i < 9; i++)
        {
            _lLabels[i]->setText(QString("%1").arg(SEQUENCE[i]));
            QFont f = _lLabels[i]->font();
            f.setPointSize(_lLabels[i]->width() * 0.6);
            _lLabels[i]->setFont(f);
        }
        _aHR->setIcon(QIcon(":/toolbar/resources/hrb.png"));
    }
    else
    {
        for(int i = 0; i < 9; i++)
            _lLabels[i]->setText("");
        _aHR->setIcon(QIcon(":/toolbar/resources/hr.png"));
    }
}


void MainWindow::onPrecision5()
{
    _tTimer.stop();
    setMode(PRECISION_5MIN);
    _iMinute = 61;
    paintFibonacci();
    _tTimer.start(1000);
}

void MainWindow::onPrecision1()
{
    _tTimer.stop();
    setMode(PRECISION_1MIN);
    _iMinute = 61;
    paintFibonacci();
    _tTimer.start(1000);
}

void MainWindow::onHumanReadable()
{
    _tTimer.stop();
    _bHumanReadable = !_bHumanReadable;
    setMode(_pType);
    _tTimer.start(1000);
}

void MainWindow::setupUI()
{
    QFont font;

    // Initialize MainWindow
    setObjectName("MainWindow");
    setWindowTitle(QApplication::translate("MainWindow", "The Fibonacci Clock", 0));
    _wCentral = new QWidget(this);

    font.setPointSize(40);
    _lHR = new QLabel(_wCentral);
    _lHR->setText("00:00");
    _lHR->setFont(font);
    _lHR->setAlignment(Qt::AlignCenter);

    font.setPointSize(12);
    _lHourColor = new QLabel(_wCentral);
    _lHourColor->setStyleSheet(COLOR_HOUR);
    _lHourLabel = new QLabel(_wCentral);
    _lHourLabel->setText(QApplication::translate("MainWindow", "Hour", 0));
    _lHourLabel->setFont(font);
    _lHourLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _lMinuteColor = new QLabel(_wCentral);
    _lMinuteColor->setStyleSheet(COLOR_MINUTE);
    _lMinuteLabel = new QLabel(_wCentral);
    _lMinuteLabel->setText(QApplication::translate("MainWindow", "Minute", 0));
    _lMinuteLabel->setFont(font);
    _lMinuteLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _lBothColor = new QLabel(_wCentral);
    _lBothColor->setStyleSheet(COLOR_BOTH);
    _lBothLabel = new QLabel(_wCentral);
    _lBothLabel->setText(QApplication::translate("MainWindow", "Hour & Minute", 0));
    _lBothLabel->setFont(font);
    _lBothLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    setCentralWidget(_wCentral);

    // Initialize Toolbar
    _tToolbar = new QToolBar(this);
    _tToolbar->setMovable(false);
    _tToolbar->setFloatable(false);
    _a5M = _tToolbar->addAction(QIcon(":/toolbar/resources/5.png"), "5 Minutes Precision", this, SLOT(onPrecision5()));
    _a1M = _tToolbar->addAction(QIcon(":/toolbar/resources/1.png"), "1 Minute Precision", this, SLOT(onPrecision1()));
    _tToolbar->addSeparator();
    _aHR = _tToolbar->addAction(QIcon(":/toolbar/resources/hr.png"), "Human Readable Time Representation", this, SLOT(onHumanReadable()));
    addToolBar(Qt::TopToolBarArea, _tToolbar);

    // Initialize Labels
    _lLabels = new QLabel*[9];
    // 1
    _lLabels[0] = createSquare(SIZE * 2, 0, SIZE, SIZE);
    // 1
    _lLabels[1] = createSquare(SIZE * 2, SIZE, SIZE, SIZE);
    // 2
    _lLabels[2] = createSquare(0, 0, SIZE * 2, SIZE * 2);
    // 3
    _lLabels[3] = createSquare(0, SIZE * 2, SIZE * 3, SIZE * 3);
    // 5
    _lLabels[4] = createSquare(SIZE * 3, 0, SIZE * 5, SIZE * 5);
    // 8
    _lLabels[5] = createSquare(0, SIZE * 5, SIZE * 8, SIZE * 8);
    // 13
    _lLabels[6] = createSquare(SIZE * 8, 0, SIZE * 13, SIZE * 13);
    // 21
    _lLabels[7] = createSquare(0, SIZE * 13, SIZE * 21, SIZE * 21);
    // 34
    _lLabels[8] = createSquare(SIZE * 21, 0, SIZE * 34, SIZE * 34);
}

QLabel *MainWindow::createSquare(int x, int y, int width, int height)
{
    QLabel *l = new QLabel(_wCentral);
    l->setGeometry(QRect(x, y, width, height));
    l->setFrameShape(QFrame::Box);
    l->setFrameShadow(QFrame::Plain);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    l->show();
    return l;
}
