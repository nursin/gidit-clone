#pragma once
#include <QWidget>

namespace Gidit {
class BuilderPalette : public QWidget {
    Q_OBJECT
public:
    explicit BuilderPalette(QWidget *parent = nullptr);
signals:
    void widgetDragged(const QString &type);
};
} // namespace Gidit
