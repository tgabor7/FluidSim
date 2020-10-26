#pragma once
#include <QOpenGLWidget>
#include "ui_OpenglWidget.h"
#include "GradientWidget.h"
#include "RenderSystem.h"
#include "Maths.h"

class GradientWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	GradientWidget(QWidget* parent = Q_NULLPTR);
	~GradientWidget();

private:
	vector3 convertToColor(float f);
	Ui::OpenglWidget ui;
	void paintGL();
	void initializeGL();
	RenderSystem* renderer;
	Texture* texture;


};

