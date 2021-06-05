#include <exception>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QToolBar>
#include <QTabWidget>
#include <QProxyStyle>
#include <QStyleOptionTab>

#include "util/common.hpp"
#include "config/configuration.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "MainWindow.hpp"

namespace postrisc {

int run(int argc, char **argv)
{
    util::setup();

    // pass host-only arguments (Qt removes its params)
    QApplication app(argc, argv);

    app.setStyleSheet("QStatusBar { border: 1px;  background: rgb(200,220,200); }");

    LOG_DEBUG(LOG_EMULATOR)
        << "DPIX:" << QApplication::primaryScreen()->logicalDotsPerInchX()
        << ", DPIY:" << QApplication::primaryScreen()->logicalDotsPerInchY()
        << ", devicePixelRatio:" << QApplication::primaryScreen()->devicePixelRatio();

    // pass rest of host arguments
    Configuration configuration;
    if (!configuration.parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    TargetSystem m_TargetSystem;

    MainWindow mainWindow(m_TargetSystem);
    mainWindow.Setup(configuration);
    mainWindow.show();

    return app.exec();
}

} // namespace postrisc

int main(int argc, char **argv)
{
    try {
        return postrisc::run(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "Exception occur: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception occur" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
