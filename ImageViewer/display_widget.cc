#include <QPainter>
#include <QWheelEvent>
#include <QShortcut>

#include "display_widget.h"

const int kBasicBlockWidth = 50;

DisplayWidget::DisplayWidget(QWidget* parent) : QWidget(parent) {
    QShortcut* zoomin_sc = new QShortcut(this);
    zoomin_sc->setKey(QKeySequence(Qt::CTRL + Qt::Key_Equal));
    connect(zoomin_sc, &QShortcut::activated, std::bind(&DisplayWidget::ZoomIn, this));

    QShortcut* zoomout_sc = new QShortcut(this);
    zoomout_sc->setKey(QKeySequence(Qt::CTRL + Qt::Key_Minus));
    connect(zoomout_sc, &QShortcut::activated, std::bind(&DisplayWidget::ZoomOut, this));

    QShortcut* restore_sc = new QShortcut(this);
    restore_sc->setKey(QKeySequence(Qt::CTRL + Qt::Key_0));
    connect(restore_sc, &QShortcut::activated, std::bind(&DisplayWidget::Restore, this));
}

bool DisplayWidget::OpenImage(const QString& file_path) {
    image_path_ = file_path;
    bool result = image_.load(image_path_);
    ScaleImage();
    update();
    return result;
}

void DisplayWidget::paintEvent(QPaintEvent* evt) {
    QPainter painter(this);

    DrawBackground(&painter);
    DrawImage(&painter);
}

void DisplayWidget::wheelEvent(QWheelEvent* event) {
    QPoint degrees = event->angleDelta() / 8;
    if (degrees.y() > 0) {
        ZoomIn();
    }
    else {
        ZoomOut();
    }
}

void DisplayWidget::mousePressEvent(QMouseEvent* event) {
    mouse_begin_ = event->pos();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        offset_ += event->pos() - mouse_begin_;
        mouse_begin_ = event->pos();
        update();
    }
}

void DisplayWidget::mouseReleaseEvent(QMouseEvent* event) {
}

void DisplayWidget::DrawBackground(QPainter* painter) {
    int block_width = kBasicBlockWidth * scale_;

    int row = height() / block_width;
    if (height() % block_width != 0) {
        ++row;
    }

    int column = width() / block_width;
    if (width() % block_width != 0) {
        ++column;
    }
    if (column % 2 == 0) {
        ++column;
    }

    bool revert = false;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            QRect item_rect(j * block_width, i * block_width, block_width, block_width);
            painter->fillRect(item_rect, revert ? Qt::lightGray : Qt::darkGray);
            revert = !revert;
        }
    }
}

void DisplayWidget::DrawImage(QPainter* painter) {
    if (scaled_image_.isNull()) {
        return;
    }

    QRect image_rect(QPoint(0, 0), scaled_image_.size());
    image_rect.moveCenter(rect().center() + offset_);
    painter->drawImage(image_rect, image_);
}

void DisplayWidget::ZoomIn() {
    scale_  *= 1.1;
    ScaleImage();
    update();
}

void DisplayWidget::ZoomOut() {
    scale_ *= 0.9;
    ScaleImage();
    update();
}

void DisplayWidget::Restore() {
    scale_ = 1.0;
    ScaleImage();
    offset_ = QPoint(0, 0);
    update();
}

void DisplayWidget::ScaleImage() {
    if (image_.isNull()) {
        return;
    }

    QSize image_size = image_.size();
    if (image_size.isEmpty()) {
        return;
    }

    QImage scaled_image = image_;
    if (width() * image_size.height() > image_size.width()* height()) {
        scaled_image = scaled_image.scaledToHeight(height());
    }
    else {
        scaled_image = scaled_image.scaledToWidth(width());
    }

    image_size = scaled_image.size();
    scaled_image = scaled_image.scaled(image_size.width() * scale_, 
        image_size.height() * scale_);

    scaled_image_ = scaled_image;
}
