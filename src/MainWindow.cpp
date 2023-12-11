#include "MainWindow.h"
#include"LogicalOperations.h"
#include<QGridLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent)
{
	QGridLayout* layout = new QGridLayout(this);

	m_pToolBar = new QToolBar(this);
	m_pSaveAction = m_pToolBar->addAction("Save");
	m_pClearAction = m_pToolBar->addAction("Clear");
	connect(m_pSaveAction, &QAction::triggered, this, &MainWindow::SaveImages);
	connect(m_pClearAction, &QAction::triggered, this, &MainWindow::Clear);
	layout->addWidget(m_pToolBar, 0, 0, 1, 2);

	m_pWidgetNx = new FuncWidget(this);
	m_pWidgetNx->Create(QSize(200, 200), 0);
	layout->addWidget(m_pWidgetNx, 1, 0);

	m_pWidgetNy = new FuncWidget(this);
	m_pWidgetNy->Create(QSize(200, 200), 1);
	layout->addWidget(m_pWidgetNy, 1, 1);

	m_pWidgetNz = new FuncWidget(this);
	m_pWidgetNz->Create(QSize(200, 200), 2);
	layout->addWidget(m_pWidgetNz, 2, 0);

	m_pWidgetNw = new FuncWidget(this);
	m_pWidgetNw->Create(QSize(200, 200), 3);
	layout->addWidget(m_pWidgetNw, 2, 1);


	this->setFixedSize(430, 430);
}

void MainWindow::Draw()
{
	std::function<float(const QPointF&)> functions[3];

	functions[0] = [](const QPointF& point) -> float { return (1-std::pow((point.x()-5),2) -std::pow(point.y()-5,2))* 1000000; };
	functions[1] = [](const QPointF& point) -> float { return (1-std::pow((point.x()-5),2) -std::pow(point.y()-8,2))* 1000000; };

	functions[2] = [](const QPointF& point) -> float { return -(1000-std::pow((point.x()-8),2)-std::pow((point.y()-8),2)); };
	std::function<float(const QPointF&)> complexFunction = [&functions](const QPointF& point) -> float
		{
			auto w3 = LogicalOperations::LogicalOperations::And(LogicalOperations::And(functions[0](point), functions[1](point)), functions[2](point));
			return w3;
		};
	QRectF subSpace(-6.f, -3.f, 20.f, 20.f);

	m_pWidgetNx->DrawFunc(complexFunction, subSpace);
	m_pWidgetNy->DrawFunc(complexFunction, subSpace);
	m_pWidgetNz->DrawFunc(complexFunction, subSpace);
	m_pWidgetNw->DrawFunc(complexFunction, subSpace);

}

void MainWindow::Update(const QColor& firstColor, const QColor& secondColor)
{
	m_pWidgetNw->UpdateImage(firstColor, secondColor);
	m_pWidgetNz->UpdateImage(firstColor, secondColor);
	m_pWidgetNy->UpdateImage(firstColor, secondColor);
	m_pWidgetNx->UpdateImage(firstColor, secondColor);
}

void MainWindow::SaveImages()
{
	m_pWidgetNx->SaveImage("Nx.png");
	m_pWidgetNy->SaveImage("Ny.png");
	m_pWidgetNz->SaveImage("Nz.png");
	m_pWidgetNw->SaveImage("Nw.png");
}

void MainWindow::Clear()
{
	m_pWidgetNx->Clear();
	m_pWidgetNy->Clear();
	m_pWidgetNz->Clear();;
	m_pWidgetNw->Clear();
}
