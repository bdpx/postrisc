#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QList>
#include <QString>
#include <QThread>
#include <QProxyStyle>

#include "AssemblerTextEdit.hpp"
#include "DisassemblerWidget.hpp"
#include "CoreWidget.hpp"
#include "LoggerWidget.hpp"

#include "util/logger.hpp"
#include "doom/DoomInterface.hpp"

namespace postrisc {

class TargetSystem;
class Configuration;
class TriangleWindow;

class ConfRunner {
public:
    ConfRunner(Configuration& conf_, int argc_, char **argv_)
        : conf(conf_), argc(argc_), argv(argv_) {}

    int run(DoomInterface& doom);

private:
    Configuration& conf;
    int argc;
    char **argv;
};

class GUIUpdater;

class QtDoomInterface : public DoomInterface {
public:
    QtDoomInterface(GUIUpdater *updater) : m_updater(updater) {}
    ~QtDoomInterface(void) override {};
    int Init(unsigned resx, unsigned resy) override;
    int DrawFrame(const u8 *screenbuffer, unsigned resx, unsigned resy, const DoomColor *colormap) override;
    int GetKey(int* pressed, unsigned char* Key) override;
    int SetWindowTitle(char const *title) override;
private:
    GUIUpdater *m_updater;
};

/***************************************************************************
* Trigger for redraw from emulator thread to gui thread
***************************************************************************/
class GUIUpdater : public QObject {
    Q_OBJECT

public:
    explicit GUIUpdater(ConfRunner& runner_, QObject *parent = 0)
        : QObject(parent), runner(runner_), doom(this) {}

public:
    void SendApplyImage(const QImage& image) { emit applyImage(image); }

public slots:
    void process(void) {
        runner.run(doom);
        LOG_DEBUG(LOG_DISPLAY) << "runner stopped";
        emit finished();
        LOG_DEBUG(LOG_DISPLAY) << "runner finished";
    }

signals:
    void applyImage(const QImage& image);
    void finished(void);
    void error(QString err);

public:
    bool stopped = false;

private:
    ConfRunner& runner;
    QtDoomInterface doom;
};

class CustomTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                         const QSize& size, const QWidget* widget) const;
  void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};

/***************************************************************************
* MainWindow
***************************************************************************/
class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(TargetSystem& targetSystem, QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;

    void Setup(ConfRunner& runner);
    int ScaleDpiX(int s) { return s * logicalDpiX() / 96.0; }
    int ScaleDpiY(int s) { return s * logicalDpiY() / 96.0; }

public:
    void Action_About();
    void Action_Build();
    void Action_Cancel();
    void Action_Compile();
    void Action_Exit();
    void Action_OpenProgram();
    void Action_SaveProgram();
    void Action_SaveAsProgram();
    void Action_Reset();
    void Action_Run();
    void Action_Start();
    void Action_StartAndWait();
    void Action_Step();
    void Action_Stop();
    void Action_WaitFinish();

private:
    QToolBar *m_ToolBar = nullptr;
    AssemblerTextEdit *m_AssemblerTextEdit = nullptr;
    DisassemblerWidget *m_DisassemblerWidget = nullptr;
    CoreWidget *m_CoreWidget = nullptr;
    LoggerWidget *m_LoggerWidget = nullptr;
    QThread *m_EmulatorThread = nullptr;
    GUIUpdater *m_updater = nullptr;
    TriangleWindow *m_twindow;

    CustomTabStyle m_CustomTabStyle;
    std::string m_Program;
    TargetSystem& m_TargetSystem;
};

} // namespace postrisc
