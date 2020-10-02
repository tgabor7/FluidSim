#pragma once

#include <QOpenGLWidget>
#include "ui_OpenglWidget.h"
#include <QTextEdit>

class OpenglWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenglWidget(QWidget *parent = Q_NULLPTR);
	QTextEdit *text;
	void startAnimation();
	void updateRe(int);
	void resetAnimation();
	
	bool running;
	~OpenglWidget();

private:
	Ui::OpenglWidget ui;
	void paintGL();
	void initializeGL();

private slots:
	void updateSimulation();
};
