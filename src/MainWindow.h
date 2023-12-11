#pragma once
#include<QWidget>
#include <QToolBar>
#include <QAction>
#include"FuncWidget.h"

class MainWindow : public QWidget
{
public:
    MainWindow(QWidget* parent = nullptr);

    void Draw();
    void Update(const QColor& firstColor, const QColor& secondColor);

private:
    FuncWidget* m_pWidgetNz;
    FuncWidget* m_pWidgetNy;
    FuncWidget* m_pWidgetNx;
    FuncWidget* m_pWidgetNw;

    QToolBar* m_pToolBar;
    QAction* m_pSaveAction;
    QAction* m_pClearAction;

    void SaveImages();
    void Clear();
};