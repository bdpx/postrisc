#include <QTextCharFormat>
#include <QBrush>
#include <QColor>
 
#include "util/common.hpp"
#include "assembler/Assembler.hpp"
#include "disassembler/Decoder.hpp"
#include "AssemblerHighLighter.hpp"

namespace postrisc {
 
AssemblerHighLighter::AssemblerHighLighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    Decoder decoder;
    m_DirectivesPCRE = QRegularExpression(QString::fromStdString(std::string("\\.") + ComputePattern(Assembler::asm_directives)));
    m_MnemonicsPCRE = QRegularExpression(QString::fromStdString(ComputePattern(decoder.asm_names)));
    m_GeneralRegistersPCRE = QRegularExpression(QString::fromStdString(ComputePattern(decoder.m_GeneralRegisters)));
    m_SpecialRegistersPCRE = QRegularExpression(QString::fromStdString(ComputePattern(decoder.m_SpecialRegisters)));
    m_LabelsPCRE = QRegularExpression("\\b[a-z|A-Z|0-9_]+:");
    m_SingleLineComment = QRegularExpression("\\;.*");
    m_MultilineCommentStartExpression = QRegularExpression("\\/\\*");
    m_MultilineCommentEndExpression = QRegularExpression("\\*\\/");
    m_QuoteStartExpression = QRegularExpression("\\\""); // any quote

    // first quote at matching position or next quote without slash
    // \G matches at the first matching position in the subject
    m_QuoteEndExpression = QRegularExpression("(\\G\\\"|[^\\\\]\\\")");

    m_FormatDirectives.setForeground(Qt::red);
    m_FormatDirectives.setFontWeight(QFont::Bold);
    m_FormatMnemonics.setForeground(Qt::blue);
    m_FormatMnemonics.setFontWeight(QFont::Bold);
    m_FormatGeneralRegisters.setForeground(Qt::darkGreen);
    m_FormatGeneralRegisters.setFontWeight(QFont::Bold);
    m_FormatSpecialRegisters.setForeground(Qt::darkBlue);
    m_FormatSpecialRegisters.setFontWeight(QFont::Bold);
    m_FormatLabels.setForeground(Qt::black);
    m_FormatLabels.setFontWeight(QFont::Bold);
    m_QuotationFormat.setForeground(Qt::darkGreen);
    m_QuotationFormat.setFontWeight(QFont::Bold);
    m_SingleLineCommentFormat.setForeground(Qt::darkGray);
    m_SingleLineCommentFormat.setFontWeight(QFont::Bold);
    m_MultiLineCommentFormat.setForeground(Qt::darkGray);
    m_MultiLineCommentFormat.setFontWeight(QFont::Bold);

}

AssemblerHighLighter::~AssemblerHighLighter()
{
}

template<typename T>
std::string
AssemblerHighLighter::ComputePattern(const keywordmap<T>& _keywordmap)
{
    std::string pattern = std::string("\\b(");
    bool first = true;
    for (const auto& word : _keywordmap) {
        if (!first) {
            pattern += "|";
        }
        pattern += word.first;
        first = false;
    }
    pattern += ")\\b";
    return pattern;
}
 
void AssemblerHighLighter::highlightBlock(const QString& text)
{
    setCurrentBlockState(None);
    const int prevBlockState = previousBlockState();
    int startIndex = 0;

    if (prevBlockState == AssemblerHighLighter::Quote) {
        QRegularExpressionMatch match = m_QuoteEndExpression.match(text);
        if (match.hasMatch()) {
            startIndex = match.capturedStart() + match.capturedLength();
            setFormat(0, startIndex, m_QuotationFormat);
        } else {
            setFormat(0, text.length(), m_QuotationFormat);
            setCurrentBlockState(Quote);
            return;
        }
    } else if (prevBlockState == AssemblerHighLighter::Comment) {
        QRegularExpressionMatch match = m_MultilineCommentEndExpression.match(text);
        if (match.hasMatch()) {
            startIndex = match.capturedStart() + match.capturedLength();
            setFormat(0, startIndex, m_MultiLineCommentFormat);
        } else {
            setFormat(0, text.length(), m_MultiLineCommentFormat);
            setCurrentBlockState(Comment);
            return;
        }
    }

    ApplyPattern(m_DirectivesPCRE, m_FormatDirectives, text, startIndex);
    ApplyPattern(m_MnemonicsPCRE, m_FormatMnemonics, text, startIndex);
    ApplyPattern(m_GeneralRegistersPCRE, m_FormatGeneralRegisters, text, startIndex);
    ApplyPattern(m_SpecialRegistersPCRE, m_FormatSpecialRegisters, text, startIndex);
    ApplyPattern(m_LabelsPCRE, m_FormatLabels, text, startIndex);

    while (true) {
        QRegularExpressionMatch matchQuote = m_QuoteStartExpression.match(text, startIndex);
        QRegularExpressionMatch matchSingleLineComment = m_SingleLineComment.match(text, startIndex);
        QRegularExpressionMatch matchMultilineComment = m_MultilineCommentStartExpression.match(text, startIndex);

        LOG_DEBUG(LOG_DISPLAY) << "startIndex=" << startIndex << " text=" << text.toStdString();

        if (matchQuote.hasMatch()) {
            if (   (!matchSingleLineComment.hasMatch() ||
                    (matchSingleLineComment.hasMatch() && matchQuote.capturedStart() < matchSingleLineComment.capturedStart()))
                && (!matchMultilineComment.hasMatch() ||
                    (matchMultilineComment.hasMatch() && matchQuote.capturedStart() < matchMultilineComment.capturedStart()))
               )
            {
                int stopIndex = matchQuote.capturedStart() + matchQuote.capturedLength();
                LOG_DEBUG(LOG_DISPLAY) << "matchStartQuote.hasMatch:"
                    << " capturedStart=" << matchQuote.capturedStart()
                    << " stopIndex=" << stopIndex;
                QRegularExpressionMatch matchEndQuote = m_QuoteEndExpression.match(text, stopIndex);
                if (matchEndQuote.hasMatch()) {
                    LOG_DEBUG(LOG_DISPLAY) << "matchEndQuote.hasMatch: capturedStart=" << matchEndQuote.capturedStart();
                    stopIndex = matchEndQuote.capturedStart() + matchEndQuote.capturedLength();
                    setFormat(matchQuote.capturedStart(), stopIndex - matchQuote.capturedStart(), m_QuotationFormat);
                    startIndex = stopIndex;
                    continue;
                } else {
                    // no quotation-end in current line
                    setFormat(matchQuote.capturedStart(), text.length() - matchQuote.capturedStart(), m_QuotationFormat);
                    setCurrentBlockState(Quote);
                    LOG_DEBUG(LOG_DISPLAY) << "!matchEndQuote.hasMatch";
                    return;
                }
            } else {
                LOG_DEBUG(LOG_DISPLAY) << "matchQuote but not: " << startIndex;
            }
        }

        if (matchMultilineComment.hasMatch()) {
            if (   (!matchSingleLineComment.hasMatch() ||
                    (matchSingleLineComment.hasMatch() && matchMultilineComment.capturedStart() < matchSingleLineComment.capturedStart()))
                && (!matchQuote.hasMatch() ||
                    (matchQuote.hasMatch() && matchMultilineComment.capturedStart() < matchQuote.capturedStart()))
               )
            {
                int stopIndex = matchMultilineComment.capturedStart() + matchMultilineComment.capturedLength();
                QRegularExpressionMatch matchEndComment = m_MultilineCommentEndExpression.match(text, stopIndex);
                if (matchEndComment.hasMatch()) {
                    LOG_DEBUG(LOG_DISPLAY) << "matchMultilineComment.hasMatch: capturedStart=" << matchEndComment.capturedStart();
                    stopIndex = matchEndComment.capturedStart() + matchEndComment.capturedLength();
                    setFormat(matchMultilineComment.capturedStart(), stopIndex - matchMultilineComment.capturedStart(), m_MultiLineCommentFormat);
                    startIndex = stopIndex;
                    continue;
                } else {
                    LOG_DEBUG(LOG_DISPLAY) << "!matchMultilineComment.hasMatch";
                    setFormat(matchMultilineComment.capturedStart(), text.length() - matchMultilineComment.capturedStart(), m_MultiLineCommentFormat);
                    setCurrentBlockState(Comment);
                    return;
                }
            } else {
                LOG_DEBUG(LOG_DISPLAY) << "matchMultilineComment but not: " << startIndex;
            }
        }

        if (matchSingleLineComment.hasMatch()) {
            if ((!matchQuote.hasMatch() ||
                 (matchQuote.hasMatch() && matchSingleLineComment.capturedStart() < matchQuote.capturedStart()))
                && (!matchMultilineComment.hasMatch() ||
                    (matchMultilineComment.hasMatch() && matchSingleLineComment.capturedStart() < matchMultilineComment.capturedStart()))
               )
            {
                LOG_DEBUG(LOG_DISPLAY) << "matchSingleLineComment.hasMatch: capturedStart=" << matchSingleLineComment.capturedStart();
                setFormat(matchSingleLineComment.capturedStart(), text.length() - matchSingleLineComment.capturedStart(), m_SingleLineCommentFormat);
                return;
            } else {
                LOG_DEBUG(LOG_DISPLAY) << "matchSingleLineComment but not: " << startIndex;
            }
        }

        LOG_DEBUG(LOG_DISPLAY) << "break: " << startIndex;
        break;
    }
}

void
AssemblerHighLighter::ApplyPattern(const QRegularExpression& re, const QTextCharFormat& format, const QString& text, int startIndex)
{
    for (QRegularExpressionMatchIterator i = re.globalMatch(text, startIndex);  i.hasNext(); /**/)
    {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            LOG_DEBUG(LOG_DISPLAY) << "captured: start=" << match.capturedStart()
                                   << " length=" << match.capturedLength();
            setFormat(match.capturedStart(), match.capturedLength(), format);
        }
    }
}

} // namespace postrisc
