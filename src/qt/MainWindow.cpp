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
#include <QStatusBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include "util/common.hpp"
#include "config/configuration.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "config/configuration.hpp"
#include "doom/doomkeys.hpp"

#include "MainWindow.hpp"
#include "OpenGLWindow.hpp"
#include "FrameBufferWindow.hpp"

#define ENTER  LOG_DEBUG(LOG_EMULATOR) << "called";

namespace postrisc {

QSize CustomTabStyle::sizeFromContents(ContentsType type, const QStyleOption* option,
                         const QSize& size, const QWidget* widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
    }
    return s;
}


void CustomTabStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (element == CE_TabBarTabLabel) {
      if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
        QStyleOptionTab opt(*tab);
        opt.shape = QTabBar::RoundedNorth;
        QProxyStyle::drawControl(element, &opt, painter, widget);
        return;
      }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

MainWindow::MainWindow(TargetSystem& targetSystem, QWidget *parent)
    : QMainWindow(parent)
    , m_TargetSystem(targetSystem)
{
    resize(ScaleDpiX(1000), ScaleDpiY(640));
    setWindowTitle("Postrisc Qt engine");

    statusBar()->showMessage("start");

    // Set style parameters for docking widgets
    //
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

MainWindow::~MainWindow(void)
{
    delete m_updater;
    delete m_EmulatorThread;
    delete m_twindow;
}

enum {
   MENU_FILE,
   MENU_RUN,
   MENU_DEBUG,
   MENU_TOOLS,
   MENU_HELP,
   MENU_LAST
};

static const struct _action_info
{
    int id;
    QKeySequence keySequence;
    void (MainWindow::*func)(void);
    const char *path;
    const char *name;
    const char *description;
} actions_table[] = {
    { MENU_FILE,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/cpu.svg",           "Cpu",         "cpu long description" },
    { MENU_FILE,     QKeySequence(Qt::Key_O | Qt::CTRL),        &MainWindow::Action_OpenProgram,     ":/icons/loadfile.svg",      "Load Asm Program",   "Load Assembler Program" },
    { MENU_FILE,     QKeySequence(Qt::Key_S | Qt::CTRL),        &MainWindow::Action_SaveProgram,     ":/icons/save.svg",          "Save",        "Save long description" },
    { MENU_FILE,     QKeySequence(Qt::Key_X | Qt::CTRL),        &MainWindow::Action_SaveAsProgram,   ":/icons/saveas.svg",        "Save As...",  "Save as long description" },
    { MENU_FILE,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/delete.svg",        "Delete",      "Delete long description" },
    { MENU_FILE,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/documents.svg",     "Documents",   "Docs long description" },
    { MENU_FILE,     QKeySequence(),                            &MainWindow::Action_Exit,            ":/icons/exit.svg",          "Exit",        "Exit application" },
    { MENU_RUN,      QKeySequence(Qt::Key_F10),                 &MainWindow::Action_Build,           ":/icons/binary-code.svg",   "Build",       "Build assembler program" },
    { MENU_RUN,      QKeySequence(Qt::Key_F9),                  &MainWindow::Action_Start,           ":/icons/run.svg",           "Start",       "Start virtual machine" },
    { MENU_RUN,      QKeySequence(Qt::Key_F8),                  &MainWindow::Action_StartAndWait,    ":/icons/run.svg",           "Start and Wait",  "Start virtual machine and debug" },
    { MENU_RUN,      QKeySequence(Qt::Key_F5),                  &MainWindow::Action_Step,            ":/icons/step-clock.svg",    "Step clock",  "Step clock" },
    { MENU_RUN,      QKeySequence(Qt::Key_E | Qt::CTRL),        &MainWindow::Action_Step,            ":/icons/step.svg",          "Step",        "Step" },
    { MENU_RUN,      QKeySequence(),                            &MainWindow::Action_WaitFinish,      ":/icons/reverse.svg",       "WaitFinish",  "Wait finish virtual machine" },
    { MENU_RUN,      QKeySequence(Qt::Key_C | Qt::CTRL),        &MainWindow::Action_Cancel,          ":/icons/cancel.svg",        "Cancel",      "Cancel execution" },
    { MENU_RUN,      QKeySequence(),                            &MainWindow::Action_Reset,           ":/icons/reset.svg",         "Reset",       "Reset machine" },
    { MENU_DEBUG,    QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/graph.svg",         "Graph",       "Graph long description" },
    { MENU_DEBUG,    QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/server.svg",        "Server",      "Server long description" },
    { MENU_DEBUG,    QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/ram-memory.svg",    "Raw memory",  "Memory long description" },
    { MENU_DEBUG,    QKeySequence(Qt::Key_E | Qt::CTRL),        &MainWindow::Action_Run,             ":/icons/expand.svg",        "Expand",      "Expand long description" },
    { MENU_DEBUG,    QKeySequence(Qt::Key_Plus | Qt::CTRL),     &MainWindow::Action_Run,             ":/icons/zoom-in.svg",       "Zoom in",     "Zoom in long description" },
    { MENU_DEBUG,    QKeySequence(Qt::Key_Minus | Qt::CTRL),    &MainWindow::Action_Run,             ":/icons/zoom-out.svg",      "Zoom out",    "Zoom out long description" },
    { MENU_DEBUG,    QKeySequence(Qt::Key_H | Qt::CTRL),        &MainWindow::Action_Run,             ":/icons/spreadsheet.svg",   "Spreadsheet", "Spreadsheet long description" },
    { MENU_TOOLS,    QKeySequence(Qt::Key_F | Qt::CTRL),        &MainWindow::Action_Run,             ":/icons/file.svg",          "File",        "File" },
    { MENU_TOOLS,    QKeySequence(Qt::Key_N | Qt::CTRL),        &MainWindow::Action_Stop,            ":/icons/stop-clock.svg",    "Stop",        "Stop executing" },
    { MENU_TOOLS,    QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/tag.svg",           "Tag",         "Tag long description" },
    { MENU_HELP,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/notepad.svg",       "test",        "test long" },
    { MENU_HELP,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/plus.svg",          "Plus",        "Plus long" },
    { MENU_HELP,     QKeySequence(),                            &MainWindow::Action_Run,             ":/icons/info.svg",          "Info",        "Info long description" },
    { MENU_HELP,     QKeySequence(),                            &MainWindow::Action_About,           ":/icons/logo.svg",          "About",       "About application" },
};

void
MainWindow::Setup(ConfRunner& runner)
{
    QMenu *submenus[MENU_LAST];

    submenus[MENU_FILE]  = menuBar()->addMenu("&File");
    submenus[MENU_RUN]   = menuBar()->addMenu("Run");
    submenus[MENU_DEBUG] = menuBar()->addMenu("Debug");
    submenus[MENU_TOOLS] = menuBar()->addMenu("Tools");
    submenus[MENU_HELP]  = menuBar()->addMenu("Help");

    m_ToolBar = addToolBar("tool bar");
    m_ToolBar->setIconSize(QSize(ScaleDpiX(20), ScaleDpiY(20)));

    for (const auto& ainfo : actions_table) {
        QAction *newAct = new QAction(QIcon(ainfo.path), ainfo.name, this);
        newAct->setStatusTip(ainfo.description);
        newAct->setShortcut(ainfo.keySequence);
        connect(newAct, &QAction::triggered, this, ainfo.func);
        submenus[ainfo.id]->addAction(newAct);
        m_ToolBar->addAction(newAct);
    }

    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);

    QTabWidget *tabWidget = new QTabWidget(cw);

    tabWidget->setIconSize(QSize(ScaleDpiX(30), ScaleDpiY(30)));
    tabWidget->setTabPosition(QTabWidget::TabPosition::West);
    tabWidget->tabBar()->setStyle(&m_CustomTabStyle);

    // assembler editor tab
    m_AssemblerTextEdit = new AssemblerTextEdit(this);
    tabWidget->addTab(m_AssemblerTextEdit, QIcon(":/icons/documents.svg"), "program");

    m_CoreWidget = new CoreWidget(this);

    auto *textEdit = new QTextEdit(this);
    textEdit->setDocument(m_CoreWidget);
    tabWidget->addTab(textEdit, QIcon(":/icons/cpu.svg"), "Core");

    m_DisassemblerWidget = new DisassemblerWidget(&m_TargetSystem);
    tabWidget->addTab(m_DisassemblerWidget, QIcon(":/icons/binary-code.svg"), "Disassembler");

    m_LoggerWidget = new LoggerWidget(this);
    tabWidget->addTab(m_LoggerWidget, QIcon(":/icons/binary-code.svg"), "Logs");

    FrameBufferWindow *fbwindow = new FrameBufferWindow(m_TargetSystem, this);
    tabWidget->addTab(fbwindow, QIcon(":/icons/spreadsheet.svg"), "Screen");

    //QTimer *_timer_screen = new QTimer(this);
    //connect(_timer_screen, &QTimer::timeout, fbwindow, &FrameBufferWindow::updateGbScreen);
    //_timer_screen->start(1000/50); // 50fps

    m_twindow = new TriangleWindow;
    QWidget *container = QWidget::createWindowContainer(m_twindow);
    tabWidget->addTab(container, QIcon(":/icons/ram-memory.svg"), "OpenGL");

    tabWidget->addTab(new QWidget, QIcon(":/icons/ram-memory.svg"), "Memory");
    tabWidget->addTab(new QWidget, QIcon(":/icons/ram-memory.svg"), "Devices");

    //tabWidget->setCurrentWidget(m_AssemblerTextEdit);
    //m_AssemblerTextEdit->setFocus();
    tabWidget->setCurrentWidget(fbwindow);
    fbwindow->setFocus();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tabWidget);
    cw->setLayout(layout);

    m_updater = new GUIUpdater(runner);
    m_EmulatorThread = new QThread(this);

    connect(m_EmulatorThread, &QThread::started, m_updater, &GUIUpdater::process, Qt::DirectConnection);
    //connect(m_EmulatorThread, &QThread::finished, m_EmulatorThread, &QThread::deleteLater);
    connect(m_updater, &GUIUpdater::finished, m_EmulatorThread, &QThread::quit, Qt::DirectConnection);
    //connect(m_updater, &GUIUpdater::finished, m_updater, &GUIUpdater::deleteLater);
    connect(m_updater, &GUIUpdater::applyImage, fbwindow, &FrameBufferWindow::applyImage, Qt::QueuedConnection);

    m_updater->moveToThread(m_EmulatorThread);
    //starting thread
    m_EmulatorThread->start();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ENTER
    assert(m_updater);
    m_updater->stopped = true;
    LOG_DEBUG(LOG_DISPLAY) << "set stopped";
    m_EmulatorThread->wait(ULONG_MAX);
    LOG_DEBUG(LOG_DISPLAY) << "waited" << std::endl;
    event->accept();
    LOG_DEBUG(LOG_DISPLAY) << "closeEvent";
}

void MainWindow::Action_Build()
{
    ENTER
    std::ostringstream stream;
    if (m_AssemblerTextEdit->Build(stream)) {
        m_Program = stream.str();

        QString str;
        QTextStream(&str) << "Compilation success: image size= " << stream.str().size() << " bytes";
        statusBar()->showMessage(str);
        //QMessageBox msgBox;
        //msgBox.setText(str);
        //msgBox.exec();
    }
}

void MainWindow::Action_Run()
{
    ENTER
    static int count = 0;

    std::ostringstream stream;
    stream << "Action_Run: " << count;
    ++count;
    statusBar()->showMessage(stream.str().c_str());
}

void MainWindow::Action_Cancel()
{
    ENTER
}

void MainWindow::Action_Start()
{
    ENTER
    m_LoggerWidget->clear();
    try {
        std::istringstream istream(m_Program);
        Configuration configuration;
        configuration.SetDefaults();
        configuration.add_config_flags(CONFIG_TIMING | CONFIG_VERBOSE);
        Result result = m_TargetSystem.StartEmulation(configuration, istream, std::cout);
        UNREFERENCED_PARAMETER(result);
        m_DisassemblerWidget->SetProgram();
        m_TargetSystem.activate_execution();
    }
    catch(...) {
    }
}

void MainWindow::Action_StartAndWait()
{
    ENTER
    m_LoggerWidget->clear();
    try {
        std::istringstream istream(m_Program);
        Configuration configuration;
        configuration.SetDefaults();
        Result result = m_TargetSystem.StartEmulation(configuration, istream, std::cout);
        UNREFERENCED_PARAMETER(result);
        Core *core = m_TargetSystem.get_bootstrap_core();
        m_DisassemblerWidget->SetProgram();
        m_CoreWidget->SetInfo(core);
    } catch(...) {
    }
}

void MainWindow::Action_WaitFinish()
{
     ENTER
     m_TargetSystem.WaitFinishEmulation(std::cout);
}


void MainWindow::Action_Stop()
{
    ENTER
}

void MainWindow::Action_Step()
{
    try {
        // ENTER
        Core *core = m_TargetSystem.get_bootstrap_core();
        if (core) {
            Result result = core->post_interrupt(Result::single_step_trap);
            result = core->execute_until_interruption(m_TargetSystem);
            m_DisassemblerWidget->SetProgram();
            m_CoreWidget->SetInfo(core);
        }
    } catch(...) {
        std::cerr << "Action_Step catch" << std::endl;
    }
}

void MainWindow::Action_Exit()
{
    ENTER
    close();
}

void MainWindow::Action_OpenProgram()
{
    ENTER
    QString fileName;
    if (m_AssemblerTextEdit->Open(fileName)) {
        statusBar()->showMessage(fileName);
        setWindowTitle("Postrisc Qt engine: " + fileName);
    }
}

void MainWindow::Action_SaveProgram()
{
    ENTER
    m_AssemblerTextEdit->Save();
}

void MainWindow::Action_SaveAsProgram()
{
    ENTER
    QString fileName;
    if (m_AssemblerTextEdit->SaveAs(fileName)) {
        statusBar()->showMessage(fileName);
        setWindowTitle("Postrisc Qt engine: " + fileName);
    }
}

void MainWindow::Action_About()
{
    ENTER
}

void MainWindow::Action_Reset()
{
    ENTER
}

/***************************************************************************
* QT doom interface
***************************************************************************/

int QtDoomInterface::Init(unsigned resx, unsigned resy)
{
    LOG_DEBUG(LOG_DOOM) << "QT: Init: w=" << resx << " h=" << resy;
    return 0;
}

int QtDoomInterface::DrawFrame(const u8 *screenbuffer, unsigned resx, unsigned resy, const DoomColor *colormap)
{
    assert(m_updater);
    if (m_updater->stopped)
        return -1;

    LOG_DEBUG(LOG_DOOM) << "QT: Draw: w=" << resx << " h=" << resy;

    QImage image(screenbuffer, resx, resy, resx * sizeof(u8), QImage::Format_Indexed8);
    image.setColorCount(256);
    for (int i = 0; i < 256; i++) {
        const DoomColor& c = colormap[i];
        image.setColor(i, qRgb(c.red, c.green, c.blue));
    }

#if 0
    if (getFramesNum() % 1000 == 0) {
        QString path("doom-");
        path += std::to_string(getFramesNum()).c_str();
        path += QString(".png");
        image.save(path, "png");
    }
#endif

    m_updater->SendApplyImage(image);
    return 0;
}

int QtDoomInterface::SetWindowTitle(char const *title)
{
    LOG_DEBUG(LOG_DOOM) << "QT: Title: " << title;
    return 0;
}

static u8 convertToDoomKey(Qt::Key key)
{
    u8 res = 0;
    switch (key) {
    case Qt::Key_Return:   res = KEY_ENTER;       break;
    case Qt::Key_Escape:   res = KEY_ESCAPE;      break;
    case Qt::Key_Left:     res = KEY_LEFTARROW;   break;
    case Qt::Key_Right:    res = KEY_RIGHTARROW;  break;
    case Qt::Key_Up:       res = KEY_UPARROW;     break;
    case Qt::Key_Down:     res = KEY_DOWNARROW;   break;
    case Qt::Key_Control:  res = KEY_FIRE;        break;
    case Qt::Key_Space:    res = KEY_USE;         break;
    case Qt::Key_Shift:    res = KEY_RSHIFT;      break;
    case Qt::Key_Alt:      res = KEY_LALT;        break;
    case Qt::Key_F2:       res = KEY_F2;          break;
    case Qt::Key_F3:       res = KEY_F3;          break;
    case Qt::Key_F4:       res = KEY_F4;          break;
    case Qt::Key_F5:       res = KEY_F5;          break;
    case Qt::Key_F6:       res = KEY_F6;          break;
    case Qt::Key_F7:       res = KEY_F7;          break;
    case Qt::Key_F8:       res = KEY_F8;          break;
    case Qt::Key_F9:       res = KEY_F9;          break;
    case Qt::Key_F10:      res = KEY_F10;         break;
    case Qt::Key_F11:      res = KEY_F11;         break;
    case Qt::Key_Equal:
    case Qt::Key_Plus:     res = KEY_EQUALS;      break;
    case Qt::Key_Minus:    res = KEY_MINUS;       break;
    default:
        res = std::tolower((char)key);
        break;
    }
    return res;
}

int QtDoomInterface::GetKey(int *pressed, u8 *doomKey)
{
    bool ispressed;
    Qt::Key keyCode;
    if (KeyRing::s_KeyRing.GetKey(ispressed, keyCode)) {
        *pressed = ispressed;
        *doomKey = convertToDoomKey(keyCode);
        return 1;
    }
    return 0;
}

int ConfRunner::run(DoomInterface& doom)
{
    return conf.do_run(doom, argc, argv);
}

} // namespace postrisc
