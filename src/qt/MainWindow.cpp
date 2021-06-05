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
#include <QStatusBar>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "util/common.hpp"
#include "emulator/configuration.hpp"
#include "emulator/core.hpp"
#include "emulator/target_system.hpp"
#include "MainWindow.hpp"
#include "OpenGLWindow.hpp"

#define ENTER  LOG_FATAL(LOG_EMULATOR) << "called";

namespace postrisc {

class CustomTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                         const QSize& size, const QWidget* widget) const
  {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
    }
    return s;
  }

  void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
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
};

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
    const char * path;
    const char * name;
    const char * description;
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
MainWindow::Setup(void)
{
    QMenu * submenus[MENU_LAST];

    submenus[MENU_FILE]  = menuBar()->addMenu("&File");
    submenus[MENU_RUN]   = menuBar()->addMenu("Run");
    submenus[MENU_DEBUG] = menuBar()->addMenu("Debug");
    submenus[MENU_TOOLS] = menuBar()->addMenu("Tools");
    submenus[MENU_HELP]  = menuBar()->addMenu("Help");

    m_ToolBar = addToolBar("tool bar");
    m_ToolBar->setIconSize(QSize(ScaleDpiX(20), ScaleDpiY(20)));

    for (const auto & ainfo : actions_table) {
        QAction * newAct = new QAction(QIcon(ainfo.path), ainfo.name, this);
        newAct->setStatusTip(ainfo.description);
        newAct->setShortcut(ainfo.keySequence);
        connect(newAct, &QAction::triggered, this, ainfo.func);
        submenus[ainfo.id]->addAction(newAct);
        m_ToolBar->addAction(newAct);
    }

    QWidget *cw = new QWidget(this);
    setCentralWidget(cw);

    QTabWidget * tabWidget = new QTabWidget(cw);

    tabWidget->setIconSize(QSize(ScaleDpiX(30), ScaleDpiY(30)));
    tabWidget->setTabPosition(QTabWidget::TabPosition::West);
    tabWidget->tabBar()->setStyle(new CustomTabStyle);

    // assembler editor tab
    m_AssemblerTextEdit = new AssemblerTextEdit(this);
    tabWidget->addTab(m_AssemblerTextEdit, QIcon(":/icons/documents.svg"), "program");

    m_CoreWidget = new CoreWidget;
    tabWidget->addTab(m_CoreWidget, QIcon(":/icons/cpu.svg"), "Core");

    m_DisassemblerWidget = new DisassemblerWidget(&m_TargetSystem);
    tabWidget->addTab(m_DisassemblerWidget, QIcon(":/icons/binary-code.svg"), "Disassembler");

    m_LoggerWidget = new LoggerWidget;
    tabWidget->addTab(m_LoggerWidget, QIcon(":/icons/binary-code.svg"), "Logs");

    tabWidget->addTab(new QWidget, QIcon(":/icons/ram-memory.svg"), "Memory");
    tabWidget->addTab(new QWidget, QIcon(":/icons/ram-memory.svg"), "Devices");
    tabWidget->addTab(new QWidget, QIcon(":/icons/ram-memory.svg"), "screen");

    TriangleWindow * twindow = new TriangleWindow;
    QWidget *container = QWidget::createWindowContainer(twindow);
    tabWidget->addTab(container, QIcon(":/icons/ram-memory.svg"), "OpenGL");

    tabWidget->setCurrentWidget(m_AssemblerTextEdit);
    m_AssemblerTextEdit->setFocus();

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(tabWidget);
    cw->setLayout(layout);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ENTER
    event->accept();
    //event->ignore();
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
    try
    {
        std::istringstream istream(m_Program);
        EmulatorConfiguration configuration;
        configuration.SetDefaults();
        configuration.config_flags |= CONFIG_TIMING | CONFIG_VERBOSE;
        CStatus status = m_TargetSystem.StartEmulation(configuration, istream, std::cout);
        UNREFERENCED_PARAMETER(status);
        m_DisassemblerWidget->SetProgram();
        m_TargetSystem.activate_execution();
    }
    catch(...)
    {
    }
}

void MainWindow::Action_StartAndWait()
{
    ENTER
    m_LoggerWidget->clear();
    try {
        std::istringstream istream(m_Program);
        EmulatorConfiguration configuration;
        configuration.SetDefaults();
        CStatus status = m_TargetSystem.StartEmulation(configuration, istream, std::cout);
        UNREFERENCED_PARAMETER(status);
        Core * core = m_TargetSystem.get_bootstrap_core();
        status = core->post_interrupt(CStatus(CStatus::single_step_trap));

        m_DisassemblerWidget->SetProgram();
        status = core->execute_until_interruption();
        //m_TargetSystem.activate_execution();
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
        Core * core = m_TargetSystem.get_bootstrap_core();
        if (core) {
            CStatus status = core->post_interrupt(CStatus(CStatus::single_step_trap));
            status = core->execute_until_interruption();
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

} // namespace postrisc
