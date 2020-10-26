#include "OpenglWidget.h"
#include "RenderSystem.h"
#include "LBGK.h"
#include <QTimer>
#include <QMouseEvent>

RenderSystem* renderer;
LBGK* lbgk;
Texture* texture;

OpenglWidget::OpenglWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
	active_tool = CURSOR;
	QPoint mouse = QWidget::mapFromGlobal(QCursor::pos());
	oldx = mouse.x();
	oldy = mouse.y();
	strokewidth = 2;
}
void OpenglWidget::setSolid(int x, int y, bool b)
{
	lbgk->setSolid(x, y, b);
}
void OpenglWidget::startAnimation()
{
	running ? running = false : running = true;
}
void OpenglWidget::updateRe(double i)
{
	lbgk->Re = i;
	lbgk->init();
}
void OpenglWidget::resetAnimation()
{
	lbgk->init();
}
int OpenglWidget::sgn(double d)
{
	return (0 < d) - (d < 0);
}
void OpenglWidget::drawCirlce(int x, int y, double d)
{
	oldSolid = lbgk->solid;
	int radius = d;
	for (int i = -radius; i < radius; i++) {
		for (int j = -radius; j < radius; j++) {
			if (sqrt(i * i + j * j) <= radius) {
				lbgk->setSolid(x + j, y + i, true);
			}
		}
	}

}
void OpenglWidget::drawLine(int x0, int x1, int y0, int y1, bool b)
{
	//bresenham
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;
	while (true) {
		
		for (int i = -strokewidth; i < strokewidth; i++) {
			for (int j = -strokewidth; j < strokewidth; j++) {
				lbgk->setSolid((y0) + j, x0 + i, b);
			}
		}
		
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}
void OpenglWidget::mouseMoveEvent(QMouseEvent* e)
{
	
	switch (active_tool) {
	case(PAINT):
		drawLine(oldx  / xscale, e->x() / xscale, oldy / yscale, e->y() / yscale, true);
		oldx = e->x();
		oldy = e->y();
		break;
	case(ERASER):
		drawLine(oldx / xscale, e->x() / xscale, oldy / yscale, e->y() / yscale, false);
		oldx = e->x();
		oldy = e->y();
		break;
	case(CURSOR):
		renderer->moveCamera(-(oldx - e->x())*.001,-(oldy - e->y())*.001);
		oldx = e->x();
		oldy = e->y();
		break;
	default:
		break;
	}
	
}
void OpenglWidget::setRadius(double d)
{
	strokewidth = d;
	radius = d;
}
void OpenglWidget::mousePressEvent(QMouseEvent* e)
{
	switch (active_tool) {
	case(CIRCLE):
		drawCirlce((e->y() / yscale), e->x() / xscale, radius);
		break;
	case(PAINT):
		for (int i = -strokewidth; i < strokewidth; i++) {
			for (int j = -strokewidth; j < strokewidth; j++) {
				lbgk->setSolid((e->y() / yscale) + j, e->x() / xscale + i, true);
			}
		}
	
		oldx = e->x();
		oldy = e->y();
		break;
	case(ERASER):
		for (int i = -strokewidth; i < strokewidth; i++) {
			for (int j = -strokewidth; j < strokewidth; j++) {
				lbgk->setSolid((e->y() / yscale) + j, e->x() / xscale + i, false);
			}
		}
		oldx = e->x();
		oldy = e->y();
		break;
	case(CURSOR):
		oldx = e->x();
		oldy = e->y();
		break;
	default:
		break;
	}
}
void OpenglWidget::updateUx(double d)
{
	lbgk->ux0 = d;
}
void OpenglWidget::updateUy(double d)
{
	lbgk->uy0 = d;
}
void OpenglWidget::initializeGL()
{
	running = false;
	renderer = new RenderSystem();
	lbgkwidth = 400;
	lbgkheight = 100;
	xscale =  (double)width() / lbgkwidth;
	yscale = (double)height() / lbgkheight;

	lbgk = new LBGK(lbgkwidth, lbgkheight);
	lbgk->ux0 = .1;
	vector<char> data;
	for (int i = 0; i < lbgkwidth; i++) {
		for (int j = 0; j < lbgkheight; j++) {
			data.push_back(255);
			data.push_back(120);
			data.push_back(0);
			data.push_back(255);
		}
	}
	texture = new Texture(&data[0], lbgkwidth, lbgkheight);
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(34);

	QTimer* updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
	updateTimer->start(1);
}
void OpenglWidget::wheelEvent(QWheelEvent* e)
{
	renderer->zoom(e->delta()*0.001);

}
void OpenglWidget::updateSimulation()
{
	if (running) {
		lbgk->update();
		text->setText(QString::number(lbgk->err));

		text->setText(text->toPlainText() + "\n" + QString::number(lbgk->max));
	}
	
	QPoint mouse = QWidget::mapFromGlobal(QCursor::pos());

	xlabel->setText("x: " + QString::number((int)(mouse.x() / xscale)));
	ylabel->setText("y: " + QString::number((int)(mouse.y() / yscale)));
	uxlabel->setText("ux: " + QString::number(lbgk->getUx((mouse.y() / yscale), mouse.x() / xscale)));
	uylabel->setText("uy: " + QString::number(lbgk->getUy((mouse.y() / yscale), mouse.x() / xscale)));
	flabel->setText("f[" + QString::number((int)((mouse.y() / yscale))) + "][" + QString::number((int)(mouse.x() / xscale)) + "]: " + QString::number(lbgk->getF((mouse.y() / yscale), mouse.x() / xscale, 0)));
	
}
void OpenglWidget::paintGL()
{
	maxLabel->setText(QString::number(lbgk->max));
	texture->update(lbgk->getData());
	renderer->render(texture);
}
OpenglWidget::~OpenglWidget()
{
	delete texture;
	delete renderer;
	delete lbgk;
}
