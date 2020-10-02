#include "MainForm.h"
#include "OpenglWidget.h"
#include <QTimer>


void MainForm::resetAnimation()
{
	ui.openGLWidget->resetAnimation();
	ui.pushButton->setText("Play");
	ui.openGLWidget->running = false;
}

void MainForm::updateRe(int i)
{
	ui.openGLWidget->updateRe(i);
}

void MainForm::handlePlayButton()
{
	ui.openGLWidget->startAnimation();
	if (ui.openGLWidget->running) {
		ui.pushButton->setText("Stop");
	}
	else {
		ui.pushButton->setText("Play");

	}
}

MainForm::MainForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.openGLWidget->text = ui.textEdit;
	
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(handlePlayButton()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(resetAnimation()));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateRe(int)));
	
}
void MainForm::updateGL(OpenglWidget* gl)
{
	gl->update();
}
MainForm::~MainForm()
{
}
