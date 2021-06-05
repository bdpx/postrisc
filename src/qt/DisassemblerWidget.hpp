#pragma once

#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

namespace postrisc {

class TargetSystem;

class DisassemblerWidget : public QPlainTextEdit {
public:
    explicit DisassemblerWidget(TargetSystem *targetSystem, QWidget *parent = nullptr);
    ~DisassemblerWidget();
    void SetProgram();

private:
    void onTextChanged();
    void onCursorPositionChanged();
    void SetScrollValue(int value);
    void SliderMoved(int value);

private:
    TargetSystem *m_TargetSystem;
    i64           m_IpOffset = 0;
};

} // namespace postrisc
