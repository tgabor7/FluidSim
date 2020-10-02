#include "OpenglWidget.h"
#include "RenderSystem.h"
#include "LBGK.h"
#include <QTimer>

RenderSystem* renderer;
LBGK* lbgk;
Texture* texture;

OpenglWidget::OpenglWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
}
void OpenglWidget::startAnimation()
{
	running ? running = false : running = true;
}
void OpenglWidget::updateRe(int i)
{
	lbgk->Re = i;
}
void OpenglWidget::resetAnimation()
{
	lbgk->init();
}
void OpenglWidget::initializeGL()
{
	running = false;
	renderer = new RenderSystem();
	lbgk = new LBGK(400, 100);
	vector<char> data;
	for (int i = 0; i < 400; i++) {
		for (int j = 0; j < 100; j++) {
			data.push_back(255);
			data.push_back(120);
			data.push_back(0);
			data.push_back(255);
		}
	}
	texture = new Texture(&data[0], 400, 100);
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(17);

	QTimer* updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateSimulation()));
	updateTimer->start(1);
}
void OpenglWidget::updateSimulation()
{
	if (running) {
		lbgk->update();
	}
}
void OpenglWidget::paintGL()
{
	
	texture->update(&lbgk->getData()[0]);
	renderer->render(texture);
}
OpenglWidget::~OpenglWidget()
{
	delete texture;
	delete renderer;
	delete lbgk;
}
