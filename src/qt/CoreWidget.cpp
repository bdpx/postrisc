#include <QWidget>
#include <QSettings>
#include <QList>
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QFontDatabase>

#include "util/common.hpp"
#include "emulator/core.hpp"
#include "emulator/dump/formatter_txt.hpp"
#include "emulator_system/target_system.hpp"
#include "CoreWidget.hpp"

namespace postrisc
{

CoreWidget::CoreWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    setReadOnly(true);
    document()->setPlainText("Core context here");
}

void
CoreWidget::SetInfo(Core * core)
{
    std::ostringstream stream;
    postrisc::Decoder Decoder;
    DumpFormatterTxt formatter(Decoder, stream);
    core->dump_state(formatter);
    document()->setPlainText(QString::fromStdString(stream.str()));
}

} // namespace postrisc
