#pragma once

namespace postrisc {

class CBundle;

/****************************************************************************
* segment types - powers of 2 for masking
***************************************************************************/
enum class ESectionKind { // flags/mask
    text       = 1,
    rodata     = 2,
    data       = 4
};

enum ESectionId : uint32_t { // numbers
    ESectionId_code   = 0,
    ESectionId_rodata = 1,
    ESectionId_data   = 2,
};

inline ESectionKind operator | (ESectionKind a, ESectionKind b) { return ESectionKind(int(a) | int(b)); }
inline bool operator & (ESectionKind a, ESectionKind b) { return (int(a) & int(b)) != 0; }

class CProgramSection {
public:
    CProgramSection(uint32_t id, ESectionKind type);
    ~CProgramSection(void);
    size_t size(void) const;
    void clear(void);
    void reserve(size_t size);
    char back(void) const;
    void write(char val);
    void write(uint8_t val);
    void write(uint16_t val);
    void write(uint32_t val);
    void write(uint64_t val);
    void write(fpu::u128 val);
    void write(const CBundle & bundle);
    void write(float x);
    void write(double x);
    void write(quadruple x);
    void align(size_t alignment);
    uint64_t ComputeIpRelative(int64_t ip, uint64_t offset) const;
    uint64_t ComputeGlobalOffset(uint64_t offset) const;
    uint64_t ComputeNextIp(void) const;
    void write(std::ostream& out) const;
    void print(const char name[], std::ostream& out) const;
    const char * GetString(size_t offset) const;
    size_t GetNextGlobalOffset(void) const;
    uint32_t GetId(void) const { return m_Id; }
    ESectionKind GetSectionType(void) const { return m_Kind; }

public:
    size_t              global_offset;  // offset in image
    size_t              global_size;    // aligned final size

private:
    uint32_t            m_Id;           // section uniq id
    ESectionKind        m_Kind;         // section type

    std::vector<char>   image;
};

} // namespace postrisc
