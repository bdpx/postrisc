#pragma once
 
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextDocument>
#include <QTextCharFormat>

namespace postrisc {
 
class AssemblerHighLighter : public QSyntaxHighlighter {
public:
    explicit AssemblerHighLighter(QTextDocument *parent = 0);
    ~AssemblerHighLighter();
 
protected:
    void highlightBlock(const QString &text) override;

private:
    void ApplyPattern(const QRegularExpression& re, const QTextCharFormat& format, const QString& text, int startIndex);
    template<typename T> std::string ComputePattern(const keywordmap<T>& keywordmap);

private:
    enum States {
        FirstLine = -1,    // Qt special value for first line
        None      = 0,     // no special meaning
        Comment   = 1,     // comment
        Quote     = 2,     // quote
    };
 
    QRegularExpression m_DirectivesPCRE;                    // . list of mnemonics
    QRegularExpression m_MnemonicsPCRE;                     // list of mnemonics
    QRegularExpression m_GeneralRegistersPCRE;              // list of regs
    QRegularExpression m_SpecialRegistersPCRE;              // list of regs
    QRegularExpression m_LabelsPCRE;                        // word:
    QRegularExpression m_SingleLineComment;                 // ;
    QRegularExpression m_MultilineCommentStartExpression;   // /*
    QRegularExpression m_MultilineCommentEndExpression;     // */
    QRegularExpression m_QuoteStartExpression;              // "
    QRegularExpression m_QuoteEndExpression;                // " but not \"

    QTextCharFormat m_SingleLineCommentFormat;
    QTextCharFormat m_MultiLineCommentFormat;
    QTextCharFormat m_QuotationFormat;
    QTextCharFormat m_FormatDirectives;
    QTextCharFormat m_FormatMnemonics;
    QTextCharFormat m_FormatGeneralRegisters;
    QTextCharFormat m_FormatSpecialRegisters;
    QTextCharFormat m_FormatLabels;
};
 
} // namespace postrisc
