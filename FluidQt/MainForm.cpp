#include "MainForm.h"
#include "OpenglWidget.h"
#include <QTimer>
#include <QLayout>

void MainForm::selectEraser()
{
	ui.pushButton_2->setEnabled(true);
	ui.pushButton_4->setEnabled(true);
	ui.pushButton_5->setEnabled(true);
	ui.pushButton_6->setEnabled(false);
	ui.openGLWidget->active_tool = Tool::ERASER;
}
void MainForm::setRadius(double d)
{
	ui.openGLWidget->setRadius(d);
}
void MainForm::updateUx(double d)
{
	ui.openGLWidget->updateUx(d);
}
void MainForm::updateUy(double d)
{
	ui.openGLWidget->updateUy(d);
}
void MainForm::resetAnimation()
{
	ui.openGLWidget->resetAnimation();
	ui.pushButton->setText("Play");
	ui.openGLWidget->running = false;
	ui.openGLWidget->updateRe(ui.doubleSpinBox->value());
}
void MainForm::selectPencil()
{
	ui.pushButton_2->setEnabled(false);
	ui.pushButton_4->setEnabled(true);
	ui.pushButton_5->setEnabled(true);
	ui.pushButton_6->setEnabled(true);
	setRadius(2);
	ui.doubleSpinBox_4->setValue(2);
	ui.openGLWidget->active_tool = Tool::PAINT;
}
void MainForm::selectCircle()
{
	setRadius(10);
	ui.doubleSpinBox_4->setValue(20);

	ui.pushButton_2->setEnabled(true);
	ui.pushButton_4->setEnabled(true);
	ui.pushButton_6->setEnabled(true);
	ui.pushButton_5->setEnabled(false);
	ui.openGLWidget->active_tool = Tool::CIRCLE;
}
void MainForm::selectCursor()
{
	ui.pushButton_4->setEnabled(false);
	ui.pushButton_6->setEnabled(true);
	ui.pushButton_2->setEnabled(true);
	ui.pushButton_5->setEnabled(true);
	ui.openGLWidget->active_tool = Tool::CURSOR;
}
void MainForm::updateRe(double i)
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
	ui.openGLWidget->xlabel = ui.label_4;
	ui.openGLWidget->ylabel = ui.label_5;
	ui.openGLWidget->uxlabel = ui.label_6;
	ui.openGLWidget->uylabel = ui.label_7;
	ui.openGLWidget->flabel = ui.label_8;
	ui.openGLWidget->maxLabel = ui.label_9;

	QPixmap pixmap(":/FluidQt/pencil.png");
	QIcon ButtonIcon(pixmap);
	ui.pushButton_2->setText("");
	ui.pushButton_2->setBaseSize(QSize(64, 64));
	ui.pushButton_2->setIcon(ButtonIcon);
	ui.pushButton_2->setIconSize(QSize(64,64));
	ui.pushButton_2->setFixedSize(QSize(64, 64));


	QPixmap pixmap0(":/FluidQt/cursor.png");
	QIcon ButtonIcon0(pixmap0);
	ui.pushButton_4->setText("");
	ui.pushButton_4->setBaseSize(QSize(64, 64));
	ui.pushButton_4->setIcon(ButtonIcon0);
	ui.pushButton_4->setIconSize(QSize(64, 64));
	ui.pushButton_4->setFixedSize(QSize(64, 64));

	QPixmap pixmap1(":/FluidQt/circle.png");
	QIcon ButtonIcon1(pixmap1);
	ui.pushButton_5->setText("");
	ui.pushButton_5->setBaseSize(QSize(64, 64));
	ui.pushButton_5->setIcon(ButtonIcon1);
	ui.pushButton_5->setIconSize(QSize(64, 64));
	ui.pushButton_5->setFixedSize(QSize(64, 64));

	QPixmap pixmap2(":/FluidQt/eraser.png");
	QIcon ButtonIcon2(pixmap2);
	ui.pushButton_6->setText("");
	ui.pushButton_6->setBaseSize(QSize(64, 64));
	ui.pushButton_6->setIcon(ButtonIcon2);
	ui.pushButton_6->setIconSize(QSize(64, 64));
	ui.pushButton_6->setFixedSize(QSize(64, 64));

	QLinearGradient linearGrad(QPointF(100, 100), QPointF(200, 200));
	linearGrad.setColorAt(0, Qt::black);
	linearGrad.setColorAt(1, Qt::white);

	selectCursor();

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(handlePlayButton()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(resetAnimation()));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateRe(double)));
	connect(ui.doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(setRadius(double)));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(selectPencil()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(selectCursor()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(selectCircle()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(selectEraser()));
	connect(ui.doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(updateUx(double)));
}
void MainForm::updateGL(OpenglWidget* gl)
{
	gl->update();
}
MainForm::~MainForm()
{
}
