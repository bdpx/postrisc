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
#include <QDesktopWidget>

#include "util/common.hpp"
#include "emulator/target_system.hpp"
#include "MainWindow.hpp"

int main(int argc, char **argv)
{
    postrisc::util::setup();
    QApplication app(argc, argv);

    LOG_TRACE(LOG_EMULATOR)
        << "DPIX:" << app.desktop()->logicalDpiX()
        << ", DPIY:" << app.desktop()->logicalDpiY()
        << ", devicePixelRatio:" << app.desktop()->devicePixelRatio();

    app.setStyleSheet( "QStatusBar { border: 1px;  background: rgb(200,220,200); }" ) ;

    postrisc::Decoder decoder;
    postrisc::TargetSystem m_TargetSystem(decoder);

    postrisc::MainWindow mainWindow(m_TargetSystem);
    mainWindow.Setup();
    mainWindow.show();

    return app.exec();
}
