#pragma once

namespace postrisc {

class Bundle;

/****************************************************************************
* segment types - powers of 2 for masking
***************************************************************************/
enum class ESectionKind { // flags/mask
    text       = 1,
    rodata     = 2,
    data       = 4
};

enum ESectionId : u32 { // numbers
    ESectionId_code   = 0,
    ESectionId_rodata = 1,
    ESectionId_data   = 2,
};

inline ESectionKind operator|(ESectionKind a, ESectionKind b) { return ESectionKind(int(a) | int(b)); }
inline bool operator&(ESectionKind a, ESectionKind b) { return (int(a) & int(b)) != 0; }

class ProgramSection {
public:
    ProgramSection(u32 id, ESectionKind type) : m_Id(id), m_Kind(type) {}
    ~ProgramSection(void) {}

    size_t size(void) const { return image.size(); }
    void clear(void) { image.clear(); }
    void reserve(size_t size) { image.reserve(size); }
    char back(void) const { return image.back(); }

    void write(char val);
    void write(u8 val);
    void write(u16 val);
    void write(u32 val);
    void write(u64 val);
    void write(u128 val);

    void write(i8   val) { write(static_cast<u8  >(val)); }
    void write(i16  val) { write(static_cast<u16 >(val)); }
    void write(i32  val) { write(static_cast<u32 >(val)); }
    void write(i64  val) { write(static_cast<u64 >(val)); }
    void write(i128 val) { write(static_cast<u128>(val)); }

    void write(const Bundle& bundle);
    void write(native::f32 x);
    void write(native::f64 x);
    void write(native::f128 x);
    void align(size_t alignment);
    u64 ComputeIpRelative(i64 ip, u64 offset) const;
    u64 ComputeGlobalOffset(u64 offset) const;
    void write(std::ostream& out) const;
    void print(const char name[], std::ostream& out) const;
    const char *GetString(size_t offset) const;
    u32 GetId(void) const { return m_Id; }
    ESectionKind GetSectionType(void) const { return m_Kind; }

public:
    size_t global_offset = 0;  // offset in image
    size_t global_size = 0;    // aligned final size

private:
    u32          m_Id = 0;    // section uniq id
    ESectionKind m_Kind;  // section type

    std::vector<char>   image;
};

} // namespace postrisc
