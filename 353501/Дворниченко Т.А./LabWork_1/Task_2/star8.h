#ifndef STAR8_H
#define STAR8_H

#include "shape.h"

class Star8: public Shape
{
public:
    Star8();
    float area() override;
    float perimeter() override;
private:
    float radius1=80;
    float radius2=100;
    float radius3=80;
    float radius4=100;
    int count=8;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void updateSizeRadiushaha(double arg1) override;

};

#endif // STAR8_H
