#pragma once
#include <QWidget>

class DisplayWidget : public QWidget {
public:
    DisplayWidget(QWidget* parent = nullptr);

    bool OpenImage(const QString& file_path);

protected:
    void paintEvent(QPaintEvent* evt) override;
    void wheelEvent(QWheelEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void DrawBackground(QPainter* painter);
    void DrawImage(QPainter* painter);

    void ZoomIn();
    void ZoomOut();
    void Restore();

    void ScaleImage();

private:
    float                   scale_      = 1.0;
    QPoint                  offset_;

    QString                 image_path_;
    QImage                  image_;
    QImage                  scaled_image_;

    QPoint                  mouse_begin_;
};

