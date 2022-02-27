#pragma once

#include <edoren/String.hpp>
#include <edoren/StringView.hpp>
#include <edoren/container/Vector.hpp>

namespace edoren {

namespace filesystem {

/**
 * @brief Checks if a file exist
 *
 * @return true if the file exist, false otherwise
 */
bool FileExists(StringView filename);

/**
 * @brief Load an UTF-8 encoded file to the memory
 *
 * @param filename The file to load the data from
 * @param dest A string to store de file text
 * @return true if the file could be loaded, false
 *         otherwise
 */
bool LoadFileData(StringView filename, String& dest);

/**
 * @brief Load a file to the memory
 *
 * @param filename The file to load the data from
 * @param dest A vector to store de file bytes
 * @return true if the file could be loaded, false
 *         otherwise
 */
bool LoadFileData(StringView filename, Vector<uint8_t>& dest);

/**
 * @brief Get the OS specific path separator
 *
 * @return The character that represents the path separator
 */
char GetOsSeparator();

/**
 * @brief Get the directory that contains the executable
 *        in which this function reside.
 *
 * @return String containing the executable directory
 */
const String& ExecutableDirectory();

/**
 * @brief Return a string representing the current working
 *        directory.
 *
 * @return String containing the current working directory
 */
String CurrentWorkingDirectory();

/**
 * @brief Return the absolute path
 *
 * @param path The relative path to make absolute
 * @return String containing the absolute path
 */
String AbsolutePath(const String& path);

/**
 * @brief Normalize a pathname by collapsing redundant
 *        separators and up-level references so that A//B,
 *        A/B/, A/./B and A/foo/../B all become A/B
 *
 * On Windows this function also change any '/' character
 * to '\'
 *
 * @param path The path to normalize
 * @return String containing the normalized path
 */
String NormalizePath(const String& path);

/**
 * @brief Check if a path is absolute
 *
 * @param path The path to check
 * @return true if the path is absolute, false otherwise
 */
bool IsAbsolutePath(const StringView& path);

/**
 * @brief Join two path components. The return value is
 *        the concatenation of the paths with a the
 *        spacific OS Separator between them.
 *
 * @param left The first path to join
 * @param right The second path to join
 * @return String concatenating the two paths components
 */
String Join(const StringView& left, const StringView& right);

/**
 * @brief Variadic version of the Join function that
 *        accepts multiple path components as arguments.
 *
 * @tparam Args Variadic arguments
 * @param left The first path to join
 * @param right The second path to join
 * @param paths Additional paths to join
 * @return String concatenating all the provided path components
 */
template <typename... Args>
String Join(const StringView& left, const StringView& right, Args... paths) {
    return Join(Join(left, right), paths...);
}

/**
 * @brief Change the search paths used to open files
 *
 * @param searchPaths Vector of new search paths
 */
void SetSearchPaths(Vector<String> searchPaths);

/**
 * @brief Obtain the serch paths used to open files
 *
 * @return A constant reference to the currently used search
 *         paths
 */
const Vector<String>& GetSearchPaths();

/**
 * @brief Add a path to the search paths used to open files
 *
 * @param path That is going to added to the search paths
 * @return A constant reference to the currently used search
 *         paths
 */
void AddSearchPath(const String& path);

}  // namespace filesystem

}  // namespace edoren
