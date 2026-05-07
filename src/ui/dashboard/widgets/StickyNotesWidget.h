#pragma once
#include "ui/dashboard/widgets/BaseWidget.h"

namespace Gidit {
class StickyNotesWidget : public BaseWidget {
    Q_OBJECT
public:
    explicit StickyNotesWidget(QWidget *parent = nullptr);
};
} // namespace Gidit
