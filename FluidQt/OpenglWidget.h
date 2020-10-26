#pragma once

#include <QOpenGLWidget>
#include "ui_OpenglWidget.h"
#include <QTextEdit>
#include <QLabel>

enum Tool {
	CURSOR,
	PAINT,
	CIRCLE,
	BOX,
	ERASER
};

class OpenglWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenglWidget(QWidget *parent = Q_NULLPTR);
	QTextEdit *text;
	QLabel* xlabel;
	QLabel* ylabel;
	QLabel* uxlabel;
	QLabel* uylabel;
	QLabel* flabel;
	QLabel* maxLabel;

	Tool active_tool;
	void setRadius(double);
	void startAnimation();
	void updateRe(double);
	void updateUx(double);
	void updateUy(double);
	void resetAnimation();
	void setSolid(int, int, bool);
	bool running;
	~OpenglWidget();

private:
	Ui::OpenglWidget ui;
	void paintGL();
	void initializeGL();
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	int oldx;
	int oldy;
	bool* oldSolid;
	double strokewidth;
	double radius;
	void drawLine(int, int, int, int, bool);
	void drawCirlce(int, int, double);
	int sgn(double d);
	int lbgkwidth;
	int lbgkheight;
	double xscale;
	double yscale;

private slots:
	void updateSimulation();
};
