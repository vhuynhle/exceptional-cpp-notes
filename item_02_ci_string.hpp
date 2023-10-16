#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>

// A case-insensitive char trait.
// Provide specialization for comparison functions and inherit others.
struct ci_char_traits : public std::char_traits<char> {
    static constexpr bool eq(char a, char b)
    {
        return std::toupper(a) == std::toupper(b);
    }

    static constexpr bool lt(char a, char b)
    {
        return std::toupper(a) < std::toupper(b);
    }

    static constexpr int compare(const char* s1, const char* s2, std::size_t count)
    {
        for (std::size_t i { 0 }; i < count; ++i) {
            if (s1[i] == '\0' && s2[i] == '0') {
                return 0;
            } else if (s1[i] == 0) {
                return -1;
            } else if (s2[i] == 0) {
                return 1;
            }

            if (ci_char_traits::lt(s1[i], s2[i])) {
                return -1;
            } else if (ci_char_traits::lt(s2[i], s1[i])) {
                return 1;
            }
        }

        return 0;
    }

    static constexpr const char* find(const char* ptr, std::size_t count, const char& ch)
    {
        const auto ch_uppercase = std::toupper(ch);
        const auto it = std::find_if(ptr, ptr + count,
            [ch_uppercase](const char c) -> bool { return std::toupper(c) == ch_uppercase; });

        return (it != ptr + count) ? it : nullptr;
    }
};

using ci_string = std::basic_string<char, ci_char_traits>;
