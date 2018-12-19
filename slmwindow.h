#ifndef SLMWINDOW_H
#define SLMWINDOW_H

#include <QMainWindow>

namespace Ui {
class SlmWindow;
}

class SlmWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SlmWindow(QWidget *parent = 0);
    void setDataPtr(uint8_t *dataPtr, int width, int height);
    void makeFullScreen(bool);
    void setSize(int width, int height);
    void updatePixmap(uint8_t* dataPtr);
    ~SlmWindow();

private:
    Ui::SlmWindow *ui;

    int _width;
    int _height;
    uint8_t *slmDisplayPtr;
    QImage img;
    QPixmap pixels;
};

#endif // SLMWINDOW_H
