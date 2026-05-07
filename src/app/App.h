#pragma once
#include <QApplication>
#include <memory>

namespace Gidit {
class MainWindow;
class EmotionEngine;

class App {
public:
    static App &instance();
    int run(int argc, char *argv[]);
    EmotionEngine *emotionEngine() const { return m_emotionEngine.get(); }
private:
    App() = default;
    void initFonts();
    void initTheme(QApplication &app);
    std::unique_ptr<MainWindow>    m_mainWindow;
    std::unique_ptr<EmotionEngine> m_emotionEngine;
};
} // namespace Gidit
