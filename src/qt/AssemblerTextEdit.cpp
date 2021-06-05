#include <QFontDatabase>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "util/common.hpp"
#include "assembler/Assembler.hpp"
#include "config/configuration.hpp"
#include "AssemblerTextEdit.hpp"
#include "AssemblerHighLighter.hpp"

namespace postrisc {

AssemblerTextEdit::AssemblerTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);
    AssemblerHighLighter *assemblerHighLighter = new AssemblerHighLighter(document());
    UNREFERENCED_PARAMETER(assemblerHighLighter);
}

AssemblerTextEdit::~AssemblerTextEdit()
{
}

bool AssemblerTextEdit::DoDialog(bool open, QString& fileName)
{
    QString dir = PATH_TO_SAMPLES;
    QFileDialog fd(this, open ? "Open assembler program" : "Save assembler program", dir, "assembler (*.s)");
    if (open) {
        fd.setFileMode(QFileDialog::ExistingFile);
    }
    fd.setViewMode(QFileDialog::Detail);
    //fd.setFilter("assembler (*.s)");
    fd.setDirectory(dir);
    fd.selectFile(SELECTED_SAMPLE);
    //fd.setOption(QFileDialog::DontUseNativeDialog);
    fd.setOption(QFileDialog::DontUseCustomDirectoryIcons);
    QList<QUrl> urls;
    urls << QUrl::fromLocalFile("/Users/foo/Code/qt6");
    fd.setSidebarUrls(urls);
    if (fd.exec() != QDialog::Accepted) {
        return false;
    }
    QStringList list = fd.selectedFiles();
    if (list.isEmpty()) {
        return false;
    }
    fileName = list[0];
    if (fileName.isEmpty()) {
        return false;
    }
    return true;
}

bool AssemblerTextEdit::Open(QString& fileName)
{
    if (!AssemblerTextEdit::DoDialog(true, fileName)) {
        return false;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_ERROR(LOG_DISPLAY) << "open failed";
        return false;
    }
    QByteArray byteArray = file.readAll();
    document()->setPlainText(byteArray);
    m_FilePath = fileName;
    return true;
}

bool AssemblerTextEdit::Save()
{
    return AssemblerTextEdit::SaveTo(m_FilePath);
}

bool AssemblerTextEdit::SaveAs(QString& fileName)
{
    if (!AssemblerTextEdit::DoDialog(false, fileName)) {
        return false;
    }
    const bool result = AssemblerTextEdit::SaveTo(fileName);
    if (result) {
        m_FilePath = fileName;
    }
    return result;
}

bool AssemblerTextEdit::SaveTo(const QString& fileName)
{
    QByteArray byteArray = document()->toPlainText().toUtf8();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG_ERROR(LOG_DISPLAY) << "open failed";
        return false;
    }

    file.write(byteArray);
    return true;
}

bool AssemblerTextEdit::Build(std::ostringstream& ostream)
{
    try {
        std::string string = document()->toPlainText().toStdString();

        std::istringstream istream(string);
        Decoder decoder;
        Assembler assembler(decoder);
        Configuration config;
        config.ExportDefinitions(assembler);
        assembler.Assemble(istream, ostream, Assembler::assemble_mode_obj);
    }
    catch (CompilationError& error) {
        const auto lineNum = error.GetLineNum();
        QTextCursor cursor(document());
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNum-1);
        setTextCursor(cursor);
#if 0
          QTextEdit::ExtraSelection selection;
          QColor lineColor = QColor(Qt::yellow).lighter(160);
          selection.format.setBackground(lineColor);
          selection.format.setProperty(QTextFormat::FullWidthSelection, true);
          //QTextCursor cursor(textCursor());
          cursor.setPosition(QTextCursor::StartOfBlock);
          cursor.setPosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
          QTextCharFormat fmt;
          fmt.setBackground(Qt::yellow);
          cursor.setCharFormat(fmt);
          selection.cursor = cursor;
          //QList<QTextEdit::ExtraSelection> extraSelections;
          //extraSelections.append(selection);
#endif
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(error.GetMessage()));
        msgBox.exec();
        return false;
    } catch (...) {
        QMessageBox msgBox;
        msgBox.setText("Error found");
        msgBox.exec();
        return false;
    }

    return true;
}

} // namespace postrisc
