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
