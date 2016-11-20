#ifndef RENDERING_H
#define RENDERING_H

#include <QDialog>
#include "tmat.hpp"
#include "libscn.hpp"
#include <QGraphicsScene>
#include "libscnqt.h"

namespace Ui {
class rendering;
}

class rendering : public QDialog
{
    Q_OBJECT

public:
     explicit rendering(QWidget *parent, Image3D &img);
    ~rendering();


public slots:
    void on_horizontalSlider_A_valueChanged(int);
    void on_horizontalSlider_B_valueChanged(int);
    void on_horizontalSlider_C_valueChanged(int);

private:
    Ui::rendering *ui;
    void writeImage();
    const Image3D &img;
    TMat tm,tmC,tmS,tmR1,tmR2,tmR3;
    QGraphicsScene *scene;
    libscnqt libScnQt;
    QPixmap pix;
    Image3D MIP;
    QByteArray qBA;
};

#endif // RENDERING_H
