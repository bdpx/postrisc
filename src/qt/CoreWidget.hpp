#pragma once

#include <QPlainTextEdit>
#include <QSettings>
#include <QList>
#include <QString>

namespace postrisc {

class Core;

class CoreWidget : public QPlainTextEdit {
public:
    explicit CoreWidget(QWidget *parent = 0);
    void SetInfo(Core * core);

private:

private:
};

} // namespace postrisc
