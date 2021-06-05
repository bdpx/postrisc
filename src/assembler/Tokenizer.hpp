#pragma once

#include <unordered_map>
#include <exception>

#include "ProgramSection.hpp"

#if USE_QUADMATH
#include "util/quadruple.hpp"
#else
#include "util/mpreal.hpp"
#endif

namespace postrisc {

#if USE_QUADMATH
using quad_type = native::f128;
#else
using quad_type = mpfr::mpreal;
#endif

/***********************************************************************
    tokenized program (lexical analyzer)

    token is part of (token_type, terminal_data)

        (identifier,   identifier_num)
        (keyword,      keyword_type)
        (operator,     EOperatorKind)
        (decimal,      string_pool_position)
        (hexadecimal,  string_pool_position)
        (binary,       string_pool_position)
        (octal,        string_pool_position)
        (string,       string_pool_position)
        (character,    string_pool_position)
        (eol,          line_start)
        (eof,          0)

***********************************************************************/

#define DECLARE_TOKEN_TABLE(X) \
    X (undefined,   "und") \
    X (identifier,  "idn") \
    X (decimal,     "dec") \
    X (hexadecimal, "hex") \
    X (octal,       "oct") \
    X (binary,      "bin") \
    X (real,        "flt") \
    X (string,      "str") \
    X (character,   "chr") \
    X (keyword,     "key") \
    X (const,       "con") \
    X (define,      "def") \
    X (register,    "reg") \
    X (operator,    "opr") \
    X (unary,       "una") \
    X (include,     "inc") \
    X (eol,         "eol") \
    X (eof,         "eof") \


enum ETokenKind {
#define TOKEN_ENUM_X(enumeration, text) token_##enumeration,
    DECLARE_TOKEN_TABLE(TOKEN_ENUM_X)
    token_last
};


/*************************************************************************
* operators parsed by lexer
*************************************************************************/
#define DECLARE_OPERATOR_TABLE(X) \
    X (plus,          30,    "+") \
    X (minus,         30,    "-") \
    X (product,       32,    "*") \
    X (slash,         32,    "/") \
    X (dbl_slash,     32,    "//") \
    X (percent,       32,    "%") \
    X (dbl_percent,   32,    "%%") \
    X (shl,           28,    "<<") \
    X (shr,           28,    ">>") \
    X (sar,           28,    ">>>") \
    X (bitand,        26,    "&") \
    X (bitxor,        24,    "^") \
    X (bitor,         22,    "|") \
    X (dbl_and,       18,    "&&") \
    X (dbl_or,        14,    "||") \
    X (dbl_xor,       16,    "^^") \
    X (equal,          0,    "=") \
    X (dbl_equal,     20,    "==") \
    X (less_greater,  20,    "<>") \
    X (not_equal,     20,    "!=") \
    X (less,          20,    "<") \
    X (less_equal,    20,    "<=") \
    X (greater,       20,    ">") \
    X (greater_equal, 20,    ">=") \
    X (increment,      0,    "++") \
    X (decrement,      0,    "--") \
    X (here,           0,    "$") \
    X (base,           0,    "$$") \
    X (diez,           0,    "#") \
    X (point,          0,    ".") \
    X (dbl_point,      0,    "..") \
    X (ellipsis,       0,    "...") \
    X (semicolon,      0,    ";") \
    X (colon,          0,    ":") \
    X (dbl_colon,      0,    "::") \
    X (invert,         0,    "~") \
    X (not,            0,    "!") \
    X (question,       0,    "?") \
    X (dbl_diez,       0,    "##") \
    X (plus_equal,     0,    "+=") \
    X (minus_equal,    0,    "-=") \
    X (xor_equal,      0,    "^=") \
    X (or_equal,       0,    "|=") \
    X (dbl_and_equal,  0,    "&&=") \
    X (dbl_or_equal,   0,    "||=") \
    X (product_equal,  0,    "*=") \
    X (slash_equal,    0,    "/=") \
    X (percent_equal,  0,    "%=") \
    X (shl_equal,      0,    "<<=") \
    X (shr_equal,      0,    ">>=") \
    X (and_equal,      0,    "&=") \
    X (assign,         0,    ":=") \
    X (l_bracket,      0,    "[") \
    X (r_bracket,      0,    "]") \
    X (l_compound,     0,    "{") \
    X (r_compound,     0,    "}") \
    X (l_parenthese,   0,    "(") \
    X (r_parenthese,   0,    ")") \
    X (arrow,          0,    "->") \
    X (arrow_mul,      0,    "->*") \
    X (point_mul,      0,    ".*") \
    X (backslash,      0,    "\\") \
    X (comma,          0,    ",")


enum EOperatorKind {
#define OPERATOR_ENUM_X(name, priority, text) operator_##name,
    DECLARE_OPERATOR_TABLE(OPERATOR_ENUM_X)
    operator_last
};

class CompilationError : public std::exception {
public:
    explicit CompilationError(int _lineNum, const std::string& msg) : lineNum(_lineNum), message(msg) {}
    const char *what(void) const noexcept override { return message.c_str(); }
    const std::string& GetMessage() const { return message; }
    int GetLineNum(void) const { return lineNum; }

private:
    int lineNum;
    std::string message;
};

/*************************************************************************
* tokenized terminal element
*************************************************************************/
class ParserToken {
public:
    explicit constexpr ParserToken(void) : value(0) {}
    explicit constexpr ParserToken(ETokenKind type, size_t data) : value(static_cast<u32>(data) | (static_cast<u32>(type) << data_bits)) {}
    explicit constexpr ParserToken(EOperatorKind op) : value(op | (static_cast<u32>(token_operator) << data_bits)) {}

    ETokenKind GetType(void) const { return static_cast<ETokenKind>(value >> data_bits); }
    u32 GetData(void) const { return value & data_mask; }

    friend bool operator==(ParserToken a, ParserToken b) { return a.value == b.value; }
    friend bool operator!=(ParserToken a, ParserToken b) { return a.value != b.value; }

    friend std::ostream& operator<<(std::ostream& out, const ParserToken token) {
        return out << static_cast<u32>(token.GetType()) << ':' << token.GetData();
    }

public:
    static const std::array<std::string, operator_last> operator_names;
    static const std::array<std::string, token_last> token_names;

private:
    static const u32 data_bits = 27;
    static const u32 data_mask = (U32C(1) << data_bits) - 1;

private:
    u32 value;
};


#define token_directive ParserToken(operator_point)

/***************************************************************************
*
*
***************************************************************************/
class AssemblerIdentifier {
public:
    explicit AssemblerIdentifier(const std::string& _name) : name(_name) {}

    void SetSectionInfo(u32 id, u32 offset) { sectionId = id; data = offset; }
    u32 GetSectionId(void) const { return sectionId; } 
    bool IsValidSection(void) const { return sectionId != s_InvalidSectionId; }
    const std::string& GetName(void) const { return name; }

public:
    u32 data = 0; // user defined token info
    u32 mask = 0;

private:
    static const u32 s_InvalidSectionId = std::numeric_limits<u32>::max();

private:
    u32 sectionId = s_InvalidSectionId;
    std::string name;
};

class Scanner {
public:
    Scanner(void) {}

    int Get(void) { return m_Position < m_Source.size() ? 255 & m_Source[m_Position++] : -1; }
    void Unget(int) { m_Position--; }
    size_t GetPosition(void) const { return m_Position; }
    void ResetPosition(void) { m_Position = 0; }
    void read(std::istream& input);
    char operator[](size_t index) const { return m_Source[index]; }

private:
    std::vector<char> m_Source;
    size_t            m_Position = 0;
};

class TextStorage {
public:
    const char *GetData(size_t dataOffset) const { return &m_TextData[dataOffset]; }
    size_t GetCurrentPosition(void) const { return m_TextData.size(); }
    void Store(char chr) { m_TextData.push_back(chr); }
    void RollBackToPosition(size_t position) { m_TextData.resize(position); }

    void Setup(size_t size) {
        m_TextData.clear();
        m_TextData.reserve(size);
    }

private:
    std::vector<char>  m_TextData;
};

class FormatterInterface;

class Tokenizer {
public:
    Tokenizer(void) {}
    ~Tokenizer();
    void InitPass();
    int Scan(std::istream& input, std::ostream& out);
    int ScanHtml(std::istream& input, std::ostream& out);
    void PrintTokens(std::ostream& out, const std::vector<ParserToken>& token, int flags) const;
    void PrintToken(std::ostream& out, ParserToken token) const;

    void report_error(std::ostringstream& out) const;
    void report_warning(std::ostringstream& out) const;

    unsigned GetLineNum(void) const { return m_linenum; }

    AssemblerIdentifier& GetIdentifierInfo(u32 ident) { return m_Idents[ident]; }
    const AssemblerIdentifier& GetIdentifierInfo(u32 ident) const { return m_Idents[ident]; }

    void Init(void);
    int TokenizeInput(FormatterInterface& formatter, std::istream& input);
    int TokenizeInput(std::istream& input);

    u64 GetChar(u32 data) const;
    u64 GetHexadecimal(u32 data) const;
    u64 GetDecimal(u32 data) const;
    u64 GetOctal(u32 data) const;
    u64 GetBinary(u32 data) const;
    quad_type GetFloat(u32 data) const;

    ParserToken AddIdentifier(const std::string& name);

protected:
    bool IsNextRowExist(void);
    void GetString(ProgramSection& seg);
    ParserToken GetToken(void);
    ParserToken GetCurrentToken(unsigned offset = 0) const;
    void PopToken(void);
    void PutBackToken(void);

private:
    int TokenizeSpaces(FormatterInterface& formatter);
    ParserToken TokenizeIdentifier(FormatterInterface& formatter, int chr);
    ParserToken TokenizeConstant(FormatterInterface& formatter, int chr);
    ParserToken TokenizeDecimal(FormatterInterface& formatter, size_t txtpos);
    ParserToken TokenizeMantissa(FormatterInterface& formatter, size_t txtpos);
    ParserToken TokenizeString(FormatterInterface& formatter);
    ParserToken TokenizeChar(FormatterInterface& formatter);
    ParserToken TokenizeOperator(FormatterInterface& formatter, int chr);
    ParserToken TokenizeOperator(FormatterInterface& formatter, EOperatorKind op);

private:
    Scanner                                      m_Scanner;
    TextStorage                                  m_TextStorage;
    std::vector<ParserToken>                     m_TokenStorage;
    std::unordered_map<std::string, u32>         m_MapNameToIdent;
    std::vector<AssemblerIdentifier>             m_Idents;
    unsigned int                                 m_token_pos = 0;  // token stream current position
    unsigned int                                 m_linenum = 0;    // token stream input line num
};

/****************************************************************************
* assembler error messaging
****************************************************************************/
#define REPORT_ERROR(ctrl)    if (false) {} else ErrorHandler(ctrl) += Formatter()
#define REPORT_WARNING(ctrl)  if (false) {} else WarningHandler(ctrl) += Formatter()

class Formatter {
public:
    Formatter() {}
    ~Formatter() {}
    Formatter(const Formatter&) = delete;
    Formatter& operator=(Formatter&) = delete;

    template <typename Type> Formatter& operator<<(Type&& value) {
        stream_ << std::forward<Type>(value);
        return *this;
    }

    std::ostringstream& stream() { return stream_; }

private:
    std::ostringstream stream_;
};

class ErrorHandler {
public:
    explicit ErrorHandler(const Tokenizer *p) : ctrl(p) {}
    void operator+=(Formatter& formatter) {
        ctrl->report_error(formatter.stream());
    }
private:
    const Tokenizer *ctrl;
};

class WarningHandler {
public:
    explicit WarningHandler(const Tokenizer *p) : ctrl(p) {}
    void operator+=(Formatter& formatter) {
        ctrl->report_warning(formatter.stream());
    }
private:
    const Tokenizer *ctrl;
};

class TokenPrinter {
public:
    explicit TokenPrinter(const Tokenizer& _assembler, ParserToken _token)
        : assembler(_assembler)
        , token(_token)
    { }

    friend std::ostream& operator<<(std::ostream& out, const TokenPrinter& printer) {
        printer.assembler.PrintToken(out, printer.token);
        return out;
    }

private:
    const Tokenizer&  assembler;
    ParserToken       token;
};

class TokenVectorPrinter {
public:
    TokenVectorPrinter(const Tokenizer& _assembler, const std::vector<ParserToken>& _tokens, int _flag)
        : assembler(_assembler)
        , tokens(_tokens)
        , flag(_flag)
    { }

    friend std::ostream& operator<<(std::ostream& out, const TokenVectorPrinter& printer) {
        printer.assembler.PrintTokens(out, printer.tokens, printer.flag);
        return out;
    }

private:
    const Tokenizer&                    assembler;
    const std::vector<ParserToken>&     tokens;
    int                                 flag;
};

} //namespace postrisc
