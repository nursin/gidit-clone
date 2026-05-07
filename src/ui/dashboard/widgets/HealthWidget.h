#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"

namespace Gidit {
class HealthWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit HealthWidget(QWidget *parent = nullptr);
};
} // namespace Gidit
