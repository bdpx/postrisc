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
#include "emulator/Configuration.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "doom/CliDoomInterface.hpp"
#include "MainWindow.hpp"

namespace postrisc {

int run(int argc, char **argv)
{
    util::setup();
    util::LoggerFileFinalizer loggerFileFinalizer;

    // pass rest of host arguments
    Configuration configuration;
    if (!configuration.parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    if (configuration.is_gui()) {
        // pass host-only arguments (Qt removes its params)
        QApplication app(argc, argv);

        app.setStyleSheet("QStatusBar { border: 1px;  background: rgb(200,220,200); }");

        LOG_DEBUG(LOG_EMULATOR)
            << "DPIX:" << QApplication::primaryScreen()->logicalDotsPerInchX()
            << ", DPIY:" << QApplication::primaryScreen()->logicalDotsPerInchY()
            << ", devicePixelRatio:" << QApplication::primaryScreen()->devicePixelRatio();

        TargetSystem targetSystem;

        MainWindow mainWindow(configuration, targetSystem);
        mainWindow.Setup();
        mainWindow.show();

        return app.exec();

    } else { // console only
        CliDoomInterface doom;
        return configuration.run(doom);
    }
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
