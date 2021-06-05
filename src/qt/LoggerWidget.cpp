#include <QFontDatabase>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QScrollBar>

#include "util/common.hpp"
#include "LoggerWidget.hpp"

namespace postrisc
{

WidgetStream::WidgetStream(std::ostream& stream, LoggerWidget *text_edit)
    : m_stream(stream)
{
    log_window = text_edit;
    m_old_buf = stream.rdbuf();
    stream.rdbuf(this);
}


WidgetStream::~WidgetStream()
{
    // output anything that is left
    if (!m_string.empty())
        emit appendMessage(m_string.c_str());

    m_stream.rdbuf(m_old_buf);
}


std::basic_streambuf<char>::int_type
WidgetStream::overflow(int_type v)
{
    if (v == '\n') {
        emit appendMessage(m_string.c_str());
        m_string.erase(m_string.begin(), m_string.end());
    } else {
         m_string += v;
    }

    return v;
}

std::streamsize
WidgetStream::xsputn(const char *p, std::streamsize n)
{
    m_string.append(p, p + n);

    size_t pos = 0;
    while (pos != std::string::npos) {
        pos = m_string.find('\n');
        if (pos != std::string::npos) {
            std::string tmp(m_string.begin(), m_string.begin() + pos);
            emit appendMessage(tmp.c_str());
            m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
        }
    }

    return n;
}

LoggerWidget::LoggerWidget(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_WidgetStream(std::cout, this)
{
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(fixedFont);

    setMaximumBlockCount(5000);
    setReadOnly(true);

    connect(&m_WidgetStream, &WidgetStream::appendMessage, this, &LoggerWidget::appendMessage);
}

LoggerWidget::~LoggerWidget()
{
}

void LoggerWidget::appendMessage(QString text)
{
    appendPlainText(text); // Adds the message to the widget
    verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom

    //m_logFile.write(text); // Logs to file
}

} // namespace postrisc
