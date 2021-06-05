#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Assembler.hpp"

namespace postrisc {

/****************************************************************************
* asembler embedded functions
****************************************************************************/

#define DECLARE_KEYWORDS_TABLE(X) \
    X(data_hi) X(data_lo) \
    X(text_hi) X(text_lo) \

enum {
#define KEYWORD_ENUM_X(name) term_##name,
    DECLARE_KEYWORDS_TABLE(KEYWORD_ENUM_X)
    term_last_keyword
};

static const keywordmap<u32> keywordtable =
{{
#define KEYWORD_NAME_X(name) { #name, term_##name },
    DECLARE_KEYWORDS_TABLE(KEYWORD_NAME_X)
}};

/****************************************************************************
* asembler embedded constants
****************************************************************************/

#define DECLARE_CONST_TABLE(X)

enum {
#define CONST_ENUM_X(name) term_##name,
    DECLARE_CONST_TABLE(CONST_ENUM_X)
    term_last_const
};

static const keywordmap<u32> consttable =
{{
#define CONST_NAME_X(name) { #name, term_##name },
    DECLARE_CONST_TABLE(CONST_NAME_X)
}};

/***************************************************************************
* assembler formula evaluator
***************************************************************************/
static const unsigned char operator_priority[operator_last+1] =
{
#define OPERATOR_PRIORITY_X(enumeration, priority, text) priority,
    DECLARE_OPERATOR_TABLE(OPERATOR_PRIORITY_X)
};

/***************************************************************************
* assembler integer formula evaluator
***************************************************************************/
i64
Assembler::EvaluateInt(unsigned int nbits, eval_mode_type evalmode, i64 addval)
{
    int  level = 0;
    bool need_binop = false;

    std::vector<ParserToken> postfix, store;

    nbits = std::min(64U, nbits);
    const ParserToken omega(operator_last);
    store.push_back(omega);
    LOG_DEBUG(LOG_EVALUATE) << "formula input:";

    for (;;) {
        const ParserToken token = GetToken();
        LOG_DEBUG(LOG_EVALUATE) << "next token: " << TokenPrinter(*this, token);

        const ETokenKind tokentype = token.GetType();
        const u32 tokendata = token.GetData();

        LOG_DEBUG(LOG_EVALUATE)
             << "term=" << tokentype
             << " data=" << tokendata;

        switch (tokentype) {
            case token_identifier: {
                const std::string& id_name = GetIdentifierInfo(tokendata).GetName();
                LOG_DEBUG(LOG_EVALUATE) << "ident = " << id_name;
                if (const auto it = keywordtable.find(id_name); it != keywordtable.end()) {
                    const u32 cmd = it->second;
                    switch (cmd) {
                        case term_text_lo:
                        case term_data_lo:
                        case term_data_hi:
                        case term_text_hi: {
                            LOG_DEBUG(LOG_EVALUATE) << "push function = " << token;
                            if (need_binop) {
                                REPORT_ERROR(this) << "need operator";
                            }
                            store.push_back(ParserToken(token_keyword, cmd));
                            const ParserToken next_token = GetToken();
                            if (next_token != ParserToken(operator_l_parenthese)) {
                                REPORT_ERROR(this) << "'(' expected";
                            }
                            store.push_back(next_token);
                            level++;
                            need_binop = false;
                            }
                            break;
                        default:
                            REPORT_ERROR(this) << "error function " << cmd;
                    }
                    break;
                }
                else if (const auto cit = consttable.find(id_name); cit != consttable.end()) {
                    LOG_DEBUG(LOG_EVALUATE) << "push = " << tokentype;
                    if (need_binop) {
                        REPORT_ERROR(this) << "need operator";
                    }
                    const u32 cmd = cit->second;
                    postfix.push_back(ParserToken(token_const, cmd));
                    need_binop = true;
                    break;
                }
                else if (const auto def_it = m_defines.find(id_name); def_it != m_defines.end()) {
                    LOG_DEBUG(LOG_EVALUATE) << "push = " << tokentype;
                    if (need_binop) {
                        REPORT_ERROR(this) << "need operator";
                    }
                    u32 const cmd = def_it->second;
                    //std::cerr << "token_define found: " << def_it->first << ", " << cmd << ", " << fmt::hex(m_define_values[cmd]) << std::endl;
                    postfix.push_back(ParserToken(token_define, cmd));
                    need_binop = true;
                    break;
                }
                else {
                    LOG_DEBUG(LOG_EVALUATE) << "true name: " << id_name;
                }
                //std::cerr << "unknown id: " << id_name << std::endl;
            }
            // fall through

            case token_decimal:
            case token_hexadecimal:
            case token_octal:
            case token_binary:
            case token_character:
                LOG_DEBUG(LOG_EVALUATE) << "push = " << tokentype;
                if (need_binop) {
                    REPORT_ERROR(this) << "need operator";
                }
                postfix.push_back(token);
                need_binop = true;
                break;

            case token_operator:
                switch (tokendata) {

                    case operator_l_parenthese:
                        if (need_binop)
                            goto end_formula;
                            // REPORT_ERROR(this) << "error '(', need operator";
                        LOG_DEBUG(LOG_EVALUATE) << "push ( = " << token;
                        store.push_back(token);
                        level++;
                        need_binop = false;
                        break;

                    case operator_r_parenthese:
                        if (!need_binop) {
                            REPORT_ERROR(this) << "error ')', need value";
                        }
                        if (level > 0) {
                            level--;
                            while (store.back() != ParserToken(operator_l_parenthese)) {
                                LOG_DEBUG(LOG_EVALUATE) << "pops pushp = " << store.back();
                                postfix.push_back(store.back());
                                store.pop_back();
                                if (store.empty()) {
                                    REPORT_ERROR(this) << "postfix stack underflow!!!";
                                }
                            }
                            LOG_DEBUG(LOG_EVALUATE) << "pops last = " << store.back();
                            store.pop_back();
                        } else {
                            REPORT_ERROR(this) << "error ')' without '('";
                        }
                        break;

                    case operator_invert:
                    case operator_not:
                        if (!need_binop) {
                            // unary ops
                            store.push_back(ParserToken(token_unary, tokendata));
                            break;
                        } else {
                            REPORT_ERROR(this) << "error unary operator";
                        }
                        break;

                    case operator_minus:
                    case operator_plus:
                        if (!need_binop) {
                            // unary ops
                            store.push_back(ParserToken(token_unary, tokendata));
                            break;
                        }
                        // fallthrough

                    default:
                        if (tokendata >= operator_last || operator_priority[tokendata] == 0) {
                            goto end_formula;
                        }
                        if (!need_binop) {
                            REPORT_ERROR(this) << "error operator";
                        }
                        while (store.back().GetType() == token_keyword ||
                               store.back().GetType() == token_unary) {
                            LOG_DEBUG(LOG_EVALUATE) << "pop function = " << store.back();
                            postfix.push_back(store.back());
                            store.pop_back();
                        }
                        while (operator_priority[tokendata] <= operator_priority[store.back().GetData()]) {
                            LOG_DEBUG(LOG_EVALUATE) << "pop stored ops & emit = " << store.back();
                            postfix.push_back(store.back());
                            store.pop_back();
                        }
                        LOG_DEBUG(LOG_EVALUATE) << "push last op = " << token;
                        store.push_back(token);
                        need_binop = false;
                        break;
                }
                break;

            default:
                goto end_formula;
        }
    }

end_formula:
    if (!need_binop || level > 0) {
        LOG_DEBUG(LOG_EVALUATE) << "level=" << level;
        REPORT_ERROR(this) << "error last";
    }

    while (store.back() != omega) {
        LOG_DEBUG(LOG_EVALUATE) << "pop stored, push = " << store.back();
        postfix.push_back(store.back());
        store.pop_back();
    }
    postfix.push_back(ParserToken(token_eol, 0));

    i64 imm = EvaluatePostfixInt(postfix, evalmode);
    imm = imm + addval;

    PutBackToken();
    LOG_DEBUG(LOG_EVALUATE) << "current_token: " << GetCurrentToken();

    if (nbits < 64) {
        if (evalmode & eval_unsigned) {
            if (static_cast<u64>(imm) != util::zext64(imm, nbits)) {
                REPORT_ERROR(this) << "unsigned value " << imm << " exceed " << nbits << "-bit limit";
            }
        } else {
            if (static_cast<i64>(imm) != util::sext64(imm, nbits)) {
               REPORT_ERROR(this) << "signed value " << imm << " exceed " << nbits << "-bit limit";
            }
        }
    }

    // truncate true signed value for future oring
    imm = util::zext64(imm, nbits);
    return (imm);
}

i64
Assembler::EvaluatePostfixInt(const std::vector<ParserToken>& postfix, eval_mode_type evalmode) const
{
    LOG_DEBUG(LOG_EVALUATE) << "postfix:\n" << TokenVectorPrinter(*this, postfix, 0);

    std::vector<i64> values;
    //values.reserve(100);

    for (const ParserToken& token : postfix) {
        const ETokenKind tokentype = token.GetType();
        const u32 tokendata = token.GetData();
        i64        imm;

        switch (tokentype) {
            case token_decimal:
                imm = GetDecimal(tokendata);
                values.push_back(imm);
                break;

            case token_hexadecimal:
                imm = GetHexadecimal(tokendata);
                values.push_back(imm);
                break;

            case token_octal:
                imm = GetOctal(tokendata);
                values.push_back(imm);
                break;

            case token_binary:
                imm = GetBinary(tokendata);
                values.push_back(imm);
                break;

            case token_character:
                imm = GetChar(tokendata);
                values.push_back(imm);
                break;

            case token_const:
                switch (tokendata) {
                    default:
                        REPORT_ERROR(this) << "error const";
                }
                break;

            case token_define:
                // std::cerr << "token_define: " << tokendata << ", " << fmt::hex(m_define_values[tokendata]) << std::endl;
                values.push_back(m_define_values[tokendata]);
                break;

            case token_keyword:
                imm = values.back();
                switch (tokendata) {
                    case term_data_lo:
                        values.back() = lodisp(imm, bits_per_binimm);
                        break;

                    case term_data_hi:
                        values.back() = hidisp(imm, bits_per_binimm);
                        break;

                    case term_text_lo:
                        values.back() = lodisp(imm, log_bytes_per_bundle);
                        break;

                    case term_text_hi:
                        values.back() = hidisp(imm, log_bytes_per_bundle);
                        break;

                    default:
                        REPORT_ERROR(this) << "error function";
                }
                break;

            case token_identifier:
                if (!GetIdentifierInfo(tokendata).IsValidSection()) {
                    if (evalmode & eval_critical) {
                        REPORT_ERROR(this) << "undefined " << GetIdentifierInfo(tokendata).GetName();
                    }
                    return 0;
                } else {
                    const std::string& cmd = GetIdentifierInfo(tokendata).GetName();

                    if (m_PassNumber < EAssemblerPass::link) { // first compiler pass
                        return 0;
                    }

                    const u32 sectionId = GetIdentifierInfo(tokendata).GetSectionId();
                    i64 pimm = GetIdentifierInfo(tokendata).data;
                    imm = m_Sections[sectionId].ComputeIpRelative(0, pimm);
                    LOG_DEBUG(LOG_EVALUATE) << "identifier: sectionId=" << sectionId << ", " << cmd << "pimm=" << pimm << " imm" << imm;
                }
                values.push_back(imm);
                break;

            case token_unary:
                imm = values.back();
                switch (tokendata) {
                    case operator_minus:    values.back() = -imm;    break;
                    case operator_plus:     values.back() =  imm;    break;
                    case operator_invert:   values.back() = ~imm;    break;
                    case operator_not:      values.back() = !imm;    break;
                    default:
                        REPORT_ERROR(this) << "error unary";
                }
                break;

            case token_operator: {
                i64 vb = values.back();
                values.pop_back();
                i64 va = values.back();
                switch (tokendata) {
                    case operator_plus:            values.back() = va + vb;    break;
                    case operator_minus:           values.back() = va - vb;    break;
                    case operator_product:         values.back() = va * vb;    break;
                    case operator_slash:        if (vb == 0) { REPORT_ERROR(this) << "divide by zero"; }  values.back() = (i64)va / (i64)vb;  break;
                    case operator_dbl_slash:    if (vb == 0) { REPORT_ERROR(this) << "divide by zero"; }  values.back() = (u64)va / (u64)vb;  break;
                    case operator_percent:      if (vb == 0) { REPORT_ERROR(this) << "divide by zero"; }  values.back() = (i64)va % (i64)vb;  break;
                    case operator_dbl_percent:  if (vb == 0) { REPORT_ERROR(this) << "divide by zero"; }  values.back() = (u64)va % (u64)vb;  break;
                    case operator_shl:             values.back() = (u64)va << vb;     break;
                    case operator_shr:             values.back() = (u64)va >> vb;     break;
                    case operator_sar:             values.back() = (i64)va >> vb;    break;
                    case operator_bitand:          values.back() = va & vb;                break;
                    case operator_bitxor:          values.back() = va ^ vb;                break;
                    case operator_bitor:           values.back() = va | vb;                break;
                    case operator_dbl_and:         values.back() = (va && vb);             break;
                    case operator_dbl_or:          values.back() = (va || vb);             break;
                    case operator_dbl_xor:         values.back() = (va != vb);             break;
                    case operator_dbl_equal:       values.back() = (va == vb);             break;
                    case operator_less_greater:    values.back() = (va != vb);             break;
                    case operator_not_equal:       values.back() = (va != vb);             break;
                    case operator_less:            values.back() = (va <  vb);             break;
                    case operator_less_equal:      values.back() = (va <= vb);             break;
                    case operator_greater:         values.back() = (va >  vb);             break;
                    case operator_greater_equal:   values.back() = (va >= vb);             break;
                    default:
                        REPORT_ERROR(this) << "unknown binary operator";
                }
                }
                break;

            default:
                break;
        }
    }

    i64 const result = values.back();
    LOG_DEBUG(LOG_EVALUATE) << "formula:" << result;
    return result;
}

/***************************************************************************
* assembler float formula evaluator
***************************************************************************/
#define FUNC_TABLE(X) \
    X(acos) X(acosh) X(asin)          \
    X(asinh) X(atan) X(atanh)         \
    X(cbrt) X(ceil) X(cos) X(cosh)    \
    X(erf) X(erfc) X(exp) X(expm1)    \
    X(fabs) X(floor)                  \
    X(lgamma)                         \
    X(log) X(log10) X(log1p) X(log2)  \
    X(rint) X(round)                  \
    X(sin) X(sinh) X(sqrt)            \
    X(tan) X(tanh) X(tgamma) X(trunc) \

//        X(j0) X(j1)
//        X(y0) X(y1)
#if USE_QUADMATH
    #define FLOAT_FUNC_NAME(name) name##q
#else
    #define FLOAT_FUNC_NAME(name) name
#endif

enum {
#define FUNC_ENUM_X(name) func_table_##name,
    FUNC_TABLE(FUNC_ENUM_X)
    func_table_size
};

#define FUNC_NAME_X(name) { #name, func_table_##name },
static const keywordmap<u32> func_name_list =
{
    FUNC_TABLE(FUNC_NAME_X)
};

quad_type
Assembler::EvaluateFloat(void)
{
    int  level = 0;
    bool need_binop = false;

    std::vector<ParserToken> postfix, store;

    const ParserToken omega(operator_last);
    store.push_back(omega);
    LOG_DEBUG(LOG_EVALUATE) << "floating-point formula input:";

    for (;;)
    {
        const ParserToken token = GetToken();

        LOG_DEBUG(LOG_EVALUATE) << TokenPrinter(*this, token);

        const ETokenKind  tokentype = token.GetType();
        const u32         tokendata = token.GetData();

        LOG_DEBUG(LOG_EVALUATE)
            << "term=" << tokentype
            << " data=" << tokendata;

        switch (tokentype) {
            case token_hexadecimal:
            case token_octal:
            case token_binary:
                REPORT_ERROR(this) << "invalid floating-point value";
                break;

            case token_decimal:
            case token_real:
                LOG_DEBUG(LOG_EVALUATE) << "push = " << tokentype;
                if (need_binop) {
                    REPORT_ERROR(this) << "need operator";
                }
                postfix.push_back(token);
                need_binop = true;
                break;

            case token_identifier: {
                LOG_DEBUG(LOG_EVALUATE) << "push function = " << token;
                if (need_binop) {
                    REPORT_ERROR(this) << "need operator";
                }
                const auto it = func_name_list.find(GetIdentifierInfo(tokendata).GetName());
                if (it == func_name_list.end()) {
                    REPORT_ERROR(this) << "unknown floating point function";
                }
                const u32 func = it->second;
                LOG_DEBUG(LOG_EVALUATE) << "func = " << func;
                store.push_back(ParserToken(token_keyword, func));
                const ParserToken next_token = GetToken();
                if (next_token != ParserToken(operator_l_parenthese)) {
                    REPORT_ERROR(this) << "'(' expected";
                }
                store.push_back(next_token);
                level++;
                need_binop = false;
                }
                break;

            case token_operator:
                switch (tokendata) {
                    case operator_l_parenthese:
                        if (need_binop)
                            goto end_formula;
                            // REPORT_ERROR(this) << "error '(', need operator";
                        LOG_DEBUG(LOG_EVALUATE) << "push ( = " << token;
                        store.push_back(token);
                        level++;
                        need_binop = false;
                        break;

                    case operator_r_parenthese:
                        if (!need_binop) {
                            REPORT_ERROR(this) << "error ')', need value";
                        }
                        if (level > 0) {
                            level--;
                            while (store.back() != ParserToken(operator_l_parenthese)) {
                                LOG_DEBUG(LOG_EVALUATE) << "pops pushp = " << store.back();
                                postfix.push_back(store.back());
                                store.pop_back();
                                if (store.empty()) {
                                    REPORT_ERROR(this) << "postfix stack underflow!!!";
                                }
                            }
                            LOG_DEBUG(LOG_EVALUATE) << "pops last = " << store.back();
                            store.pop_back();
                        } else {
                            REPORT_ERROR(this) << "error ')' without '('";
                        }
                        break;

                    case operator_invert:
                    case operator_not:
                        if (!need_binop) {
                            // unary ops
                            store.push_back(ParserToken(token_unary, tokendata));
                            break;
                        } else {
                            REPORT_ERROR(this) << "error unary operator";
                        }
                        break;

                    case operator_minus:
                    case operator_plus:
                        if (!need_binop) {
                            // unary ops
                            store.push_back(ParserToken(token_unary, tokendata));
                            break;
                        }
                        // fallthrough

                    default:
                        if (tokendata >= operator_last || operator_priority[tokendata] == 0) {
                            goto end_formula;
                        }
                        if (!need_binop) {
                            REPORT_ERROR(this) << "error operator";
                        }
                        while (store.back().GetType() == token_keyword ||
                               store.back().GetType() == token_unary) {
                            LOG_DEBUG(LOG_EVALUATE) << "pop function = " << store.back();
                            postfix.push_back(store.back());
                            store.pop_back();
                        }
                        while (operator_priority[tokendata] <= operator_priority[store.back().GetData()]) {
                            LOG_DEBUG(LOG_EVALUATE) << "pop stored ops & emit = " << store.back();
                            postfix.push_back(store.back());
                            store.pop_back();
                        }
                        LOG_DEBUG(LOG_EVALUATE) << "push last op = " << token;
                        store.push_back(token);
                        need_binop = false;
                        break;
                }
                break;

            default:
                goto end_formula;
        }
    }

end_formula:
    if (!need_binop || level > 0) {
        REPORT_ERROR(this) << "error last";
    }

    while (store.back() != omega) {
        LOG_DEBUG(LOG_EVALUATE) << "pop stored, push = " << store.back();
        postfix.push_back(store.back());
        store.pop_back();
    }
    postfix.push_back(ParserToken(token_eol, 0));

    quad_type result = EvaluatePostfixReal(postfix);

    PutBackToken();

    return result;
}

quad_type
Assembler::EvaluatePostfixReal(const std::vector<ParserToken>& postfix)
{
    LOG_DEBUG(LOG_EVALUATE) << "\npostfix:\n" << TokenVectorPrinter(*this, postfix, 0);

    std::vector<quad_type> values;

    for (const ParserToken& token : postfix) {
        const ETokenKind tokentype = token.GetType();
        const u32 tokendata = token.GetData();

        switch (tokentype) {
            case token_binary:
            case token_hexadecimal:
            case token_octal:
                REPORT_ERROR(this) << "invalid floating-point literal\n";
                break;

            case token_decimal:
            case token_real:
                values.push_back(GetFloat(tokendata));
                break;

            case token_keyword:
                LOG_DEBUG(LOG_EVALUATE) << "function: " << tokendata << values.back();
                switch (tokendata) {
                    #define FUNC_CASE_X(name) \
                         case func_table_##name: \
                             values.back() = FLOAT_FUNC_NAME(name)(values.back()); \
                             break;
                    FUNC_TABLE(FUNC_CASE_X)
                    #undef FUNC_CASE_X
                    default:
                        LOG_ERROR(LOG_EVALUATE) << "unknown function: " << tokendata;
                }
                LOG_DEBUG(LOG_EVALUATE) << "end function: " << tokendata;
                break;

            case token_identifier:
                if (!GetIdentifierInfo(tokendata).IsValidSection()) {
                    PutBackToken();
                    return (0.0);
                }
                values.push_back(GetIdentifierInfo(tokendata).data);
                break;

            case token_unary:
                switch (tokendata) {
                    case operator_minus:
                        values.back() = -values.back();
                        break;

                    case operator_plus:
                        // values.back() = values.back();
                        break;

                    default:
                        REPORT_ERROR(this) << "unknown unary float operator";
                }
                break;

            case token_operator: {
                quad_type vb = values.back();
                values.pop_back();
                quad_type va = values.back();

                switch (tokendata) {
                    case operator_plus:
                        values.back() = va + vb;
                        break;

                    case operator_minus:
                        values.back() = va - vb;
                        break;

                    case operator_product:
                        values.back() = va * vb;
                        break;

                    case operator_slash:
                        if (iszero(vb)) {
                            REPORT_ERROR(this) << "divide by zero";
                        }
                        values.back() = va / vb;
                        break;
                    case operator_percent:
                        if (iszero(vb)) {
                            REPORT_ERROR(this) << "remainder from division by zero";
                        }
                        values.back() = FLOAT_FUNC_NAME(remainder)(quad_type(va), quad_type(vb));
                        break;

                    case operator_dbl_percent:
                        if (iszero(vb)) {
                            REPORT_ERROR(this) << "remainder from division by zero";
                        }
                        values.back() = FLOAT_FUNC_NAME(remainder)(quad_type(va), quad_type(vb));
                        break;
                    case operator_bitxor:
                        values.back() = FLOAT_FUNC_NAME(pow)(quad_type(va), quad_type(vb));
                        break;

                    default:
                        REPORT_ERROR(this) << "unknown binary float operator";
                }
                }
                break;

            default:
                break;
        }
    }

    LOG_DEBUG(LOG_EVALUATE) << "formula: " << values.back();
    return values.back();
}

} // namespace postrisc
