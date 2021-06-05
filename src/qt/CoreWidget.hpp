#pragma once

//#include <QPlainTextEdit>
#include <QTextDocument>
#include <QSettings>
#include <QList>
#include <QString>

namespace postrisc {

class Core;

class CoreWidget : public QTextDocument {
public:
    explicit CoreWidget(QWidget *parent = 0);
    void SetInfo(Core *core);

private:
};

} // namespace postrisc
