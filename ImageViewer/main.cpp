#include "image_viewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ImageViewer w;
	w.show();
	if (argc > 1) {
		w.OpenImage(QString::fromLocal8Bit(argv[1]));
	}

	return a.exec();
}
