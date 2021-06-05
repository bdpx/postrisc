#pragma once

#include <QPlainTextEdit>

namespace postrisc {

class AssemblerTextEdit : public QPlainTextEdit {
public:
    explicit AssemblerTextEdit(QWidget *parent);
    ~AssemblerTextEdit();

    bool Open(QString& fileName);
    bool Save();
    bool SaveAs(QString& fileName);
    bool Build(std::ostringstream& ostream);

private:
    bool DoDialog(bool open, QString& fileName);
    bool SaveTo(const QString& fileName);

private:
    QString m_FilePath;
};

} // namespace postrisc
