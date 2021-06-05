#pragma once

namespace postrisc {
namespace fmt {

/*****************************************************************************
* custom output manipulators
*****************************************************************************/
template<class T, size_t N = 2 * sizeof(T)> class hexer {
  public:
      hexer(T val) : v(val) {}

      template<class D, size_t DN> friend std::ostream & operator << (std::ostream & out, const hexer<D,DN> a);

  private:
      T v;
};

template<class T, size_t N>
std::ostream & operator << (std::ostream & out, const hexer<T, N> a)
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
    hexer(bool val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        out << char(a.v ? '1' : '0');
        return out;
    }
};

template<class T, size_t N = 2 * sizeof(T)> inline hexer<T,N> hex(T v) {  return hexer<T,N>(v); }

template<class T> class decer {
  private:
      T v;
  public:
      decer(T _v) :v(_v) {}
      friend std::ostream& operator << (std::ostream & out, decer a)
      {
          return out << std::setfill(' ') << std::dec << (+a.v);
      }
};

template<class T> inline decer<T> dec(T v) {  return decer<T>(v); }

/***************************************************************************
* this template write binary digit with '_' delimiters between bytes
***************************************************************************/
template<class T> class biner {
  private:
      T v;
  public:
      biner(T _v) :v(_v) {}
      friend std::ostream& operator << (std::ostream& out, biner a)
      {
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

template<class T> inline biner<T> bin(T v) {  return biner<T>(v); }

template<class T> class span {
public:
    explicit span(T const * _data, size_t _n) : data(_data), n(_n) {}
    friend std::ostream& operator << (std::ostream& out, span const& r)
    {
        for (size_t i = 0; i < r.n; i++) {
            out << ' ' << r.data[i];
        }
        return out;
    }
private:
    T const * data;
    size_t n;
};

/////////////////////////////////////
//
inline std::ostream & flush(std::ostream & out)
{
    // out << std::flush;
    return out;
}

ALWAYS_INLINE std::ostream & endl(std::ostream & out)
{
    return out << '\n';
}

class table {
public:
    table(const char * name) : m_name(name) {}

    friend std::ostream & operator << (std::ostream & out, const table r)
    {
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

} // namespace fmt
} // namespace postrisc
