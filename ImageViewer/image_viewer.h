#pragma once

#include <QtWidgets/QWidget>
#include "ui_image_viewer.h"

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = Q_NULLPTR);

    bool OpenImage(const QString& path);

private:
    Ui::ImageViewerClass    ui;
};
