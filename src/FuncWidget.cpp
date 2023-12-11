#include "FuncWidget.h"
#include <QPainter>
#include "LogicalOperations.h"
#include "iostream"
#include<algorithm>

FuncWidget::FuncWidget(QWidget* parent) : QWidget(parent) { }

void FuncWidget::Create(const QSize& size, const int getElem)
{
	m_image = QImage(size, QImage::Format_ARGB32);
	m_image.fill(Qt::cyan);

	m_firstColor = Qt::black;
	m_secondColor = Qt::white;

	m_nNormalComponentIndex = getElem;
}

void FuncWidget::DrawFunc(const std::function<float(const QPointF&)>& rfunc, const QRectF& subSpace)
{
	QPointF spaceStep = { subSpace.width() / static_cast<float>(m_image.width()),
						 subSpace.height() / static_cast<float>(m_image.height()) };

	for (int x = 0; x < m_image.width() - 1; ++x)
	{
		for (int y = 0; y < m_image.height() - 1; ++y)
		{
			QPointF spacePoint1 = { subSpace.left() + static_cast<float>(x) * spaceStep.x(),
								   subSpace.top() + static_cast<float>(y) * spaceStep.y() };

			const float z1 = rfunc(spacePoint1);


			QPointF spacePoint2 = { subSpace.left() + static_cast<float>(x + 1) * spaceStep.x(),
								   subSpace.top() + static_cast<float>(y) * spaceStep.y() };

			const float z2 = rfunc(spacePoint2);

			QPointF spacePoint3 = { subSpace.left() + static_cast<float>(x) * spaceStep.x(),
								   subSpace.top() + static_cast<float>(y + 1) * spaceStep.y() };

			const float z3 = rfunc(spacePoint3);
			if (std::abs(z2) < 0.01f) {
				m_aCirclePoints.push_back(QPointF(x + 2, y + 8));
			}

			const float A = CalculateDeterminant3x3({
				{(float)spacePoint1.y(), z1, 1},
				{(float)spacePoint2.y(), z2, 1},
				{(float)spacePoint3.y(), z3, 1},
				});

			const float B = CalculateDeterminant3x3({
				{(float)spacePoint1.x(), z1, 1},
				{(float)spacePoint2.x(), z2, 1},
				{(float)spacePoint3.x(), z3, 1},
				});

			const float C = CalculateDeterminant3x3({
				{(float)spacePoint1.x(), (float)spacePoint1.y(), 1},
				{(float)spacePoint2.x(), (float)spacePoint2.y(), 1},
				{(float)spacePoint3.x(), (float)spacePoint3.y(), 1},
				});

			const float D = CalculateDeterminant3x3({
				{(float)spacePoint1.x(), (float)spacePoint1.y(), z1},
				{(float)spacePoint2.x(), (float)spacePoint2.y(), z2},
				{(float)spacePoint3.x(), (float)spacePoint3.y(), z3},
				});


			float det = std::sqrt(A * A + B * B + C * C + D * D);

			float nx = A / det;
			float ny = B / det;
			float nz = C / det;
			float nw = D / det;


			float norm = nx;

			switch (m_nNormalComponentIndex)
			{
			case 0:
				break;
			case 1:
				norm = ny;
				break;
			case 2:
				norm = nz;
				break;
			case 3:
				norm = nw;
				break;
			}

			auto pixelColor = InterpolateColors(m_firstColor, m_secondColor, (1.f + norm) / 2);
			m_image.setPixelColor(x, y, pixelColor);
		}
	}
	QVector<QPointF> fPoints;
	QVector<QPointF> sPoints;
	for (const auto& gradientPath : m_aCirclePoints)
	{
		if (gradientPath.y() > 103)
		{
			sPoints.push_back(gradientPath);
		}
		else
		{
			fPoints.push_back(gradientPath);
		}

	}
	firstRect = GetBoundingRect(fPoints);
	secondRect = GetBoundingRect(sPoints);
}

float FuncWidget::CalculateDeterminant3x3(const std::vector<std::vector<float>>& matrix)
{
	if (matrix.size() != 3 || matrix[0].size() != 3 || matrix[1].size() != 3 || matrix[2].size() != 3)
	{
		throw std::runtime_error("Wrong");
	}

	return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
		matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
		matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

void FuncWidget::UpdateImage(const QColor& firstColor, const QColor& secondColor)
{
	for (int x = 0; x < m_image.width() - 1; ++x)
	{
		for (int y = 0; y < m_image.height() - 1; ++y)
		{
			float t =
				(static_cast<float>(m_image.pixelColor(x, y).red()) - m_firstColor.red()) / (m_secondColor.red() - m_firstColor.red());
			auto pixelColor = InterpolateColors(firstColor, secondColor, t);
			m_image.setPixelColor(x, y, pixelColor);
		}
	}
	m_image = m_image.mirrored(false, true);
	m_firstColor = firstColor;
	m_secondColor = secondColor;
}

void FuncWidget::SaveImage(const QString& filename)
{
	m_image.save(filename);
}

void FuncWidget::Clear()
{
	m_aGradientPaths.clear();
	update();
}

QRectF FuncWidget::GetBoundingRect(const QVector<QPointF>& points)
{
	if (points.isEmpty())
		return QRectF();

	int xMin = points[0].x();
	int yMin = points[0].y();
	int xMax = points[0].x();
	int yMax = points[0].y();

	for (auto point : points) {
		xMin = std::min(xMin, (int)point.x());
		yMin = std::min(yMin, (int)point.y());
		xMax = std::max(xMax, (int)point.x());
		yMax = std::max(yMax, (int)point.y());
	}

	return QRectF(QPoint(xMin, yMin), QPoint(xMax, yMax));
}

QPolygonF FuncWidget::GradientDescent(QPointF start, QPointF target, double attractionConstant, double repulsionConstant)
{

	QPolygonF descentPath;
	QPointF currentPoint = start;




	descentPath.append(currentPoint);
	while (Distance(currentPoint, target) > 1.0) {
		currentPoint = GradientDescentStep(currentPoint, target, firstRect, secondRect, attractionConstant, repulsionConstant);
		descentPath.append(currentPoint);
	}
	return descentPath;
}


QColor FuncWidget::InterpolateColors(const QColor& colorFirst, const QColor& colorSec, float k)
{
	QColor val;
	val.setRed(static_cast<int>(colorFirst.red() + (colorSec.red() - colorFirst.red()) * k));
	val.setGreen(static_cast<int>(colorFirst.green() + (colorSec.green() - colorFirst.green()) * k));
	val.setBlue(static_cast<int>(colorFirst.blue() + (colorSec.blue() - colorFirst.blue()) * k));
	val.setAlpha(static_cast<int>(colorFirst.alpha() + (colorSec.alpha() - colorFirst.alpha()) * k));

	return val;
}

double FuncWidget::Distance(QPointF a, QPointF b)
{
	return std::sqrt(std::pow(a.x() - b.x(), 2) + std::pow(a.y() - b.y(), 2));
}

QPointF FuncWidget::RepulsionForce(QPointF current, QRectF obstacle, double constant)
{
	QPointF center = obstacle.center();
	double d = Distance(current, center);
	double r = std::min(obstacle.width(), obstacle.height()) / 2.0;
	if (d <= r) {
		return (current - center) * constant * (1.0 - d / r);
	}
	else {
		return QPointF(0, 0);
	}
}

QPointF FuncWidget::GradientDescentStep(QPointF current, QPointF target, QRectF obstacle1, QRectF obstacle2, double attractionConstant, double repulsionConstant)
{
	QPointF attraction = AttractionForce(current, target, attractionConstant);
	QPointF repulsion1 = RepulsionForce(current, obstacle1, repulsionConstant);
	QPointF repulsion2 = RepulsionForce(current, obstacle2, repulsionConstant);
	return current + attraction + repulsion1 + repulsion2;
}

QPointF FuncWidget::AttractionForce(QPointF current, QPointF target, double constant)
{
	return (target - current) * constant;
}

void FuncWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawImage(0, 0, m_image);
	QPen pen;
	pen.setColor(Qt::green);
	pen.setWidth(2);
	painter.setPen(pen);

	for (const auto& gradientPath : m_aGradientPaths)
	{
		painter.drawPolyline(gradientPath);
	}
}

void FuncWidget::mousePressEvent(QMouseEvent* event)
{
	std::function<float(const QPointF&)> functions[3];

	functions[0] = [](const QPointF& point) -> float { return (1 - std::pow((point.x() - 5), 2) - std::pow(point.y() - 5, 2)) * 1000000; };
	functions[1] = [](const QPointF& point) -> float { return (1 - std::pow((point.x() - 5), 2) - std::pow(point.y() - 8, 2)) * 1000000; };

	functions[2] = [](const QPointF& point) -> float { return -(1000 - std::pow((point.x() - 8), 2) - std::pow((point.y() - 8), 2)); };
	std::function<float(const QPointF&)> complexFunction = [&functions](const QPointF& point) -> float
		{
			auto w3 = LogicalOperations::LogicalOperations::And(LogicalOperations::And(functions[0](point), functions[1](point)), functions[2](point));
			return w3;
		};
	if (event->button() == Qt::LeftButton)
	{
		QPointF mouseClickPoint = event->localPos();
		QPolygonF descentPath = GradientDescent(mouseClickPoint, QPointF(150, 50), 0.01, 3);
		m_aGradientPaths.push_back(descentPath);
	}
}


void FuncWidget::mouseReleaseEvent(QMouseEvent* event)
{
	update();
}
