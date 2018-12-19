#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "slmwindow.h"
#include "Wavefront.h"
#include "multifocalwavefront.h"

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


    void on_FullScreenButton_clicked();


    void on_PistonSlider_valueChanged(int value);
    void on_XTiltSlider_valueChanged(int value);

    void on_YTiltSlider_valueChanged(int value);

    void on_DefocusSlider_valueChanged(int value);

    void on_OblAstigSlider_valueChanged(int value);

    void on_VertAstigSlider_valueChanged(int value);

    void on_VertComaSlider_valueChanged(int value);

    void on_HorizComaSlider_valueChanged(int value);

    void on_VertTrefoilSlider_valueChanged(int value);

    void on_OblTrefoilSlider_valueChanged(int value);

    void on_PrimarySphericalSlider_valueChanged(int value);

    void on_Vert2ndAstig_valueChanged(int value);

    void on_Obl2ndAstig_valueChanged(int value);

    void on_VertQuadrafoil_valueChanged(int value);

    void on_OblQuadrafoil_valueChanged(int value);

    void on_CentreX_spinBox_valueChanged(int arg1);

    void on_CentreY_spinBox_valueChanged(int arg1);

    void on_beamTypeComboBox_currentIndexChanged(int index);

    void on_exitProgramButton_clicked();

    void closeEvent (QCloseEvent *event);
    void on_HologramSizeButton_valueChanged(int arg1);

    void on_topologicalChargeBtn_valueChanged(int arg1);

    void on_topologicalChargeBtn_2_valueChanged(double arg1);

    void on_maxBlazeSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    SlmWindow slmView;
    Wavefront wf = Wavefront(4,4);
    //MultiFocalWavefront mfwf = MultiFocalWavefront(512,512);
    int _width;
    int _height;
    uint8_t *slmDisplayPtr;
    QImage img;
    QPixmap pixels;

    void setDataPtr(uint8_t *dataPtr, int width, int height);

    void updatePixmap(uint8_t* dataPtr);
};

#endif // MAINWINDOW_H
