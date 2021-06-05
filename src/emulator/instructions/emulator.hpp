#include "util/quadruple.hpp"
#include "dump/formatter_txt.hpp"
#include "core.hpp"

namespace postrisc {

class CRegisterNameFormatter {
public:
    CRegisterNameFormatter(const Decoder& decoder, unsigned regnum)
        : name(decoder.gpr_names[regnum]) {}
    friend std::ostream& operator << (std::ostream&  os, const CRegisterNameFormatter regname);

private:
    const std::string& name;
};

std::ostream&
operator << (std::ostream& os, const CRegisterNameFormatter regname)
{
    return os << std::left
              << std::setfill(' ')
              << std::setw(8)
              << regname.name
              << std::right;
}

//****************************************************************************************
// write pseudo-instruction
//****************************************************************************************
enum formatter_flag {
    FORMAT_BINARY       = 0x001000,
    FORMAT_FLOAT        = 0x002000,
    FORMAT_UNSIGNED     = 0x004000,
    FORMAT_SIGNED       = 0x008000,
    FORMAT_VECTOR       = 0x010000,
    FORMAT_CHARACTER    = 0x020000,
    FORMAT_HEXADECIMAL  = 0x040000,
    FORMAT_SPECIAL      = 0x080000,
    FORMAT_COMMAND      = 0x100000,
    FORMAT_SIZE_MASK    = 0x000fff,
};

// (m)|(s(x))|([v](f|e|u|i|x|b)[digit])
// s, sx
// f32, f64, f128, vf32, vf64
// i8, i16, i32, i64, vi8, vi16, vi32, vi64
// u8, u16, u32, u64, vu8, vu16, vu32, vu64
// x8, x16, x32, x64, vx8, vx16, vx32, vx64
// b8, b16, b32, b64, vb8, vb16, vb32, vb64
// c, vc
// m
static unsigned ParseFormatting(std::istream& input)
{
    char chr;
    unsigned flags = 0;
    if (!input.get(chr)) {
        LOG_TRACE(LOG_EMULATOR) << "no formatter" << fmt::endl;
        return flags;
    }

    if (chr == 'm') {
        LOG_TRACE(LOG_EMULATOR) << "command" << fmt::endl;
        return FORMAT_COMMAND;
    }

    if (chr == 's') {
        LOG_TRACE(LOG_EMULATOR) << "special register formatter" << fmt::endl;
        flags = FORMAT_SPECIAL;
        if (input.get(chr)) {
            if (chr == 'x') {
                flags |= FORMAT_HEXADECIMAL;
                LOG_TRACE(LOG_EMULATOR) << "specail hexadecimal" << fmt::endl;
            } else {
                LOG_TRACE(LOG_EMULATOR) << "usual special formatter" << fmt::endl;
                input.putback(chr);
            }
        }
        return flags;
    }

    if (chr == 'v') {
        flags |= FORMAT_VECTOR;
        if (!input.get(chr)) {
            LOG_TRACE(LOG_EMULATOR) << "just vector" << fmt::endl;
            return flags;
        }
    }
    switch (chr) {
        case 'c':  flags |= FORMAT_CHARACTER;    break;
        case 'i':  flags |= FORMAT_SIGNED;       break;
        case 'u':  flags |= FORMAT_UNSIGNED;     break;
        case 'x':  flags |= FORMAT_HEXADECIMAL;  break;
        case 'b':  flags |= FORMAT_BINARY;       break;
        case 'f':  flags |= FORMAT_FLOAT;        break;
        default:
            LOG_TRACE(LOG_EMULATOR) << "invalid formatting character: " << chr << fmt::endl;
            input.putback(chr);
            return flags;
    }
    if (flags & FORMAT_CHARACTER) {
        LOG_TRACE(LOG_EMULATOR) << "just character" << fmt::endl;
        return flags;
    }

    unsigned size = 0;
    for (;;) {
        if (!input.get(chr))
            break;
        if (!isdigit(chr)) {
            input.putback(chr);
            break;
        }
        size = size * 10 + (chr - '0');
        if (size > 128) {
            LOG_TRACE(LOG_EMULATOR) << "size too big: " << size << fmt::endl;
            break;
        }
    }
    LOG_TRACE(LOG_EMULATOR) << "size=" << size << fmt::endl;
    return flags | size;
}

template<class T>
void
OutputVector(std::ostream& out, const CGeneralRegister& b)
{
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::showpos;

    if constexpr (!std::numeric_limits<T>::is_integer)
       out << std::setprecision(std::numeric_limits<T>::digits10) << std::scientific;

    const size_t n = CGeneralRegister::size<T>();
    const unsigned WIDTH = 80 / n;
    out << '{';
    for (size_t i = 0; i < n; i++) {
        out << std::setw(WIDTH) << (+b.vector<T>(i)) << fmt::flush;
    }
    out << ' ' << '}';
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::noshowpos;
}

// %%, %reg.fmt %r23.vi1 %ip
CStatus
Core::OutputStringFormatting(std::ostream& stream, std::istream& input)
{
    // %%
    char chr;
    if (!input.get(chr)) {
        stream << '%' << fmt::flush;
        return CStatus(CStatus::continue_execution);
    }
    if (chr == '%') {
        stream << chr << fmt::flush;
        return CStatus(CStatus::continue_execution);
    } else {
        input.putback(chr);
    }

    unsigned const flags = ParseFormatting(input);
    unsigned const size = flags & FORMAT_SIZE_MASK;

    // (register-name)
    if (!input.get(chr)) {
        LOG_TRACE(LOG_EMULATOR) << "missing opening bracket" << fmt::endl;
        return CStatus(CStatus::data_debug);
    }
    if (chr != '(') {
        LOG_TRACE(LOG_EMULATOR) << "invalid opening bracket" << fmt::endl;
        return CStatus(CStatus::data_debug);
    }

    if (!input.get(chr)) {
        LOG_TRACE(LOG_EMULATOR) << "missing first name symbol: " << chr << fmt::endl;
        return CStatus(CStatus::data_debug);
    }
    if (!std::isalpha(chr)) {
        LOG_TRACE(LOG_EMULATOR) << "invalid first name symbol: " << chr << fmt::endl;
        input.putback(chr);
        return CStatus(CStatus::data_debug);
    }
    std::string reg_name;
    reg_name.push_back(chr);
    for (;;) {
        if (!input.get(chr)) {
            LOG_TRACE(LOG_EMULATOR) << "name stopped: " << reg_name << fmt::endl;
            return CStatus(CStatus::data_debug);
        }
        if (std::isalnum(chr)) {
            reg_name.push_back(std::tolower(chr));
        } else {
            input.putback(chr);
            break;
        }
        if (reg_name.size() > 32) {
            LOG_TRACE(LOG_EMULATOR) << "too long register name: " << reg_name << fmt::endl;
            return CStatus(CStatus::data_debug);
        }
    }

    if (!input.get(chr)) {
        LOG_TRACE(LOG_EMULATOR) << "missing closing bracket" << fmt::endl;
        return CStatus(CStatus::data_debug);
    }
    if (chr != ')') {
        LOG_TRACE(LOG_EMULATOR) << "invalid closing bracket" << fmt::endl;
        return CStatus(CStatus::data_debug);
    }

    LOG_TRACE(LOG_EMULATOR) << "register name: " << reg_name << fmt::endl;

    if (flags & FORMAT_COMMAND) {
        if (reg_name == "dump") {
            DumpFormatterTxt formatter(stream);
            dump_state(formatter);
            return CStatus(CStatus::continue_execution);
        }
        return CStatus(CStatus::data_debug);
    }

    if (flags & FORMAT_SPECIAL) {
        const auto it = GetDecoder().m_SpecialRegisters.find(reg_name);
        if (it == GetDecoder().m_SpecialRegisters.end()) {
            LOG_TRACE(LOG_EMULATOR) << "invalid special register name: " << reg_name << fmt::endl;
            return CStatus(CStatus::data_debug);
        }
        unsigned const regnum = it->second;
        DumpFormatterTxt formatter(stream);
        return OutputSpecialRegister(formatter, regnum);
    }

    // general registers
    const auto it = GetDecoder().m_GeneralRegisters.find(reg_name);
    if (it == GetDecoder().m_GeneralRegisters.end()) {
        LOG_TRACE(LOG_EMULATOR) << "invalid general register name: " << reg_name << fmt::endl;
        return CStatus(CStatus::data_debug);
    }

    unsigned const regnum = it->second;
    LOG_TRACE(LOG_EMULATOR) << "general register name: " << reg_name << " " << regnum << fmt::endl;
    ROTATE(ria_, regnum)
    LOG_TRACE(LOG_EMULATOR) << "rotation: " << ria_result << fmt::endl;
    const CGeneralRegister & a = rget( ria_result );

    LOG_TRACE(LOG_EMULATOR) << "flags=" << flags << ", size=" << size << fmt::endl;

    LOG_TRACE(LOG_EMULATOR) << CRegisterNameFormatter(GetDecoder(), regnum);

    if (flags & FORMAT_CHARACTER) {
        stream << '\''
               << a.template scalar<char>()
               << '\'';
        return CStatus(CStatus::continue_execution);
    }

    if (flags & FORMAT_SIGNED) {
        if (flags & FORMAT_VECTOR) {
            switch (size) {
                case 8:    OutputVector<int8_t>(stream, a);      break;
                case 16:   OutputVector<int16_t>(stream, a);     break;
                case 32:   OutputVector<int32_t>(stream, a);     break;
                case 64:   OutputVector<int64_t>(stream, a);     break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid signed vector size: " << size;
                    return CStatus(CStatus::data_debug);
          }
        } else {
            switch (size) {
                case 8:    stream << +a.template scalar<int8_t>();      break;
                case 16:   stream << +a.template scalar<int16_t>();     break;
                case 32:   stream << +a.template scalar<int32_t>();     break;
                case 64:   stream << +a.template scalar<int64_t>();     break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid integer point size: " << size << fmt::endl;
                    return CStatus(CStatus::data_debug);
            }
        }
        return CStatus(CStatus::continue_execution);
    }

    if (flags & FORMAT_UNSIGNED) {
        if (flags & FORMAT_VECTOR) {
            switch (size) {
                case 8:    OutputVector<uint8_t>(stream, a);      break;
                case 16:   OutputVector<uint16_t>(stream, a);     break;
                case 32:   OutputVector<uint32_t>(stream, a);     break;
                case 64:   OutputVector<uint64_t>(stream, a);     break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid signed vector size: " << size;
                    return CStatus(CStatus::data_debug);
          }
        } else {
            switch (size) {
                case 8:    stream << +a.template scalar<uint8_t>();      break;
                case 16:   stream << +a.template scalar<uint16_t>();     break;
                case 32:   stream << +a.template scalar<uint32_t>();     break;
                case 64:   stream << +a.template scalar<uint64_t>();     break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid integer point size: " << size << fmt::endl;
                    return CStatus(CStatus::data_debug);
            }
        }
        return CStatus(CStatus::continue_execution);
    }

    if (flags & FORMAT_HEXADECIMAL) {
        if (flags & FORMAT_VECTOR) {
            LOG_TRACE(LOG_EMULATOR) << "invalid signed vector size: " << size;
            return CStatus(CStatus::data_debug);
        } else {
            switch (size) {
                case 8:   stream << fmt::hex(a.template scalar<uint8_t>());      break;
                case 16:  stream << fmt::hex(a.template scalar<uint16_t>());     break;
                case 32:  stream << fmt::hex(a.template scalar<uint32_t>());     break;
                case 64:  stream << fmt::hex(a.template scalar<uint64_t>());     break;
                case 128: stream << a;    break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid hexadecimal size: " << size << fmt::endl;
                    return CStatus(CStatus::data_debug);
            }
        }
        return CStatus(CStatus::continue_execution);
    }

    if (flags & FORMAT_BINARY) {
        if (flags & FORMAT_VECTOR) {
            LOG_TRACE(LOG_EMULATOR) << "invalid binary vector";
            return CStatus(CStatus::data_debug);
        } else {
            switch (size) {
                case 8:   stream << fmt::bin(a.template scalar<uint8_t>());      break;
                case 16:  stream << fmt::bin(a.template scalar<uint16_t>());     break;
                case 32:  stream << fmt::bin(a.template scalar<uint32_t>());     break;
                case 64:  stream << fmt::bin(a.template scalar<uint64_t>());     break;
                default:
                    LOG_TRACE(LOG_EMULATOR) << "invalid binary size: " << size << fmt::endl;
                    return CStatus(CStatus::data_debug);
            }
        }
        return CStatus(CStatus::continue_execution);
    }

    if (flags & FORMAT_FLOAT) {
        if (flags & FORMAT_VECTOR) {
          switch (size) {
              case 32:   OutputVector<float>(stream, a);      break;
              case 64:   OutputVector<double>(stream, a);     break;
              default:
                  LOG_TRACE(LOG_EMULATOR) << "invalid floating point vector size: " << size;
                  return CStatus(CStatus::data_debug);
          }
        } else {
          switch (size) {
              case 32:   stream << a.template scalar<float>();      break;
              case 64:   stream << a.template scalar<double>();     break;
              case 128:  stream << a.template scalar<quadruple>();  break;
              default:
                  LOG_TRACE(LOG_EMULATOR) << "invalid floating point size: " << size;
                  return CStatus(CStatus::data_debug);
          }
        }
        return CStatus(CStatus::continue_execution);
    }

    return CStatus(CStatus::data_debug);
}

/***************************************************************************
* this function print string from emulator
***************************************************************************/
CStatus
Core::OutputString(uint64_t address)
{
    util::cout_streamer streamer;
    std::ostream & stream = streamer.stream();
    std::string string;
    char chr;
    for (;;) {
        CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(chr), &chr));
        ++address;
        if (chr == '\0')
            break;
        string.push_back(chr);
        if (string.size() > 256) {
            LOG_TRACE(LOG_EMULATOR) << "too long output string";
            return CStatus(CStatus::data_debug);
        }
    }
    if (string.empty() || string.back() != '\n') {
        string.push_back('\n');
    }

    std::istringstream input(string);
    while (input.get(chr)) {
        if (chr == '%') {
            CHECK_STATUS(OutputStringFormatting(stream, input));
        } else {
            stream << chr << fmt::flush;
        }
    }
    return FinalizeInstruction();
}

} //namespace postrisc
