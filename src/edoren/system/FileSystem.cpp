#include <edoren/system/FileSystem.hpp>

#include <cstring>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#if PLATFORM_IS(PLATFORM_WINDOWS)
    #include <windows.h>
#elif PLATFORM_IS(PLATFORM_LINUX | PLATFORM_MACOS | PLATFORM_IOS | PLATFORM_ANDROID)
    #include <unistd.h>
#endif

#include <edoren/container/Vector.hpp>
#include <edoren/util/Platform.hpp>

#define PATH_MAX_LENGTH 256

namespace edoren {

namespace {

String sExecutableDirectory;
String sCurrentWorkingDirectory;
Vector<String> sSearchPaths;

}  // namespace

namespace filesystem {

// TODO Improve performance per platform
bool FileExists(StringView filename) {
    if (IsAbsolutePath(filename) && std::filesystem::exists(std::filesystem::path(filename.getData()))) {
        return true;
    } else {
        for (const String& path : GetSearchPaths()) {
            String filePath = Join(path, filename);
            if (std::filesystem::exists(std::filesystem::path(filePath.getData()))) {
                return true;
            }
        }
    }
    return false;
}

String FindFile(StringView filename) {
    if (IsAbsolutePath(filename) && std::filesystem::exists(std::filesystem::path(filename.getData()))) {
        return filename;
    } else {
        for (const String& path : GetSearchPaths()) {
            String filePath = Join(path, filename);
            if (std::filesystem::exists(std::filesystem::path(filePath.getData()))) {
                return filePath;
            }
        }
    }
    return String();
}

bool LoadFileData(StringView filename, Vector<uint8_t>& dest) {
    String filenameCpy = filename;
    filenameCpy.replace('\\', GetOsSeparator());
    filenameCpy.replace('/', GetOsSeparator());

    String foundFilePath;
    for (const String& path : GetSearchPaths()) {
        String filePath = Join(path, filenameCpy);
        if (std::filesystem::exists(std::filesystem::path(filePath.getData()))) {
            foundFilePath = std::move(filePath);
            break;
        }
    }

    if (foundFilePath.isEmpty()) {
        return false;
    }

    // Open the file:
    std::ifstream file(foundFilePath.toUtf8(), std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // Get its size:
    size_t fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Reserve capacity
    dest.clear();
    dest.resize(fileSize);

    file.read(reinterpret_cast<char*>(dest.data()), fileSize);

    return file.gcount() == fileSize;
}

bool LoadFileData(StringView filename, String& dest) {
    Vector<uint8_t> out;
    bool success = LoadFileData(filename, out);
    if (success) {
        auto* begin = reinterpret_cast<char*>(out.data());
        dest = String::FromUtf8(begin, begin + out.size());
    }
    return success;
}

char GetOsSeparator() {
#if PLATFORM_IS(PLATFORM_WINDOWS)
    return '\\';
#else
    return '/';
#endif
}

const String& GetExecutableDirectory() {
    if (sExecutableDirectory.isEmpty()) {
#if PLATFORM_IS(PLATFORM_WINDOWS)
        WCHAR path[PATH_MAX_LENGTH];
        GetModuleFileNameW(NULL, path, PATH_MAX_LENGTH);
        sExecutableDirectory = path;
        sExecutableDirectory = sExecutableDirectory.subString(0, sExecutableDirectory.findLastOf("\\"));
#elif PLATFORM_IS(PLATFORM_LINUX | PLATFORM_MACOS)
        char result[PATH_MAX] = {};
        size_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            sExecutableDirectory = dirname(result);
        }
#elif PLATFORM_IS(PLATFORM_MACOS)
        static_assert(true, "Missing implementation");
#elif PLATFORM_IS(PLATFORM_IOS | PLATFORM_ANDROID)
        static_assert(true, "Missing implementation");
#endif
        sExecutableDirectory = filesystem::NormalizePath(sExecutableDirectory);
    }
    return sExecutableDirectory;
}

const String& GetCurrentWorkingDirectory() {
    if (!sCurrentWorkingDirectory.isEmpty()) {
        return sCurrentWorkingDirectory;
    }
    String ret;
#if PLATFORM_IS(PLATFORM_WINDOWS)
    DWORD buffer_length = PATH_MAX_LENGTH;
    Vector<std::remove_pointer_t<LPWSTR>> buffer;
    while (true) {
        buffer.resize(buffer_length + 1);
        DWORD num_characters = GetCurrentDirectoryW(buffer_length, buffer.data());
        if (num_characters > 0) {
            if (buffer[num_characters - 1] != L'\\') {
                buffer[num_characters++] = L'\\';
                buffer[num_characters] = L'\0';
            }
            ret = String::FromWide(buffer.data(), buffer.data() + num_characters);
            buffer.clear();
            break;
        }
        buffer.clear();
        buffer_length *= 2;
    }
#elif PLATFORM_IS(PLATFORM_LINUX | PLATFORM_MACOS | PLATFORM_IOS | PLATFORM_ANDROID)
    size_t bufferLength = PATH_MAX_LENGTH;
    Vector<char> buffer;
    while (true) {
        buffer.resize(bufferLength + 1);
        char* result = nullptr;
        result = getcwd(buffer.data(), bufferLength);
        if (result != nullptr) {
            size_t numCharacters = strlen(result);
            if (numCharacters > 0 && buffer[numCharacters - 1] != '/') {
                buffer[numCharacters++] = '/';
                buffer[numCharacters] = '\0';
            }
            ret = String::FromUtf8(buffer.data(), buffer.data() + numCharacters);
            buffer.clear();
            break;
        }
        buffer.clear();
        bufferLength *= 2;
    }
#endif
    sCurrentWorkingDirectory = std::move(ret);
    return sCurrentWorkingDirectory;
}

String AbsolutePath(const String& /*path*/) {
    String ret;

    return ret;
}

String NormalizePath(const String& path) {
    bool isAbsolute = IsAbsolutePath(path);
    Vector<std::pair<const char*, const char*>> pathComps;

    auto addPathComponent = [&pathComps, isAbsolute](const char* begin, const char* end) {
        size_t seqSize = end - begin;

        // Ignore the component if the . directories
        if (seqSize == 1 && std::memcmp(begin, ".", 1) == 0) {
            return;
        }

        // If the component is a .. directory
        if (seqSize == 2 && std::memcmp(begin, "..", 2) == 0) {
            // Check if the path_comps is empty
            if (!pathComps.empty()) {
                // If the last element is a .. directory, append another one
                // if not just pop_back the last component
                auto& last = pathComps.back();
                if (!isAbsolute && (last.second - last.first) == 2 && std::memcmp(last.first, "..", 2) == 0) {
                    pathComps.emplace_back(begin, end);
                } else {
                    pathComps.pop_back();
                }
            }
            // Only add .. directories if the path is not absolute
            else if (!isAbsolute) {
                pathComps.emplace_back(begin, end);
            }
            return;
        }

        // Add the path component
        pathComps.emplace_back(begin, end);
    };

    const auto& internal = path.toUtf8();

    // Get the path component without the drive on Windows
    size_t beginOffset = 0;
#if PLATFORM_IS(PLATFORM_WINDOWS)
    if (isAbsolute) {
        beginOffset = 2;
    }
#endif

    // Split the string by the separator
    const char* pathcStart = internal.data() + beginOffset;
    const char* pathcEnd = pathcStart;
    while (*pathcEnd != 0) {
        // Get the path component from the start and end iterators
        if (*pathcEnd == GetOsSeparator() && pathcEnd > pathcStart) {
            addPathComponent(pathcStart, pathcEnd);
            pathcStart = pathcEnd;
        }
        if (*pathcStart == GetOsSeparator()) {
            pathcStart++;
        }
        pathcEnd++;
    }
    // Get the last path component
    // (if the string does not finish in a separator)
    if (pathcStart != pathcEnd) {
        addPathComponent(pathcStart, pathcEnd);
    }

    // Create the result normalized path
    String ret;
    if (isAbsolute) {
#if PLATFORM_IS(PLATFORM_WINDOWS)
        auto it = internal.cbegin();
        ret += String::FromUtf8(it, it + 2) + '\\';
#else
        ret += '/';
#endif
    }
    if (!isAbsolute & pathComps.empty()) {
        ret += '.';
    } else {
        for (size_t i = 0; i < pathComps.size(); i++) {
            if (i) {
                ret += GetOsSeparator();
            }
            ret += String::FromUtf8(pathComps[i].first, pathComps[i].second);
        }
    }

#if PLATFORM_IS(PLATFORM_WINDOWS)
    // Fix separators on Windows
    ret.replace('/', '\\');
#endif

    return ret;
}

bool IsAbsolutePath(StringView path) {
    const auto& internal = path.getData();
    if (path.isEmpty()) {
        return false;
    }
#if PLATFORM_IS(PLATFORM_WINDOWS)
    return path.getSize() > 2 && internal[1] == ':' && internal[2] == '\\';
#elif PLATFORM_IS(PLATFORM_LINUX | PLATFORM_MACOS | PLATFORM_IOS | PLATFORM_ANDROID)
    return internal[0] == '/';
#else
    return false;
#endif
}

String Join(StringView left, StringView right) {
    if (right.isEmpty()) {
        return String::FromUtf8(left.cbegin(), left.cend());
    }
    if (left.isEmpty()) {
        return String::FromUtf8(right.cbegin(), right.cend());
    }

    if (IsAbsolutePath(right)) {
        return String::FromUtf8(right.cbegin(), right.cend());
    }

    String ret;
    const auto& internal = left.getData();
    char lastCharacter = internal[left.getSize() - 1];
    if (lastCharacter == GetOsSeparator()) {
        ret = "{}{}"_format(left, right);
    } else {
        ret = "{}{}{}"_format(left, GetOsSeparator(), right);
    }
    return ret;
}

void SetSearchPaths(Vector<String> searchPaths) {
    sSearchPaths = std::move(searchPaths);
}

const Vector<String>& GetSearchPaths() {
    if (sSearchPaths.empty()) {
        sSearchPaths.push_back(GetExecutableDirectory());
    }
    return sSearchPaths;
}

void AddSearchPath(const String& path) {
    sSearchPaths.push_back(path);
}

}  // namespace filesystem

}  // namespace edoren
