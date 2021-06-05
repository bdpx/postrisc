#pragma once

namespace postrisc {
namespace fmt {

/*****************************************************************************
* custom output manipulators
*****************************************************************************/
template<typename T, size_t N = 2 * sizeof(T)> class hexer {
public:
    explicit hexer(T val) : v(val) {}

    template<typename D, size_t DN>
    friend std::ostream& operator<<(std::ostream& out, const hexer<D,DN> a);

private:
    T v;
};

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& out, const hexer<T, N> a)
{
    return out
        << std::right
        << std::setfill('0')
        << std::setw(N)
        << std::hex
        << (+a.v)  // + for char conversion to int
        << std::setfill(' ') // restore default space fill
        << std::dec; // restore default decimal
}

template<> class hexer<bool, 1> {
private:
    bool v;
public:
    explicit hexer(bool val) : v(val) {}
    friend std::ostream& operator<<(std::ostream& out, const hexer a) {
        out << char(a.v ? '1' : '0');
        return out;
    }
};

template<> class hexer<i128, 32> {
private:
    i128 v;
public:
    explicit hexer(i128 val) : v(val) {}
    friend std::ostream& operator<<(std::ostream& out, const hexer a) {
        return out
        << std::right
        << std::setfill('0')
        << std::setw(16)
        << std::hex
        << (u64)(a.v >> 64)
        << std::setw(16)
        << (u64)a.v
        << std::setfill(' ') // restore default space fill
        << std::dec; // restore default decimal
    }
};

template<> class hexer<u128, 32> {
private:
    u128 v;
public:
    explicit hexer(u128 val) : v(val) {}
    friend std::ostream& operator<<(std::ostream& out, const hexer a) {
        return out
        << std::right
        << std::setfill('0')
        << std::setw(16)
        << std::hex
        << (u64)(a.v >> 64)
        << std::setw(16)
        << (u64)a.v
        << std::setfill(' ') // restore default space fill
        << std::dec; // restore default decimal
    }
};

template<typename T, size_t N = 2 * sizeof(T)>
inline hexer<T, N> hex(T v) { return hexer<T, N>(v); }

template<typename T> class decer {
  private:
      T v;
  public:
      explicit decer(T _v) :v(_v) {}
      friend std::ostream& operator<<(std::ostream& out, decer a) {
          return out << std::setfill(' ') << std::dec << (+a.v);
      }
};

template<typename T> inline decer<T> dec(T v) {  return decer<T>(v); }

/***************************************************************************
* this template write binary digit with '_' delimiters between bytes
***************************************************************************/
template<typename T> class biner {
  private:
      T v;
  public:
      explicit biner(T _v) :v(_v) {}
      friend std::ostream& operator<<(std::ostream& out, biner a) {
          const size_t N = 8 * sizeof(T);

          for (int i=N-1; i>0; i--) {
              out << (char)(((a.v >> i) & 1) + '0');
              if ((i & 7) == 0)
                 out << '_';
          }
          out << (char)(((a.v) & 1) + '0');
          return out;
      }
};

template<typename T> inline biner<T> bin(T v) {  return biner<T>(v); }

template<typename T> class span {
public:
    explicit span(T const * _data, size_t _n) : data(_data), n(_n) {}
    friend std::ostream& operator<<(std::ostream& out, span const& r) {
        for (size_t i = 0; i < r.n; i++) {
            out << ' ' << r.data[i];
        }
        return out;
    }
private:
    T const* data;
    size_t n;
};

void hexdump(u8 const *data, size_t len, uintptr_t offset, std::ostream& out);

class hex_data {
public:
    hex_data(void const* _data, size_t _size, uintptr_t _offset)
        : data(_data), size(_size), offset(_offset) {}
    friend std::ostream& operator<<(std::ostream& out, const hex_data& r) {
        hexdump(reinterpret_cast<u8 const *>(r.data), r.size, r.offset, out);
        return out;
    }
private:
    void const*    data;
    size_t         size;
    uintptr_t      offset;
};

/////////////////////////////////////
//
std::ostream& flush(std::ostream& out);
std::ostream& endl(std::ostream& out);

class table {
public:
    explicit table(const char *name) : m_name(name) {}

    friend std::ostream& operator<<(std::ostream& out, const table r) {
        return
        out << std::left
            << "  "
            << std::setw(22) << r.m_name
            << " => "
            << std::right;
    }

private:
    char const * m_name;
};


class data_logger {
public:
    data_logger(void const *_data, size_t _len) : data(_data), len(_len) {}
    friend std::ostream& operator<<(std::ostream& out, const data_logger& r);
private:
    template<typename T> void dump_value(std::ostream& out) const;

private:
    void const  *data;
    size_t      len;
};

} // namespace fmt
} // namespace postrisc
