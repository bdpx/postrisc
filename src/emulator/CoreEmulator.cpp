#include "util/quadruple.hpp"
#include "Core.hpp"

namespace postrisc {

class RegisterNameFormatter {
public:
    RegisterNameFormatter(unsigned regnum_)
        : regnum(regnum_) {}
    friend std::ostream& operator<<(std::ostream& os, const RegisterNameFormatter regname);

private:
    unsigned regnum;
};

std::ostream& operator<<(std::ostream& os, const RegisterNameFormatter regname)
{
    return os << std::left
              << std::setfill(' ')
              << std::setw(8)
              << Decoder::GetGprName(regname.regnum)
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
        LOG_DEBUG(LOG_EMULATOR) << "no formatter" << fmt::endl;
        return flags;
    }

    if (chr == 'm') {
        LOG_DEBUG(LOG_EMULATOR) << "command" << fmt::endl;
        return FORMAT_COMMAND;
    }

    if (chr == 's') {
        LOG_DEBUG(LOG_EMULATOR) << "special register formatter" << fmt::endl;
        flags = FORMAT_SPECIAL;
        if (input.get(chr)) {
            if (chr == 'x') {
                flags |= FORMAT_HEXADECIMAL;
                LOG_DEBUG(LOG_EMULATOR) << "specail hexadecimal" << fmt::endl;
            } else {
                LOG_DEBUG(LOG_EMULATOR) << "usual special formatter" << fmt::endl;
                input.putback(chr);
            }
        }
        return flags;
    }

    if (chr == 'v') {
        flags |= FORMAT_VECTOR;
        if (!input.get(chr)) {
            LOG_DEBUG(LOG_EMULATOR) << "just vector" << fmt::endl;
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
            LOG_DEBUG(LOG_EMULATOR) << "invalid formatting character: " << chr << fmt::endl;
            input.putback(chr);
            return flags;
    }
    if (flags & FORMAT_CHARACTER) {
        LOG_DEBUG(LOG_EMULATOR) << "just character" << fmt::endl;
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
            LOG_DEBUG(LOG_EMULATOR) << "size too big: " << size << fmt::endl;
            break;
        }
    }
    LOG_DEBUG(LOG_EMULATOR) << "size=" << size << fmt::endl;
    return flags | size;
}

template<typename T>
void OutputVector(std::ostream& out, const GeneralRegister& b)
{
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::showpos;

    if constexpr (!std::numeric_limits<T>::is_integer)
       out << std::setprecision(std::numeric_limits<T>::digits10 + 1) << std::scientific;

    const size_t n = GeneralRegister::size<T>();
    const unsigned WIDTH = 80 / n;
    out << '{';
    for (size_t i = 0; i < n; i++) {
        out << std::setw(WIDTH) << (+b.vector<T>(i));
    }
    out << ' ' << '}' << fmt::flush;
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::noshowpos;
}

template<typename T>
void OutputScalar(std::ostream& out, const T b)
{
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::showpos;

    if constexpr (!std::numeric_limits<T>::is_integer)
       out << std::setprecision(std::numeric_limits<T>::digits10 + 1) << std::scientific;

    out << b;
    if constexpr (std::numeric_limits<T>::is_integer && std::numeric_limits<T>::is_signed)
       out << std::noshowpos;
}

// %%, %reg.fmt %r23.vi1 %ip
Result Core::OutputStringFormatting(AddressSpace& as, std::ostream& stream, std::istream& input)
{
    // %%
    char chr;
    if (!input.get(chr)) {
        stream << '%' << fmt::flush;
        return Result::continue_execution;
    }
    if (chr == '%') {
        stream << chr << fmt::flush;
        return Result::continue_execution;
    } else {
        input.putback(chr);
    }

    unsigned const flags = ParseFormatting(input);
    unsigned const size = flags & FORMAT_SIZE_MASK;

    // (register-name)
    if (!input.get(chr)) {
        LOG_DEBUG(LOG_EMULATOR) << "missing opening bracket" << fmt::endl;
        return Result::data_debug;
    }
    if (chr != '(') {
        LOG_DEBUG(LOG_EMULATOR) << "invalid opening bracket" << fmt::endl;
        return Result::data_debug;
    }

    if (!input.get(chr)) {
        LOG_DEBUG(LOG_EMULATOR) << "missing first name symbol: " << chr << fmt::endl;
        return Result::data_debug;
    }
    if (!std::isalpha(chr)) {
        LOG_DEBUG(LOG_EMULATOR) << "invalid first name symbol: " << chr << fmt::endl;
        input.putback(chr);
        return Result::data_debug;
    }
    std::string reg_name;
    reg_name.push_back(chr);
    for (;;) {
        if (!input.get(chr)) {
            LOG_DEBUG(LOG_EMULATOR) << "name stopped: " << reg_name << fmt::endl;
            return Result::data_debug;
        }
        if (std::isalnum(chr)) {
            reg_name.push_back(std::tolower(chr));
        } else {
            input.putback(chr);
            break;
        }
        if (reg_name.size() > 32) {
            LOG_DEBUG(LOG_EMULATOR) << "too long register name: " << reg_name << fmt::endl;
            return Result::data_debug;
        }
    }

    if (!input.get(chr)) {
        LOG_DEBUG(LOG_EMULATOR) << "missing closing bracket" << fmt::endl;
        return Result::data_debug;
    }
    if (chr != ')') {
        LOG_DEBUG(LOG_EMULATOR) << "invalid closing bracket" << fmt::endl;
        return Result::data_debug;
    }

    LOG_DEBUG(LOG_EMULATOR) << "register name: " << reg_name << fmt::endl;

    if (flags & FORMAT_COMMAND) {
        if (reg_name == "dump") {
            DumpFormatter formatter(as.get_decoder(), stream, false);
            dump_state(formatter);
            return Result::continue_execution;
        }
        return Result::data_debug;
    }

    if (flags & FORMAT_SPECIAL) {
        int const regnum = Decoder::GetSprIndex(reg_name.c_str());
        if (regnum < 0) {
            LOG_DEBUG(LOG_EMULATOR) << "invalid special register name: " << reg_name << fmt::endl;
            return Result::data_debug;
        }
        DumpFormatter formatter(as.get_decoder(), stream, false);
        return OutputSpecialRegister(formatter, regnum);
    }

    // general registers
    int const regnum = Decoder::GetGprIndex(reg_name.c_str());
    if (regnum < 0) {
        LOG_DEBUG(LOG_EMULATOR) << "invalid general register name: " << reg_name << fmt::endl;
        return Result::data_debug;
    }

    LOG_DEBUG(LOG_EMULATOR) << "general register name: " << reg_name << " " << regnum << fmt::endl;
    unsigned ria_result, temp;
    ROTATE_REG(*this, ria_result, temp, regnum)
    LOG_DEBUG(LOG_EMULATOR) << "rotation: " << ria_result << fmt::endl;
    const GeneralRegister & a = rget( ria_result );

    LOG_DEBUG(LOG_EMULATOR) << "flags=" << flags << ", size=" << size << fmt::endl;

    LOG_DEBUG(LOG_EMULATOR) << RegisterNameFormatter(regnum);

    if (flags & FORMAT_CHARACTER) {
        stream << '\''
               << a.template scalar<char>()
               << '\'';
        return Result::continue_execution;
    }

    if (flags & FORMAT_SIGNED) {
        if (flags & FORMAT_VECTOR) {
            switch (size) {
                case 8:    OutputVector<i8>(stream, a);      break;
                case 16:   OutputVector<i16>(stream, a);     break;
                case 32:   OutputVector<i32>(stream, a);     break;
                case 64:   OutputVector<i64>(stream, a);     break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid signed vector size: " << size;
                    return Result::data_debug;
          }
        } else {
            switch (size) {
                case 8:    stream << +a.template scalar<i8>();      break;
                case 16:   stream << +a.template scalar<i16>();     break;
                case 32:   stream << +a.template scalar<i32>();     break;
                case 64:   stream << +a.template scalar<i64>();     break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid integer point size: " << size << fmt::endl;
                    return Result::data_debug;
            }
        }
        return Result::continue_execution;
    }

    if (flags & FORMAT_UNSIGNED) {
        if (flags & FORMAT_VECTOR) {
            switch (size) {
                case 8:    OutputVector<u8>(stream, a);      break;
                case 16:   OutputVector<u16>(stream, a);     break;
                case 32:   OutputVector<u32>(stream, a);     break;
                case 64:   OutputVector<u64>(stream, a);     break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid signed vector size: " << size;
                    return Result::data_debug;
          }
        } else {
            switch (size) {
                case 8:    stream << +a.template scalar<u8>();      break;
                case 16:   stream << +a.template scalar<u16>();     break;
                case 32:   stream << +a.template scalar<u32>();     break;
                case 64:   stream << +a.template scalar<u64>();     break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid integer point size: " << size << fmt::endl;
                    return Result::data_debug;
            }
        }
        return Result::continue_execution;
    }

    if (flags & FORMAT_HEXADECIMAL) {
        if (flags & FORMAT_VECTOR) {
            LOG_DEBUG(LOG_EMULATOR) << "invalid signed vector size: " << size;
            return Result::data_debug;
        } else {
            switch (size) {
                case 8:   stream << fmt::hex(a.template scalar<u8>());      break;
                case 16:  stream << fmt::hex(a.template scalar<u16>());     break;
                case 32:  stream << fmt::hex(a.template scalar<u32>());     break;
                case 64:  stream << fmt::hex(a.template scalar<u64>());     break;
                case 128: stream << a;    break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid hexadecimal size: " << size << fmt::endl;
                    return Result::data_debug;
            }
        }
        return Result::continue_execution;
    }

    if (flags & FORMAT_BINARY) {
        if (flags & FORMAT_VECTOR) {
            LOG_DEBUG(LOG_EMULATOR) << "invalid binary vector";
            return Result::data_debug;
        } else {
            switch (size) {
                case 8:   stream << fmt::bin(a.template scalar<u8>());      break;
                case 16:  stream << fmt::bin(a.template scalar<u16>());     break;
                case 32:  stream << fmt::bin(a.template scalar<u32>());     break;
                case 64:  stream << fmt::bin(a.template scalar<u64>());     break;
                default:
                    LOG_DEBUG(LOG_EMULATOR) << "invalid binary size: " << size << fmt::endl;
                    return Result::data_debug;
            }
        }
        return Result::continue_execution;
    }

    if (flags & FORMAT_FLOAT) {
        if (flags & FORMAT_VECTOR) {
          switch (size) {
              case 16:   OutputVector<native::f16>(stream, a); break;
              case 32:   OutputVector<native::f32>(stream, a); break;
              case 64:   OutputVector<native::f64>(stream, a); break;
              default:
                  LOG_DEBUG(LOG_EMULATOR) << "invalid floating point vector size: " << size;
                  return Result::data_debug;
          }
        } else {
          switch (size) {
              case 16:   OutputScalar(stream, a.template scalar<native::f16>());   break;
              case 32:   OutputScalar(stream, a.template scalar<native::f32>());   break;
              case 64:   OutputScalar(stream, a.template scalar<native::f64>());   break;
              case 128:  OutputScalar(stream, a.template scalar<native::f128>());  break;
              default:
                  LOG_DEBUG(LOG_EMULATOR) << "invalid floating point size: " << size;
                  return Result::data_debug;
          }
        }
        return Result::continue_execution;
    }

    return Result::data_debug;
}

/***************************************************************************
* this function print string from emulator
***************************************************************************/
Result Core::OutputString(AddressSpace& as, u64 address)
{
    util::cout_streamer streamer;
    std::ostream& stream = streamer.stream();
    std::string string;
    for (;;) {
        u8 chr;
        const VirtAddress va(address);
        CHECK_RESULT_ADDRESS(*this, va, as.mmu_read(*this, va, chr));
        ++address;
        if (chr == '\0')
            break;
        string.push_back(chr);
        if (string.size() > 256) {
            LOG_DEBUG(LOG_EMULATOR) << "too long output string";
            return Result::data_debug;
        }
    }
    if (string.empty() || string.back() != '\n') {
        string.push_back('\n');
    }

    std::istringstream input(string);
    char chr;
    while (input.get(chr)) {
        if (chr == '%') {
            const VirtAddress va(address);
            CHECK_RESULT_ADDRESS(*this, va, OutputStringFormatting(as, stream, input));
        } else {
            stream << chr << fmt::flush;
        }
    }
    return FinalizeInstruction();
}

} //namespace postrisc
