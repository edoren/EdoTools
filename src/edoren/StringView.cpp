#include <edoren/StringView.hpp>

#include <edoren/String.hpp>

namespace edoren {

StringView::StringView(const char8_t* utf8String) : StringView(reinterpret_cast<const char*>(utf8String)) {}

StringView::StringView(const String& string) : m_data(string.getData()), m_size(string.getDataSize()) {}

StringView& StringView::operator=(const String& right) {
    m_data = right.getData();
    m_size = right.getDataSize();
    return *this;
}

StringView& StringView::operator=(const char8_t* right) {
    return operator=(reinterpret_cast<const char*>(right));
}

}  // namespace edoren

#ifdef EDOTOOLS_NLOHMANN_JSON_SUPPORT

namespace nlohmann {

void adl_serializer<edoren::StringView>::to_json(json& j, const edoren::StringView& s) {
    auto tmp = std::string(s.getData(), s.getData() + s.getDataSize());
    j = std::move(tmp);
}

}  // namespace nlohmann

#endif  // EDOTOOLS_NLOHMANN_JSON_SUPPORT
