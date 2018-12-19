#include "slmwindow.h"
#include "ui_slmwindow.h"
#include "QApplication"
#include "QDesktopWidget"
#include <QScreen>

SlmWindow::SlmWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SlmWindow)
{
    ui->setupUi(this);

    /*QString filePath = "C://Users//Hugo//Pictures//spiralPol.png";
    QImage im(filePath);
    QPixmap px;
    px = QPixmap::fromImage(im);
    //px = px.scaled(1024,1024);
    ui->imLabel->setPixmap(px);*/
}

void SlmWindow::setDataPtr(uint8_t *dataPtr, int width, int height)
{
    _width = width;
    _height = height;
    img = QImage((uchar*)dataPtr, _width, _height, QImage::Format_Grayscale8);
    pixels = pixels.fromImage(img);
    ui->imLabel->setPixmap(pixels);

}

void SlmWindow::makeFullScreen(bool)
{

    int numberScreens = QGuiApplication::screens().length();

    if(numberScreens > 1)
    {
        QScreen *screen = QGuiApplication::screens()[1];
        QRect screenGeometry = screen->geometry();
        int height = screenGeometry.height();
        int width = screenGeometry.width();

        /*QRect rec = QApplication::desktop()->screenGeometry();
        int height = rec.height();
        int width = rec.width();*/
        QSize sz(width,height);
        ui->imLabel->resize(sz);

        pixels = pixels.scaled(width,height);
        ui->imLabel->setPixmap(pixels);
        this->setMaximumHeight(height);
        this->setMaximumWidth(width);
        this->showFullScreen();
    }
    else
    {
        QScreen *screen = QGuiApplication::screens()[0];
        QRect screenGeometry = screen->geometry();
        int height = screenGeometry.height();
        int width = screenGeometry.width();

        /*QRect rec = QApplication::desktop()->screenGeometry();
        int height = rec.height();
        int width = rec.width();*/
        QSize sz(width,height);
        ui->imLabel->resize(sz);

        pixels = pixels.scaled(width,height);
        ui->imLabel->setPixmap(pixels);
        this->setMaximumHeight(height);
        this->setMaximumWidth(width);
        this->showFullScreen();
    }
    /*QScreen *screen = QGuiApplication::screens()[1];
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    /*QRect rec = QApplication::desktop()->screenGeometry();
    int height = rec.height();
    int width = rec.width();
    QSize sz(width,height);
    ui->imLabel->resize(sz);

    pixels = pixels.scaled(width,height);
    ui->imLabel->setPixmap(pixels);
    this->setMaximumHeight(height);
    this->setMaximumWidth(width);
    this->showFullScreen();*/
}

void SlmWindow::setSize(int width, int height)
{
    QSize sz(width,height);
    ui->imLabel->resize(sz);
}

void SlmWindow::updatePixmap(uint8_t* dataPtr)
{
    img = QImage((uchar*)dataPtr, _width, _height, QImage::Format_Grayscale8);
    pixels = pixels.fromImage(img);
    ui->imLabel->setPixmap(pixels);
}

SlmWindow::~SlmWindow()
{
    delete ui;
}
