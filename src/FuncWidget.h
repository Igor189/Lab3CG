#pragma once
#include <QImage>
#include <QWidget>
#include<QMouseEvent>
#include<qvector.h>
#include <qpolygon.h>

class FuncWidget : public QWidget
{
public:
    FuncWidget(QWidget* parent = nullptr);

    void Create(const QSize& size, const int getElem);
    void DrawFunc(const std::function<float(const QPointF&)>& rfunc, const QRectF& subSpace);

    float CalculateDeterminant3x3(const std::vector<std::vector<float>>& matrix);

    void UpdateImage(const QColor& firstColor, const QColor& secondColor);
    void SaveImage(const QString& filename);
    void Clear();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QColor m_firstColor;
    QColor m_secondColor;
    QImage m_image;
    QVector<QPolygonF> m_aGradientPaths;
    int m_nNormalComponentIndex;
    QVector<QPointF> m_aCirclePoints;
    QRectF firstRect;
    QRectF secondRect;
    QRectF GetBoundingRect(const QVector<QPointF>& points);
    QPolygonF GradientDescent(QPointF start, QPointF target, double attractionConstant, double repulsionConstant);
    QColor InterpolateColors(const QColor& colorFirst, const QColor& colorSec, float k);
    double Distance(QPointF a, QPointF b);
    QPointF RepulsionForce(QPointF current, QRectF obstacle, double constant);
    QPointF GradientDescentStep(QPointF current, QPointF target, QRectF obstacle1, QRectF obstacle2, double attractionConstant, double repulsionConstant);
    QPointF AttractionForce(QPointF current, QPointF target, double constant);
};