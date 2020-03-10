#include <QFileDialog>
#include <QShortcut>

#include "image_viewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent) {
    ui.setupUi(this);

    QShortcut* quit_sc = new QShortcut(this);
    quit_sc->setKey(QKeySequence(Qt::CTRL + Qt::Key_Q));
    connect(quit_sc, &QShortcut::activated, []() {
        qApp->quit();
    });

    connect(ui.open_button, &QPushButton::clicked, [&]() {
        QString file_path = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
        ui.display_widget->OpenImage(file_path);
    });
}
