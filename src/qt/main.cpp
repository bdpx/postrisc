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
    Configuration configuration;

    // copy host-only arguments
    std::vector<char*> host_argv;
    int host_argc = 0;
    int guest_argc_pos = 0;
    for (int i = 0; i < argc; i++) {
        if (0 == std::strcmp(argv[i], "--")) {
            guest_argc_pos = i + 1;
            break;
        }
        host_argv.emplace_back(argv[i]);
        host_argc++;
    }

    // pass host-only arguments (Qt removes its params)
    QApplication app(host_argc, host_argv.data());

    app.setStyleSheet("QStatusBar { border: 1px;  background: rgb(200,220,200); }");

    LOG_DEBUG(LOG_EMULATOR)
        << "DPIX:" << QApplication::primaryScreen()->logicalDotsPerInchX()
        << ", DPIY:" << QApplication::primaryScreen()->logicalDotsPerInchY()
        << ", devicePixelRatio:" << QApplication::primaryScreen()->devicePixelRatio();

    // pass rest of host arguments
    int const parsed = configuration.parse(host_argc, host_argv.data());
    if (parsed < 0) {
        return EXIT_FAILURE;
    }

    Decoder Decoder;
    TargetSystem m_TargetSystem(Decoder);

    // pass guest arguments further in emulator
    int const guest_argc = argc - guest_argc_pos;
    char **guest_argv = argv + guest_argc_pos;

    ConfRunner runner(configuration, guest_argc, guest_argv);

    MainWindow mainWindow(m_TargetSystem);
    mainWindow.Setup(runner);
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
