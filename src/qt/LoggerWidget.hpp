#pragma once

#include <QPlainTextEdit>
#include <QFile>

namespace postrisc {

class LoggerWidget;

class WidgetStream : public QObject, public std::basic_streambuf<char> {
Q_OBJECT
public:
    explicit WidgetStream(std::ostream& stream, LoggerWidget *text_edit);
    ~WidgetStream();

signals:
    void appendMessage( QString text);

protected:
    std::basic_streambuf<char>::int_type overflow(int_type v) override;
    std::streamsize xsputn(const char *p, std::streamsize n) override;

private:
    std::ostream&    m_stream;
    std::streambuf  *m_old_buf;
    std::string      m_string;
    LoggerWidget    *log_window;
};


class LoggerWidget : public QPlainTextEdit {
public:
    explicit LoggerWidget(QWidget *parent = nullptr);
    ~LoggerWidget();

    void appendMessage(QString text);

private:
    QFile m_logFile;
    WidgetStream m_WidgetStream;
};

} // namespace postrisc
