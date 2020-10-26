#include "GradientWidget.h"
#include <QTimer>

GradientWidget::GradientWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
}
void GradientWidget::initializeGL()
{
	renderer = new RenderSystem();
	vector<char> data;
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 200; j++) {
			vector3 color = convertToColor((i * 200 + j)/20);
			data.push_back(color.x);
			data.push_back(color.y);
			data.push_back(color.z);
			data.push_back(255);
		}
	}
	texture = new Texture(&data[0], 50, 200);
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
vector3 GradientWidget::convertToColor(float f)
{
	vector3 color;
	if (f < 256) {
		color.z = 255 - f;
		color.y = f;
	}
	else if (f > 255) {
		color.x = f - 256;
		color.y = 255 - (f - 256);
	}
	return color;
}
void GradientWidget::paintGL()
{
	renderer->render(texture);
}
GradientWidget::~GradientWidget()
{

}