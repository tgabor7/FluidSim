#pragma once

#include <QWidget>
#include "ui_MainForm.h"

class MainForm : public QWidget
{
	Q_OBJECT

public:
	MainForm(QWidget *parent = Q_NULLPTR);
	~MainForm();

private:
	Ui::MainForm ui;
	static void updateGL(OpenglWidget*);
public slots:
	void handlePlayButton();
	void updateRe(double);
	void updateUx(double);
	void updateUy(double);
	void resetAnimation();
	void selectPencil();
	void selectCursor();
	void selectCircle();
	void selectEraser();
	void setRadius(double);
};
