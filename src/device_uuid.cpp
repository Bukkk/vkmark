#include "device_uuid.h"
#include <stdexcept>

static std::array<char, 2 * VK_UUID_SIZE> decode_UUID(const std::array<uint8_t, VK_UUID_SIZE>& bytes)
{
    std::array<char, 2 * VK_UUID_SIZE> representation{};
    constexpr char characters[16] = 
        { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    for (std::size_t i = 0; i < bytes.size(); ++i)
    {
        auto& byte = bytes[i];

        representation[2 * i] = characters[byte / 16];
        representation[2 * i + 1] = characters[byte % 16];
    }

    return representation;
}

static std::array<uint8_t, VK_UUID_SIZE> encode_UUID(const std::array<char, 2 * VK_UUID_SIZE>& representation)
{
    std::array<uint8_t, VK_UUID_SIZE> bytes{};

    auto&& decode_character = [](const char ch) {
        if (ch >= '0' && ch <= '9')
            return ch - '0';
        else if (ch >= 'a' && ch <= 'f')
            return ch - 'a' + 10;
        throw std::invalid_argument(
            std::string{ch} + "character found while parsing hexadecimal string!");
    };

    for (std::size_t i = 0; i < bytes.size(); ++i)
    {
        bytes[i] = decode_character(representation[2 * i]) * 16
            + decode_character(representation[2 * i + 1]);
    }

    return bytes;
}

DeviceUUID::DeviceUUID(std::string const& representation)
{
    std::array<char, 2 * VK_UUID_SIZE> chars{};
    if (representation.size() != chars.size())
        throw std::invalid_argument("given UUID representation has wrong size!");

    std::copy(representation.begin(), representation.end(), chars.begin());
    raw = encode_UUID(chars);
}

std::array<char, 2 * VK_UUID_SIZE + 1> DeviceUUID::representation() const
{
    std::array<char, 2 * VK_UUID_SIZE + 1> c_str{};
    auto&& chars = decode_UUID(raw);
    std::copy(chars.begin(), chars.end(), c_str.begin());

    return c_str;
}