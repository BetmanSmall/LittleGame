#ifndef CIRCLE_H
#define CIRCLE_H

#include <QDebug>
#include <QtMath>
#include <QPointF>

class Circle
{
public:
    float x, y;
    float radius;

public:
    Circle();
    Circle(float x, float y, float radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }

    Circle(QPointF position, float radius) {
        this->x = position.x();
        this->y = position.y();
        this->radius = radius;
    }

    Circle(Circle* circle) {
        this->x = circle->x;
        this->y = circle->y;
        this->radius = circle->radius;
    }

//    Circle(Vector2 center, Vector2 edge) {
//        this->x = center.x;
//        this->y = center.y;
//        this->radius = Vector2.len(center.x - edge.x, center.y - edge.y);
//    }

    void set(float x, float y, float radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }

    void set(QPointF position, float radius) {
        this->x = position.x();
        this->y = position.y();
        this->radius = radius;
    }

    void set(Circle* circle) {
        this->x = circle->x;
        this->y = circle->y;
        this->radius = circle->radius;
    }

//    void set(Vector2 center, Vector2 edge) {
//        this->x = center.x;
//        this->y = center.y;
//        this->radius = Vector2.len(center.x - edge.x, center.y - edge.y);
//    }

    void setPosition(QPointF position) {
        this->x = position.x();
        this->y = position.y();
    }

    void setPosition(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void setX(float x) {
        this->x = x;
    }

    void setY(float y) {
        this->y = y;
    }

    void setRadius(float radius) {
        this->radius = radius;
    }

    bool contains(float x, float y) {
        x = this->x - x;
        y = this->y - y;
        return x * x + y * y <= radius * radius;
    }

    bool contains(QPointF point) {
        float dx = x - point.x();
        float dy = y - point.y();
        return dx * dx + dy * dy <= radius * radius;
    }

    bool contains(Circle* c) {
        float radiusDiff = radius - c->radius;
        if (radiusDiff < 0.0) return false; // Can't contain bigger circle
        float dx = x - c->x;
        float dy = y - c->y;
        float dst = dx * dx + dy * dy;
        float radiusSum = radius + c->radius;
        return (!(radiusDiff * radiusDiff < dst) && (dst < radiusSum * radiusSum));
    }

    bool overlaps(Circle* c) {
        float dx = x - c->x;
        float dy = y - c->y;
        float distance = dx * dx + dy * dy;
        float radiusSum = radius + c->radius;
        return distance < radiusSum * radiusSum;
    }

    QString toString() {
        QString sb("Circle[");
        sb.append("x:" + QString::number(x));
        sb.append(",y:" + QString::number(y));
        sb.append(",radius:" + QString::number(radius));
        sb.append("]");
        return sb;
    }

    float circumference() {
        return this->radius * (M_PI*2);
    }

    float area() {
        return this->radius * this->radius * M_PI;
    }

    bool equals(Circle* c) {
        if (c == this) return true;
        if (c == NULL) return false;
        return this->x == c->x && this->y == c->y && this->radius == c->radius;
    }
};

#endif // CIRCLE_H
