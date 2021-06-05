#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QList>
#include <QString>

#include "AssemblerTextEdit.hpp"
#include "DisassemblerWidget.hpp"
#include "CoreWidget.hpp"
#include "LoggerWidget.hpp"

namespace postrisc {

class TargetSystem;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(TargetSystem& targetSystem, QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *) override;

    void Setup(void);
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
    QToolBar * m_ToolBar = nullptr;
    AssemblerTextEdit * m_AssemblerTextEdit = nullptr;
    DisassemblerWidget * m_DisassemblerWidget = nullptr;
    CoreWidget * m_CoreWidget = nullptr;
    LoggerWidget * m_LoggerWidget = nullptr;
    std::string m_Program;
    TargetSystem& m_TargetSystem;
};

} // namespace postrisc
