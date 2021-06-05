#include <QScrollBar>
#include <QTextCursor>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "assembler/Assembler.hpp"
#include "emulator/Core.hpp"
#include "DisassemblerWidget.hpp"
#include "emulator_system/TargetSystem.hpp"

//#include "AssemblerHighLighter.hpp"


namespace postrisc {

static const char defaultStyleSheet[] =
    "span.opc     { font-weight:bold; color:rgb(  0,  0,255); }\n"
    "span.gpr     { font-weight:bold; color:rgb(  0,128,  0); }\n"
    "span.spr     { font-weight:bold; color:rgb(  0,  0,128); }\n"
    "span.imm     { font-weight:bold; color:rgb(128,  0,  0); }\n"
    "span.mod     { font-weight:bold; color:rgb(255,184, 10); }\n"
    "span.label   { font-weight:bold; color:rgb(  0,100,100); }\n"
    "span.current { background-color:rgb(200,100,100); }\n"
    "span.nullified { background-color:rgb(150,250,150); }\n"
    "span.error   { background-color:rgb(75,255,255); }\n"
;

DisassemblerWidget::DisassemblerWidget(TargetSystem *targetSystem, QWidget *parent)
    : QPlainTextEdit(parent)
    , m_TargetSystem(targetSystem)
{
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    //AssemblerHighLighter *assemblerHighLighter = new AssemblerHighLighter(document());

    // set tab to 8 spaces
    setTabStopDistance(QFontMetrics(fixedFont).horizontalAdvance(' ') * 8);

    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // connect(verticalScrollBar(), &QScrollBar::valueChanged/*sliderMoved*/, this, &DisassemblerWidget::SetScrollValue);
    connect(verticalScrollBar(), &QScrollBar::actionTriggered, this, &DisassemblerWidget::SetScrollValue);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &DisassemblerWidget::SliderMoved);

    document()->setDefaultStyleSheet(defaultStyleSheet);
}

DisassemblerWidget::~DisassemblerWidget()
{
}

void DisassemblerWidget::onTextChanged()
{
    // Code that executes on text change here
    //LOG_ERROR(LOG_DISPLAY) << "changed";
}

void DisassemblerWidget::onCursorPositionChanged()
{
    // Code that executes on cursor change here
    // m_label.setText(QString("Position: %1").arg(m_AddressEdit.cursorPosition()));
}

void
DisassemblerWidget::SetProgram()
{
    Core *core = m_TargetSystem->get_bootstrap_core();
    if (!core) {
        LOG_ERROR(LOG_EMULATOR) << "no core";
        return;
    }

    clear();
    std::ostringstream ostream;
    core->DisassembleDynamic(*m_TargetSystem, ostream, m_IpOffset - 4 * sizeof(Bundle), m_IpOffset + 60 * sizeof(Bundle));
    appendHtml(QString::fromStdString(ostream.str()));
    moveCursor(QTextCursor::Start);
}

void
DisassemblerWidget::SetScrollValue(int action)
{
    switch (action) {
        case QAbstractSlider::SliderSingleStepAdd:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderSingleStepAdd";
            m_IpOffset += 1 * sizeof(Bundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderSingleStepSub:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderSingleStepSub";
            m_IpOffset -= 1 * sizeof(Bundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderPageStepAdd:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderPageStepAdd";
            m_IpOffset += 10 * sizeof(Bundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderPageStepSub:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderPageStepSub";
            m_IpOffset -= 10 * sizeof(Bundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderToMinimum:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderToMinimum";
            m_IpOffset = 0;
            break;
        case QAbstractSlider::SliderToMaximum:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderToMaximum";
            break;
        case QAbstractSlider::SliderMove:
            LOG_DEBUG(LOG_EMULATOR) << ": SliderMove: " << verticalScrollBar()->sliderPosition()
                                    << " isdown=" << verticalScrollBar()->isSliderDown();
            break;
        default:
            LOG_DEBUG(LOG_EMULATOR) << ": unknown: " << action;
    }
}

void
DisassemblerWidget::SliderMoved(int value)
{
    LOG_DEBUG(LOG_EMULATOR) << ": " << value;
}

} // namespace postrisc
