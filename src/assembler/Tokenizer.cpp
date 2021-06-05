#include <stdexcept>   // for runtime_error
#include <sstream>     // for ostringstream

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Assembler.hpp"
#include "Tokenizer.hpp"

namespace postrisc {

static_assert(std::numeric_limits<std::streamoff>::max() == std::numeric_limits<i64>::max());

class FormatterInterface {
public:
    virtual ~FormatterInterface() {};
    virtual void echo_char(char chr) = 0;
    virtual void echo_comment_start(void) = 0;
    virtual void echo_comment_stop(void) = 0;
    virtual void echo_keyword(const char *name) = 0;
    virtual void echo_ident_new(const std::string& name) = 0;
    virtual void echo_ident_old(const std::string& name) = 0;
    virtual void echo_hexadecimal(const char *name) = 0;
    virtual void echo_octal(const char *name) = 0;
    virtual void echo_decimal(const char *name) = 0;
    virtual void echo_real(const char *name) = 0;
    virtual void echo_string(const char *name) = 0;
    virtual void echo_operator(EOperatorKind name) = 0;
    virtual void echo_character(char name) = 0;
    virtual void echo_character(const char *name) = 0;
};

/************************************************************************
* void output class for tokenizer
*************************************************************************/
class TxtFormatter : public FormatterInterface {
public:
    TxtFormatter(void) {}
    ~TxtFormatter(void) override {}

    void echo_char(char chr) override                 { LOG_DEBUG(LOG_TOKENIZER) << chr; }
    void echo_comment_start(void) override            { LOG_DEBUG(LOG_TOKENIZER) << "comment"; }
    void echo_comment_stop(void) override             { LOG_DEBUG(LOG_TOKENIZER) << "end"; }
    void echo_keyword(const char *name) override      { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_ident_new(const std::string& name) override { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_ident_old(const std::string& name) override { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_hexadecimal(const char *name) override  { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_octal(const char *name) override        { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_decimal(const char *name) override      { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_real(const char *name) override         { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_string(const char *name) override       { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_operator(EOperatorKind name) override   { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_character(char name) override           { LOG_DEBUG(LOG_TOKENIZER) << name; }
    void echo_character(const char *name) override    { LOG_DEBUG(LOG_TOKENIZER) << name; }
};

/***************************************************************************
* this function load data from any stream (don't used file-specific)
***************************************************************************/
void Scanner::read(std::istream& input)
{
    m_Source.clear();
    m_Source.reserve(64 * 1024);
    char chr;
    while (input.get(chr)) {
        if (m_Source.size() >= 64 * 1024 * 1024) {
            LOG_ABORT << "too long scanned input stream";
        }
        m_Source.push_back(chr);
    }
    LOG_DEBUG(LOG_PARSER) << "file size = " << m_Source.size();
    // add newline for possible last line without newline
    if (m_Source.empty() || m_Source.back() != '\n') {
        m_Source.push_back('\n');
    }
}

Tokenizer::~Tokenizer()
{
    // to not inline
}

void Tokenizer::InitPass()
{
    m_token_pos = 0;
    m_linenum   = 0;
}

ParserToken Tokenizer::GetCurrentToken(unsigned offset) const
{
    return m_TokenStorage[m_token_pos + offset];
}

void Tokenizer::PopToken(void)
{
    LOG_DEBUG(LOG_PARSER | LOG_EVALUATE) << "pop token: " << m_TokenStorage[m_token_pos];
    ++m_token_pos;
}

void Tokenizer::PutBackToken(void)
{
    LOG_DEBUG(LOG_PARSER | LOG_EVALUATE) << "put back token: " << m_TokenStorage[m_token_pos];
    --m_token_pos;
}

ParserToken Tokenizer::GetToken(void)
{
    ParserToken token = m_TokenStorage[m_token_pos];
    LOG_DEBUG(LOG_PARSER | LOG_EVALUATE) << "get token: " << token << " " << fmt::dec(token.GetType());
    ++m_token_pos;
    return token;
}

/****************************************************************************
*
***************************************************************************/
bool Tokenizer::IsNextRowExist(void)
{
    while (m_TokenStorage[m_token_pos].GetType() == token_eol) {
        m_linenum++;
        m_token_pos++;
    }

    if (m_TokenStorage[m_token_pos] == ParserToken(token_eof, 0))
       return false;

    return true;
}

/***********************************************************************
* error/warning reporting
***********************************************************************/
void Tokenizer::report_error(std::ostringstream& out) const
{
    out << fmt::endl << "error in line " << m_linenum << ":";
    out << fmt::endl << "source:";

    size_t tokennum = m_token_pos - 1;
    while (m_TokenStorage[tokennum].GetType() != token_eol) {
        tokennum--;
    }

    LOG_DEBUG(LOG_EVALUATE)
        << "tokennum=" << tokennum
        << " token=" << TokenPrinter(*this, m_TokenStorage[tokennum]);

    size_t pos = m_TokenStorage[tokennum].GetData();
    LOG_DEBUG(LOG_EVALUATE) << "pos = " << pos;

    while (m_Scanner[pos] != '\n')
         out << m_Scanner[pos++];
    out << std::endl;

    std::cerr << out.str();

    throw CompilationError(m_linenum, out.str());
}

void Tokenizer::report_warning(std::ostringstream& out) const
{
    out << fmt::endl << "warning in line " << m_linenum << ":";
    out << fmt::endl;
    std::cerr << out.str();
    throw std::runtime_error("assembler syntax error");
}

ParserToken Tokenizer::AddIdentifier(const std::string& name)
{
    const auto it = m_MapNameToIdent.find(name);
    if (it != m_MapNameToIdent.end()) {
        return ParserToken(token_identifier, it->second);
    }

    // new identifier
    const u32 id = m_Idents.size();
    m_MapNameToIdent[name] = id;
    m_Idents.emplace_back(name);
    return ParserToken(token_identifier, id);
}

ParserToken Tokenizer::TokenizeIdentifier(FormatterInterface& formatter, int chr)
{
    std::string name;

    name += chr;
    while ((chr = m_Scanner.Get(), chr >= 0) && (isalnum(chr) || chr == '_')) {
        name += chr;
    }
    m_Scanner.Unget(chr);

    ParserToken token = AddIdentifier(name);
    formatter.echo_ident_old(name);
    return token;
}

ParserToken Tokenizer::TokenizeMantissa(FormatterInterface& UNUSED(formatter), size_t txtpos)
{
    return ParserToken(token_real, txtpos);
}

ParserToken Tokenizer::TokenizeDecimal(FormatterInterface& formatter, size_t txtpos)
{
    ETokenKind token_type = token_decimal;

    int chr;
    while ((chr = m_Scanner.Get(), chr >= 0) && isdigit(chr)) {
        m_TextStorage.Store(chr);
    }

    if (chr == '.') { // mantissa
        m_TextStorage.Store(chr);
        while ((chr = m_Scanner.Get(), chr >= 0) && isdigit(chr)) {
            m_TextStorage.Store(chr);
        }
        token_type = token_real;
    }

    if (chr == 'e' || chr == 'E') {
        m_TextStorage.Store('e');
        chr = m_Scanner.Get();
        if (chr == '-' || chr == '+') {
            m_TextStorage.Store(chr);
            chr = m_Scanner.Get();
        }
        if (isdigit(chr)) {
            m_TextStorage.Store(chr);
        } else {
            REPORT_ERROR(this) << "must be minimum one digit after decimal point";
        }
        while ((chr = m_Scanner.Get(), chr >= 0) && isdigit(chr)) {
            m_TextStorage.Store(chr);
        }
        token_type = token_real;
    }
    m_Scanner.Unget(chr);
    m_TextStorage.Store(0);

    if (token_type == token_decimal) {
        formatter.echo_decimal(m_TextStorage.GetData(txtpos));
    } else {
        formatter.echo_real(m_TextStorage.GetData(txtpos));
    }
    LOG_DEBUG(LOG_TOKENIZER) << "\ndig: " << m_TextStorage.GetData(txtpos);
    return ParserToken(token_type, txtpos);
}

ParserToken Tokenizer::TokenizeConstant(FormatterInterface& formatter, int chr)
{
    ParserToken token;
    const size_t txtpos = m_TextStorage.GetCurrentPosition();
    m_TextStorage.Store(chr);
    if (chr == '0') {
        chr = m_Scanner.Get();
        if (chr == 'x' || chr == 'X') {
            m_TextStorage.Store('x');
            while ((chr = m_Scanner.Get(), chr >= 0) && isxdigit(chr)) {
                m_TextStorage.Store(chr);
            }
            m_Scanner.Unget(chr);
            m_TextStorage.Store(0);
            token = ParserToken(token_hexadecimal, txtpos);
            formatter.echo_hexadecimal(m_TextStorage.GetData(txtpos));
        } else if (chr == 'b' || chr == 'B') {
            m_TextStorage.Store('b');
            while ((chr = m_Scanner.Get(), chr >= 0) && util::isbinary(chr)) {
                 m_TextStorage.Store(chr);
            }
            m_Scanner.Unget(chr);
            m_TextStorage.Store(0);
            token = ParserToken(token_binary, txtpos);
            formatter.echo_hexadecimal(m_TextStorage.GetData(txtpos));
        } else if (util::isoctal(chr)) {
            m_TextStorage.Store(chr);
            while ((chr = m_Scanner.Get(), chr >= 0) && util::isoctal(chr)) {
                m_TextStorage.Store(chr);
            }
            m_Scanner.Unget(chr);
            m_TextStorage.Store(0);
            formatter.echo_octal(m_TextStorage.GetData(txtpos));
            token = ParserToken(token_octal, txtpos);
        } else {
            m_Scanner.Unget(chr);
            token = TokenizeDecimal(formatter, txtpos);
        }
    } else {
        token = TokenizeDecimal(formatter, txtpos);
    }
    return token;
}

ParserToken Tokenizer::TokenizeOperator(FormatterInterface& formatter, EOperatorKind op)
{
    formatter.echo_operator(op);
    return ParserToken(op);
}

ParserToken Tokenizer::TokenizeOperator(FormatterInterface& formatter, int chr)
{
    switch (chr) {
        case '(':    return TokenizeOperator(formatter, operator_l_parenthese);
        case ')':    return TokenizeOperator(formatter, operator_r_parenthese);
        case '[':    return TokenizeOperator(formatter, operator_l_bracket);
        case ']':    return TokenizeOperator(formatter, operator_r_bracket);
        case '{':    return TokenizeOperator(formatter, operator_l_compound);
        case '}':    return TokenizeOperator(formatter, operator_r_compound);
        case ';':    return TokenizeOperator(formatter, operator_semicolon);
        case '?':    return TokenizeOperator(formatter, operator_question);
        case '~':    return TokenizeOperator(formatter, operator_invert);
        case ',':    return TokenizeOperator(formatter, operator_comma);
        case '\\':   return TokenizeOperator(formatter, operator_backslash);

        case '#':
            chr = m_Scanner.Get();
            if (chr == '#') {
                return TokenizeOperator(formatter, operator_dbl_diez);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_diez);
            }
            break;

        // !, !=
        case '!':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_not_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_not);
            }
            break;

        // ":", "::" ":="
        case ':':
            chr = m_Scanner.Get();
            if (chr == ':') {
                return TokenizeOperator(formatter, operator_dbl_colon);
            } else if (chr == '=') {
                return TokenizeOperator(formatter, operator_assign);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_colon);
            }
            break;

        // ".", ".*", "..."
        case '.':
            chr = m_Scanner.Get();
            if (chr == '*') {
                return TokenizeOperator(formatter, operator_point_mul);
            } else if (chr == '.') {
                chr = m_Scanner.Get();
                if (chr == '.') {
                    return TokenizeOperator(formatter, operator_ellipsis);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_dbl_point);
                }
            } else if (isdigit(chr)) {
                const size_t txtpos = m_TextStorage.GetCurrentPosition();
                m_TextStorage.Store('.');
                m_Scanner.Unget(chr);
                return TokenizeMantissa(formatter, txtpos);
            }  else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_point);
            }
            break;

        // =, ==
        case '=':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_dbl_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_equal);
            }
            break;

        // *, *=
        case '*':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_product_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_product);
            }
            break;

        // /, /=, //
        case '/':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_slash_equal);
            } else if (chr == '/') {
                return TokenizeOperator(formatter, operator_dbl_slash);
            } else {
                assert(chr != '*'); // comments should be already skipped
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_slash);
            }
            break;

        // %, %=, %%
        case '%':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_percent_equal);
            } else if (chr == '%') {
                return TokenizeOperator(formatter, operator_dbl_percent);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_percent);
            }
            break;

        // &, &&, &=, &&=,
        case '&':
            chr = m_Scanner.Get();
            if (chr == '&') {
                chr = m_Scanner.Get();
                if (chr == '=') {
                    return TokenizeOperator(formatter, operator_dbl_and_equal);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_dbl_and);
                }
            } else if (chr == '=') {
                return TokenizeOperator(formatter, operator_and_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_bitand);
            }
            break;

        // |, ||, |=, ||=,
        case '|':
            chr = m_Scanner.Get();
            if (chr == '|') {
                chr = m_Scanner.Get();
                if (chr == '=') {
                    return TokenizeOperator(formatter, operator_dbl_or_equal);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_dbl_or);
                }
            } else if (chr == '=') {
                return TokenizeOperator(formatter, operator_or_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_bitor);
            }
            break;

        // ^, ^=, ^^
        case '^':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_xor_equal);
            } else if (chr == '^') {
                return TokenizeOperator(formatter, operator_dbl_xor);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_bitxor);
            }
            break;

        // +, ++, +=
        case '+':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_plus_equal);
            } else if (chr == '+') {
                return TokenizeOperator(formatter, operator_increment);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_plus);
            }
            break;

        // -, -=, --, ->, ->*
        case '-':
            chr = m_Scanner.Get();
            if (chr == '=') {
                return TokenizeOperator(formatter, operator_minus_equal);
            } else if (chr == '-') {
                return TokenizeOperator(formatter, operator_decrement);
            } else if (chr == '>') {
                chr = m_Scanner.Get();
                if (chr == '*') {
                    return TokenizeOperator(formatter, operator_arrow_mul);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_arrow);
                }
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_minus);
            }
            break;

        // <, <=, <<, <<=, <>
        case '<':
            chr = m_Scanner.Get();
            if (chr == '<') { // <<
                chr = m_Scanner.Get();
                if (chr == '=') {
                    return TokenizeOperator(formatter, operator_shl_equal);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_shl);
                }
            } else if (chr == '=') {
                return TokenizeOperator(formatter, operator_less_equal);
            } else if (chr == '>') {
                return TokenizeOperator(formatter, operator_less_greater);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_less);
            }
            break;

        // >, >=, >>, >>>, >>=
        case '>':
            chr = m_Scanner.Get();
            if (chr == '>') { // >>
                chr = m_Scanner.Get();
                if (chr == '=') {
                    return TokenizeOperator(formatter, operator_shr_equal);
                } else if (chr == '>') {
                    return TokenizeOperator(formatter, operator_sar);
                } else {
                    m_Scanner.Unget(chr);
                    return TokenizeOperator(formatter, operator_shr);
                }
            } else if (chr == '=') {
                return TokenizeOperator(formatter, operator_greater_equal);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_greater);
            }
            break;

        case '$':
            chr = m_Scanner.Get();
            if (chr == '$') {
                return TokenizeOperator(formatter, operator_base);
            } else if (isalpha(chr) || chr == '_') {
                return TokenizeIdentifier(formatter, chr);
            } else {
                m_Scanner.Unget(chr);
                return TokenizeOperator(formatter, operator_here);
            }
            break;

        default:
            m_Scanner.Unget(chr);
            REPORT_ERROR(this) << "invalid character code " << chr;

    } // end symbol switch

    return ParserToken(operator_here);
}

ParserToken Tokenizer::TokenizeString(FormatterInterface& formatter)
{
    const size_t txtpos = m_TextStorage.GetCurrentPosition();
    bool backslashcounter = false;
    for (;;) {
        const int chr = m_Scanner.Get();
        if (iscntrl(chr)) {
            REPORT_ERROR(this) << "non-string character, code=" << chr;
        }
        if (chr == '\"' && !backslashcounter) {
            break;
        }
        if (chr == '\\') {
            backslashcounter = !backslashcounter;
        } else {
            backslashcounter = false;
        }
        m_TextStorage.Store(chr);
    }

    m_TextStorage.Store(0);

    formatter.echo_string(m_TextStorage.GetData(txtpos));

    return ParserToken(token_string, txtpos);
}

ParserToken Tokenizer::TokenizeChar(FormatterInterface& formatter)
{
    const size_t txtpos = m_TextStorage.GetCurrentPosition();

    bool backslashcounter = false;
    for (;;) {
        const int chr = m_Scanner.Get();

        if (iscntrl(chr)) {
            REPORT_ERROR(this) << "control character code=" << chr;
        }

        if (chr == '\'' && !backslashcounter) {
            break;
        }

        if (chr == '\\') {
            backslashcounter = !backslashcounter;
        } else {
            backslashcounter = false;
        }

        m_TextStorage.Store(chr);
    }

    m_TextStorage.Store(0);

    formatter.echo_character(m_TextStorage.GetData(txtpos));

    return ParserToken(token_character, txtpos);
}

int Tokenizer::TokenizeSpaces(FormatterInterface& formatter)
{
    int chr;
    while (chr = m_Scanner.Get(), chr >= 0) {
        // single line comment like ';text'
        if (chr == ';' || chr == '#') {
            formatter.echo_comment_start();
            formatter.echo_char(chr);
            while ((chr = m_Scanner.Get(), chr >= 0) && chr != '\n') {
                formatter.echo_char(chr);
            }
            formatter.echo_comment_stop();
        }

        // multiline comment like /* comment */
        if (chr == '/') {
            chr = m_Scanner.Get();
            if (chr == '*') {
                formatter.echo_comment_start();
                formatter.echo_char('/');
                formatter.echo_char('*');
                int oldchr = 0;
                while (chr = m_Scanner.Get(), chr >= 0) {
                    formatter.echo_char(chr);
                    if (chr == '\n') {
                        m_TokenStorage.push_back(ParserToken(token_eol, m_Scanner.GetPosition()));
                        m_linenum++;
                    }
                    if (chr == '/' && oldchr == '*') {
                        break;
                    }
                    oldchr = chr;
                }
                formatter.echo_comment_stop();
                continue;
            } else {
                m_Scanner.Unget(chr);
                chr = '/';
            }
        }

        if (chr == '\n') {
            formatter.echo_char(chr);
            m_TokenStorage.push_back(ParserToken(token_eol, m_Scanner.GetPosition()));
            m_linenum++;
            continue;
        }

        if (isspace(chr)) {
            formatter.echo_char(chr);
            continue;
        }

        return chr;
    }

    return chr;
}

int Tokenizer::TokenizeInput(std::istream& input)
{
    TxtFormatter formatter;
    Tokenizer::Init();
    return TokenizeInput(formatter, input);
}

void Tokenizer::Init()
{
    // precompute tables sizes
    const size_t tablesize = 8 * 1024;

    m_MapNameToIdent.clear();
    m_Idents.clear();
    m_TokenStorage.clear();

    m_Idents.reserve(tablesize);
    m_TokenStorage.reserve(tablesize);

    m_TextStorage.Setup(tablesize);
}

int Tokenizer::TokenizeInput(FormatterInterface& formatter, std::istream& input)
{
    m_Scanner.read(input);
    m_Scanner.ResetPosition();

    m_linenum = 0;

    // insert first token - its always newline marker
    m_TokenStorage.push_back(ParserToken(token_eol, 0));

    int chr;
    while (chr = TokenizeSpaces(formatter), chr >= 0) {
        ParserToken token(token_eof, 0);
        if (isalpha(chr) || chr == '_') {
            token = TokenizeIdentifier(formatter, chr);
        } else if (isdigit(chr)) {
            token = TokenizeConstant(formatter, chr);
        } else if (chr == '\'') {
            token = TokenizeChar(formatter);
        } else if (chr == '"') {
            token = TokenizeString(formatter);
        } else {
            token = TokenizeOperator(formatter, chr);
        }
        m_TokenStorage.push_back(token);
    }

    LOG_DEBUG(LOG_TOKENIZER) << "end_of_file";

    // last token - special 'eof' marker
    m_TokenStorage.push_back( ParserToken( token_eol, m_Scanner.GetPosition() ) );
    m_TokenStorage.push_back( ParserToken( token_eof, 0 ) );

#if 0
    LOG_DEBUG(LOG_TOKENIZER) << "stringpoolsize=" << m_TextStorage.GetCurrentPosition();
    PrintTokens(std::cerr, m_TokenStorage, 1);
#endif

    LOG_DEBUG(LOG_TOKENIZER) << "end tokenizer";
    return 0;
}

/************************************************************************
* this function print token array
************************************************************************/
void Tokenizer::PrintToken(std::ostream& out, ParserToken token) const
{
    const ETokenKind    type = token.GetType();
    const u32      data = token.GetData();

    out << ParserToken::token_names[type] << ' ' << fmt::hex<u32, 7>(data);

    switch (type) {
        case token_decimal:
        case token_binary:
        case token_hexadecimal:
        case token_octal:
        case token_real:
        case token_string:
        case token_character:
            out << ' ' << m_TextStorage.GetData(data) << '\n';
            break;

        case token_identifier:
            out << ' ' << GetIdentifierInfo(data).GetName() << '\n';
            break;

        case token_operator:
        case token_unary:
            out << ' ' << ParserToken::operator_names[data] << '\n';
            break;

        case token_eol:
            out << ' ' << data << '\n';
            break;

        case token_eof:
            out << '\n';
            break;

        default:
            out << " unknown\n";
            break;
    }
}

void Tokenizer::PrintTokens(std::ostream& out, const std::vector<ParserToken>& tokens, int flags) const
{
    std::array<size_t, token_last> counters;
    counters.fill(0);

    const size_t ntokens = tokens.size();

    for (size_t i = 0; i<ntokens; i++)
    {
       ETokenKind term = tokens[i].GetType();
       counters[term]++;
       PrintToken(out, tokens[i]);
    }
    out << '\n';

    if (flags) {
       out << "Summary: " << ntokens << " tokens\n";
       if (ntokens > 0) {
          double tokenpart = 100.0 / ntokens;
          out << std::fixed << std::setprecision(3);
          for (size_t i = 0; i<counters.size(); i++) {
              out << ParserToken::token_names[i]
                  << ": " << std::setw(6) << (counters[i] * tokenpart)
                  << "% " << counters[i]
                  << '\n';
          }
       }
    }
}

const std::array<std::string, token_last> ParserToken::token_names =
{
#define TOKEN_TEXT_X(enumeration, text) text,
    DECLARE_TOKEN_TABLE(TOKEN_TEXT_X)
};

const std::array<std::string, operator_last> ParserToken::operator_names =
{
#define OPERATOR_TEXT_X(enumeration, priority, text) text,
    DECLARE_OPERATOR_TABLE(OPERATOR_TEXT_X)
};

/************************************************************************
* scan and markup
*************************************************************************/
int
Tokenizer::Scan(std::istream& input, std::ostream& out)
{
    TxtFormatter formatter;

    out << m_TokenStorage.size() << fmt::endl;

    Tokenizer::Init();
    TokenizeInput(formatter, input);

    out << m_TokenStorage.size() << fmt::endl;

    PrintTokens(out, m_TokenStorage, 1);
    return 0;
}

/************************************************************************
* html output class for tokenizer
*************************************************************************/
class HtmlFormatter : public FormatterInterface {
public:
    explicit HtmlFormatter(std::ostream& o) : out(o) {}
    ~HtmlFormatter() override {}

    void echo_char(char chr) override {
        output_char(chr);
    }
    void echo_comment_start(void) override {
        out << "<span class=\"comment\">";
    }
    void echo_comment_stop(void) override {
        out << "</span>";
    }
    void echo_keyword(const char *name) override {
        out << "<span class=\"keyword\">" << (name) << "</span>";
    }
    void echo_ident_new(const std::string& name) override {
        out << "<span class=\"ident\">" << (name) << "</span>";
    }
    void echo_ident_old(const std::string& name) override {
        out << "<span class=\"ident\">" << (name) << "</span>";
    }
    void echo_hexadecimal(const char *name) override {
        out << "<span class=\"hex\">" << (name) << "</span>";
    }
    void echo_octal(const char *name) override {
        out << "<span class=\"oct\">" << (name) << "</span>";
    }
    void echo_decimal(const char *name) override {
        out << "<span class=\"dec\">" << (name) << "</span>";
    }
    void echo_real(const char *name) override {
        out << "<span class=\"flt\">" << (name) << "</span>";
    }
    void echo_string(const char *name) override {
        out << "<span class=\"str\">&quot;";
        output_string(name);
        out << "&quot;</span>";
    }
    void echo_operator(EOperatorKind name) override {
        output_string(ParserToken::operator_names[name].c_str());
    }
    void echo_character(char name) override {
        out << "<span class=\"chr\">";
        output_char(name);
        out << "</span>";
    }
    void echo_character(const char *name) override {
        out << "<span class=\"chr\">";
        output_string(name);
        out << "</span>";
    }

private:
    void output_char(char chr) {
        switch (chr) {
            case '<':  out << "&lt;";   break;
            case '>':  out << "&gt;";   break;
            case '&':  out << "&amp;";  break;
            case '\"': out << "&quot;"; break;
            default:   out << chr;      break;
        }
    }
    void output_string(char const *str) {
        for (size_t i = 0; str[i]; i++) {
            output_char(str[i]);
        }
    }

private:
    std::ostream& out;
};

int Tokenizer::ScanHtml(std::istream& input, std::ostream& out)
{
    HtmlFormatter formatter(out);

    out << "<pre>";

    Tokenizer::Init();
    TokenizeInput(formatter, input);
    out << "</pre></body></html>\n";
    return 0;
}

/***************************************************************************
* this function scan token stream for quoted string
* and copy parsed string to opened segment
***************************************************************************/
void Tokenizer::GetString(ProgramSection& seg)
{
    const char *str = m_TextStorage.GetData(GetToken().GetData());
    while (*str) {
        char chr = *str++;

        if (chr == '"')
           break;
        else if (chr != '\\')
           seg.write(chr);
        else {
           chr = *str++;
           switch (chr) {
               case '0':  chr = '\0';  break;
               case 'n':  chr = '\n';  break;
               case 't':  chr = '\t';  break;
               case 'b':  chr = '\b';  break;
               case 'v':  chr = '\v';  break;
               case 'r':  chr = '\r';  break;
               case 'f':  chr = '\f';  break;
               case '\\': chr = '\\';  break;
               case '\'': chr = '\'';  break;
               case '\"': chr = '"';   break;
               default:
                   REPORT_ERROR(this) << "unknown character sequence '\\" << chr << "'";
                   return;
           }
           seg.write(chr);
        }
    }
    seg.write('\0');
}

u64 Tokenizer::GetHexadecimal(u32 tokendata) const
{
    u64 imm  = 0;
    const char *buf = m_TextStorage.GetData(tokendata + 2); // skip 0x prefix
    LOG_DEBUG(LOG_EVALUATE) << "buf = " << buf;
    while (isxdigit(*buf)) {
        if (isdigit(*buf))       imm = imm * 16 + (*buf - '0');
        else if (isupper(*buf))  imm = imm * 16 + (*buf - 'A' + 10);
        else /* lower */         imm = imm * 16 + (*buf - 'a' + 10);
        buf++;
    }

    return imm;
}

u64 Tokenizer::GetChar(u32 tokendata) const
{
    u64 result = 0;
    const char *str = m_TextStorage.GetData(tokendata);
    while (*str) {
        char chr = *str++;

        if (chr == '"')
           break;
        else if (chr != '\\')
           result = (result << 8) | (chr & 255);
        else {
           chr = *str++;
           switch (chr) {
               case '0':  chr = '\0';  break;
               case 'n':  chr = '\n';  break;
               case 't':  chr = '\t';  break;
               case 'b':  chr = '\b';  break;
               case 'v':  chr = '\v';  break;
               case 'r':  chr = '\r';  break;
               case 'f':  chr = '\f';  break;
               case '\\': chr = '\\';  break;
               case '\'': chr = '\'';  break;
               case '\"': chr = '"';   break;
               default:
                   REPORT_ERROR(this) << "unknown character sequence '\\" << chr << "'";
                   return 0;
           }
           result = (result << 8) | (chr & 255);
        }
    }
    return result;
}

u64 Tokenizer::GetDecimal(u32 tokendata) const
{
     u64 imm = 0;
     const char *buf = m_TextStorage.GetData(tokendata);
     while (isdigit(*buf)) {
         imm = imm * 10 + (*buf - '0');
         buf++;
     }
     return imm;
}

u64 Tokenizer::GetOctal(u32 tokendata) const
{
    u64 imm = 0;
    const char *buf = m_TextStorage.GetData(tokendata);
    while (util::isoctal(*buf)) {
        imm = imm * 8 + (*buf - '0');
        buf++;
    }

    return imm;
}

u64 Tokenizer::GetBinary(u32 tokendata) const
{
    u64 imm = 0;
    const char *buf = m_TextStorage.GetData(tokendata + 2); // skip 0b prefix
    while (util::isbinary(*buf)) {
        imm = imm * 2 + (*buf - '0');
        buf++;
    }
    return imm;
}

quad_type Tokenizer::GetFloat(u32 tokendata) const
{
    const char *buf = m_TextStorage.GetData(tokendata);
    std::istringstream iss(buf);
#if USE_QUADMATH
    using postrisc::operator>>;
    native::f128 imm = 0.0;
#else
    mpfr::mpreal imm(0, 256);
#endif
    iss >> imm;
    return imm;
}

} // namespace postrisc
