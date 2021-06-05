#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QList>
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QFontDatabase>
#include <QTextStream>
#include <QDebug>

#include "util/common.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "CoreWidget.hpp"

namespace postrisc
{

CoreWidget::CoreWidget(QWidget *parent)
    : QTextDocument(parent)
{
    //const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    //setFont(fixedFont);
    //setReadOnly(true);

    QFile file(":/stylesheets/styles.css");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QString style = in.readAll();
        setDefaultStyleSheet(style);
    }
}

void
CoreWidget::SetInfo(Core *core)
{
    std::ostringstream ostream;
    postrisc::Decoder Decoder;
    DumpFormatter formatter(Decoder, ostream, true);
    formatter << "<!DOCTYPE html>\n"
                 "<html>\n"
                 "<head>\n"
                 "<title>Core dump</title>\n"
                 "</head>\n"
                 "<body>\n";
    core->dump_state(formatter);
    formatter << "</body>\n</html>\n";
    setHtml(QString::fromStdString(ostream.str()));
    //moveCursor(QTextCursor::Start);
}

} // namespace postrisc
