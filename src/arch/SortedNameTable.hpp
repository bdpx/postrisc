#pragma once

namespace postrisc {

/***************************************************************************
* SortedNameTable: sparse packed keyword table
***************************************************************************/
template<typename ID,
         size_t max_name_index,
         size_t max_search_size,
         size_t max_name_buffer_size>
class SortedNameTable {
public:
    const std::string_view name(ID n) const {
        return { &m_names[m_offset[n]], m_length[n] };
    }

    const std::span<const ID> sorted_range(void) const {
        return m_sorted_indexes;
    }

    std::optional<ID> parse(const std::string_view needle) const {
        const auto it = std::lower_bound(m_sorted_indexes.begin(), m_sorted_indexes.end(), needle,
            [this] (const ID id, const std::string_view test_name) {
                return name(id) < test_name;
            });

        if (it == m_sorted_indexes.end())
            return {};

        const ID id = *it;
        if (needle != name(id))
            return {};

        return id;
    }

    std::string regex(void) const {
        std::string pattern("\\b(");
        bool first = true;
        for (const auto id : m_sorted_indexes) {
            if (!first) {
                pattern += "|";
            }
            pattern += name(id);
            first = false;
        }
        pattern += ")\\b";
        return pattern;
    }

    template<typename NamesFactory,
             typename WholeSizeFactory,
             typename DefaultNameFactory>
    friend constexpr auto createSortedNameTable(
                  NamesFactory factory,
                  WholeSizeFactory wholeSizeFactory,
                  DefaultNameFactory defaultNameFactory) -> auto;

private:
    std::array<u16,   max_name_index>       m_offset;
    std::array<u8,    max_name_index>       m_length;
    std::array<ID,    max_search_size>      m_sorted_indexes;
    std::array<char,  max_name_buffer_size> m_names;
};

/***************************************************************************
* SortedNameTable fabric
***************************************************************************/
template<typename ID> class NameInfo {
public:
    ID                 id;
    std::string_view   name;
};

template<typename NamesFactory,
         typename WholeSizeFactory,
         typename DefaultNameFactory>
constexpr auto createSortedNameTable(
    NamesFactory factory,
    WholeSizeFactory wholeSizeFactory,
    DefaultNameFactory defaultNameFactory) -> auto
{
    constexpr auto names = factory();
    constexpr size_t N = names.size();
    using ID = decltype(names[0].id);

    constexpr size_t max_offset_size = wholeSizeFactory(N);
    constexpr std::string_view default_name = defaultNameFactory();

    constexpr const size_t max_name_buffer_size = [names, default_name]() constexpr {
        size_t size = default_name.size();
        for (const auto& x : names) {
            size += x.name.size();
        }
        return size;
    }();

    constexpr const size_t max_name_index = [names, max_offset_size]() constexpr {
        size_t max_index = 0;
        for (const auto& x : names) {
            const auto index = x.id;
            max_index = std::max<size_t>(max_index, static_cast<size_t>(index));
        }
        return std::max(max_offset_size, max_index + 1);
    }();

    SortedNameTable<ID, max_name_index, N, max_name_buffer_size> table;

    auto arr = names;
    std::sort(arr.begin(), arr.end(), [](const auto& l, const auto& r) {
        return l.name < r.name;
    });

    for (auto& x : table.m_offset) {
        x = 0;
    }
    for (auto& x : table.m_length) {
        x = default_name.size();
    }

    size_t offset = 0;
    for (const char chr : default_name) {
        table.m_names[offset++] = chr;
    }

    for (size_t i = 0; i < arr.size(); i++) {
        const ID id = arr[i].id;
        table.m_sorted_indexes[i] = id;
        table.m_offset[id] = offset;
        table.m_length[id] = arr[i].name.size();
        for (const char chr : arr[i].name) {
            table.m_names[offset++] = chr;
        }
    }

    return table;
}

/***************************************************************************
* SortedNameTable consumer
***************************************************************************/
template<typename E> class EnumHelper {
public:
    static const std::string_view name(E value);
    static std::optional<E> parse(const std::string_view name);
    static const std::span<const E> sorted_range(void);
    static std::string regex(void);
};

} // namespace postrisc
