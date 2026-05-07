#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"

namespace Gidit {

class StatsWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit StatsWidget(QWidget *parent = nullptr);
};

} // namespace Gidit
