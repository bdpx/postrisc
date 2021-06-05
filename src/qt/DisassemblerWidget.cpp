#include <QScrollBar>
#include <QTextCursor>

#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "assembler/assembler.hpp"
#include "emulator/core.hpp"
#include "DisassemblerWidget.hpp"
#include "emulator/target_system.hpp"

//#include "AssemblerHighLighter.hpp"


namespace postrisc {

static const char defaultStypeSheet[] =
    "span.opc     { font-weight:bold; color:rgb(  0,  0,255); }\n"
    "span.gpr     { font-weight:bold; color:rgb(  0,128,  0); }\n"
    "span.spr     { font-weight:bold; color:rgb(  0,  0,128); }\n"
    "span.imm     { font-weight:bold; color:rgb(128,  0,  0); }\n"
    "span.mod     { font-weight:bold; color:rgb(255,184, 10); }\n"
    "span.label   { font-weight:bold; color:rgb(  0,100,100); }\n"
    "span.current { background-color:rgb(200,100,100); }\n"
    "span.error   { background-color:rgb(75,255,255); }\n"
;

DisassemblerWidget::DisassemblerWidget(TargetSystem *targetSystem, QWidget *parent)
    : QPlainTextEdit(parent)
    , m_TargetSystem(targetSystem)
    , m_IpOffset(0)
{
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    //AssemblerHighLighter * assemblerHighLighter = new AssemblerHighLighter(document());

    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // connect(verticalScrollBar(), &QScrollBar::valueChanged/*sliderMoved*/, this, &DisassemblerWidget::SetScrollValue);
    connect(verticalScrollBar(), &QScrollBar::actionTriggered, this, &DisassemblerWidget::SetScrollValue);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &DisassemblerWidget::SliderMoved);

    document()->setDefaultStyleSheet(defaultStypeSheet);
}

DisassemblerWidget::~DisassemblerWidget()
{
}

void DisassemblerWidget::onTextChanged()
{
    // Code that executes on text change here
    //LOG_FATAL(LOG_DISPLAY) << "changed";
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
    if (!core)
    {
        LOG_FATAL(LOG_EMULATOR) << "no core";
        return;
    }

    clear();

    for (int64_t offset = m_IpOffset - 4 * 16; offset < m_IpOffset + 60 * 16;  offset += 16) {
        std::ostringstream ostream;
        ostream << "<pre>";
        CStatus status = core->disassemble_bundle(ostream, offset, disasm_mode_html);
        UNREFERENCED_PARAMETER(status);
        ostream << "</pre>";
        LOG_DEBUG(LOG_EMULATOR) << "set: " << offset << ", size=" << ostream.str().size() << "\n";
        appendHtml(QString::fromStdString(ostream.str()));
    }

    moveCursor(QTextCursor::Start);
}

void
DisassemblerWidget::SetScrollValue(int action)
{
    switch (action) {
        case QAbstractSlider::SliderSingleStepAdd:
            LOG_TRACE(LOG_EMULATOR) << ": SliderSingleStepAdd";
            m_IpOffset += 1 * sizeof(CBundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderSingleStepSub:
            LOG_TRACE(LOG_EMULATOR) << ": SliderSingleStepSub";
            m_IpOffset -= 1 * sizeof(CBundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderPageStepAdd:
            LOG_TRACE(LOG_EMULATOR) << ": SliderPageStepAdd";
            m_IpOffset += 10 * sizeof(CBundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderPageStepSub:
            LOG_TRACE(LOG_EMULATOR) << ": SliderPageStepSub";
            m_IpOffset -= 10 * sizeof(CBundle);
            SetProgram();
            break;
        case QAbstractSlider::SliderToMinimum:
            LOG_TRACE(LOG_EMULATOR) << ": SliderToMinimum";
            m_IpOffset = 0;
            break;
        case QAbstractSlider::SliderToMaximum:
            LOG_TRACE(LOG_EMULATOR) << ": SliderToMaximum";
            break;
        case QAbstractSlider::SliderMove:
            LOG_TRACE(LOG_EMULATOR) << ": SliderMove: " << verticalScrollBar()->sliderPosition()
                                    << " isdown=" << verticalScrollBar()->isSliderDown();
            break;
        default:
            LOG_TRACE(LOG_EMULATOR) << ": unknown: " << action;
    }
}

void
DisassemblerWidget::SliderMoved(int value)
{
    LOG_TRACE(LOG_EMULATOR) << ": " << value;
}

} // namespace postrisc
