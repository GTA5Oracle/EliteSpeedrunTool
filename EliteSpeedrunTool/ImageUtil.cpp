#include "ImageUtil.h"
#include <QImageReader>

Q_GLOBAL_STATIC(ImageUtil, imageUtilInstance)

ImageUtil::ImageUtil(QObject* parent)
    : QObject { parent }
{
}

ImageUtil* ImageUtil::instance()
{
    return imageUtilInstance;
}

QCursor ImageUtil::cursorFromCurFile(QString curPath)
{
    QImageReader reader(curPath);
    reader.setDecideFormatFromContent(true);

    if (!reader.canRead()) {
        qWarning() << "Unable to read cursor file:" << reader.errorString();
        return QCursor();
    }

    QImage image = reader.read();
    if (image.isNull()) {
        qWarning() << "Failed to read cursor:" << reader.errorString();
        return QCursor();
    }

    QPoint hotspot = reader.text("HotSpot").isEmpty()
        ? QPoint(0, 0)
        : QPoint(reader.text("HotSpotX").toInt(), reader.text("HotSpotY").toInt());

    qInfo() << "Cursor hotspot:" << hotspot;
    return QCursor(QPixmap::fromImage(image), hotspot.x(), hotspot.y());
}
