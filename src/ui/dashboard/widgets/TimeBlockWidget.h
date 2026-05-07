#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"

namespace Gidit {

struct TimeBlock { QString time; QString label; QString icon; QString type; int heightScale; };

class TimeBlockWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit TimeBlockWidget(QWidget *parent = nullptr);
private:
    QVector<TimeBlock> m_blocks;
};

} // namespace Gidit
