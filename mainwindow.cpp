#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDesktopWidget"
#include "Wavefront.h"
#include "multifocalwavefront.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QScreen>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*QRect rec = QApplication::desktop()->screenGeometry();
    int height = rec.height();
    int width = rec.width();*/
    int numScreens = QGuiApplication::screens().length();

    if(numScreens > 1)
    {
        QScreen *screen = QGuiApplication::screens()[1];
        QRect screenGeometry = screen->geometry();
        _height = screenGeometry.height();
        _width = screenGeometry.width();
    }
    else
    {
        QScreen *screen = QGuiApplication::screens()[0];
        QRect screenGeometry = screen->geometry();
        _height = screenGeometry.height();
        _width = screenGeometry.width();
    }

    /*int height = screenGeometry.height();
    int width = screenGeometry.width();

    std::cout<<width<<std::endl;*/
    wf = Wavefront::Wavefront(512,512, _width,_height);
    //MultiFocalWavefront mfwf = MultiFocalWavefront::MultiFocalWavefront(_height,_height, _width,_height);
    ui->setupUi(this);
    double ratio = (double)_width/(double(_height));
    QSize sz(480,(int)(480.0/ratio));
    ui->label->resize(sz);
    slmView.setDataPtr(wf.PhaseDisplay.data(), _width, _height);

    slmView.show();
    /*ui->CentreX_spinBox->setValue(_width/2);
    ui->CentreY_spinBox->setValue(_height/2);*/

    setDataPtr(wf.PhaseDisplay.data(), _width, _height);
    updatePixmap(wf.PhaseDisplay.data());
    QPoint position(_width,0);
    if(numScreens > 1)
    {
        slmView.makeFullScreen(true);
        slmView.move(position);
    }


}

MainWindow::~MainWindow()
{    
    delete ui;
}

void MainWindow::on_FullScreenButton_clicked()
{
    if(slmView.isFullScreen())
    {
        slmView.showNormal();
    }
    else
    {
        slmView.makeFullScreen(true);
        //slmView.showfullScreen();
    }
    //QRect rec = QApplication::desktop()->screenGeometry();
    //int height = rec.height();
    //int width = rec.width();

}

void MainWindow::on_PistonSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(0,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_XTiltSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(1,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_YTiltSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(2,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_DefocusSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(3,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_OblAstigSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(4,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_VertAstigSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(5,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_VertComaSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(6,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_HorizComaSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(7,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_VertTrefoilSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(8,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_OblTrefoilSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(9,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_PrimarySphericalSlider_valueChanged(int value)
{
    wf.updateAberrationFunction(10,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_Vert2ndAstig_valueChanged(int value)
{
    wf.updateAberrationFunction(11,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_Obl2ndAstig_valueChanged(int value)
{
    wf.updateAberrationFunction(12,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_VertQuadrafoil_valueChanged(int value)
{
    wf.updateAberrationFunction(13,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_OblQuadrafoil_valueChanged(int value)
{
    wf.updateAberrationFunction(14,(double)value);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_CentreX_spinBox_valueChanged(int arg1)
{
    wf.updateCentrePosition(arg1,ui->CentreY_spinBox->value());
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_CentreY_spinBox_valueChanged(int arg1)
{
    wf.updateCentrePosition(ui->CentreX_spinBox->value(),arg1);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_beamTypeComboBox_currentIndexChanged(int index)
{
    Wavefront::beamType bt = static_cast<Wavefront::beamType>(index);
    wf.setBeamType(bt);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_exitProgramButton_clicked()
{
    qApp->exit();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Holograms",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        qApp->exit();
        event->accept();
    }
}

void MainWindow::on_HologramSizeButton_valueChanged(int arg1)
{
    wf.resizeHologram(arg1,arg1);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_topologicalChargeBtn_valueChanged(int arg1)
{
    wf.setTopologicalCharge(arg1);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::on_topologicalChargeBtn_2_valueChanged(double arg1)
{
    wf.setStepRadius(arg1);
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

void MainWindow::setDataPtr(uint8_t *dataPtr, int width, int height)
{
    _width = width;
    _height = height;
    img = QImage((uchar*)dataPtr, _width, _height, QImage::Format_Grayscale8);
    pixels = pixels.fromImage(img);
    ui->label->setPixmap(pixels);

}

void MainWindow::updatePixmap(uint8_t* dataPtr)
{
    img = QImage((uchar*)dataPtr, _width, _height, QImage::Format_Grayscale8);
    pixels = pixels.fromImage(img);
    double ratio = (double)_width/(double)_height;
    pixels = pixels.scaled(480,(int)480.0/ratio);
    ui->label->setPixmap(pixels);
}

void MainWindow::on_maxBlazeSlider_sliderMoved(int position)
{
    wf.setMaxBlaze(position);
    ui->maxBlazeLabel->setText(QString::number(position));
    slmView.updatePixmap(wf.PhaseDisplay.data());
    updatePixmap(wf.PhaseDisplay.data());
}

