#pragma once

#include <vector>

namespace postrisc {

class Decoder;

class LabelMarker {
public:
    LabelMarker(size_t size);
    ~LabelMarker(void);

    void MarkLabels(const Decoder& decoder, const CBundle bnd[], size_t codesize, size_t filesize);
    bool is_label(size_t offset) const;

private:
    template<class CMD>
    void MarkInstructionLabels(const Decoder& decoder, uint64_t ip, const CMD cmd);
    template<class T>
    void SetLabel(const Decoder& decoder, const T cmd, const uint64_t ip, const uint64_t pos);

private:
    std::vector<bool> bitmap;
};

} // namespace postrisc
