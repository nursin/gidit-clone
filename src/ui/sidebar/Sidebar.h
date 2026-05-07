#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

namespace Gidit {

class BuilderPalette;

class Sidebar : public QWidget {
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent = nullptr);

signals:
    void navItemClicked(const QString &name);

private:
    QPushButton *addNavItem(QVBoxLayout *layout, const QString &icon, const QString &label,
                            bool active = false, const QString &badge = "");
    BuilderPalette *m_palette = nullptr;
    QVector<QPushButton *> m_navItems;
};

} // namespace Gidit
