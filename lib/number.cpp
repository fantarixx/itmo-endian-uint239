#include "number.h"

uint239_t FromInt(uint32_t value, uint32_t shift) {
    uint239_t number;

    uint32_t byte_shift = shift / 7;
    uint32_t byte = 0, inside_byte_pos = shift - byte_shift * 7;
    while (value > 0 || shift > 0) {
        number.data[34 - (byte + byte_shift) % 35] +=
            (value % 2) * (1 << inside_byte_pos++);
        if (inside_byte_pos % 8 == 7) {
            number.data[34 - byte++] += shift % 2 << 7;
            shift /= 2;
            inside_byte_pos = 0;
        }
        value /= 2;
    }
    return number;
}

uint239_t FromString(const char* str, uint32_t shift) {
    uint239_t number;
    uint64_t value = std::stoll(str);

    uint32_t byte_shift = shift / 7;
    uint32_t byte = 0, inside_byte_pos = shift - byte_shift * 7;
    while (value > 0 || shift > 0) {
        number.data[34 - (byte + byte_shift) % 35] +=
            (value % 2) * (1 << inside_byte_pos++);
        if (inside_byte_pos % 8 == 7) {
            number.data[34 - byte++] += shift % 2 << 7;
            shift /= 2;
            inside_byte_pos = 0;
        }
        value /= 2;
    }
    return number;
}

uint239_t operator+(const uint239_t& lhs, const uint239_t& rhs) {
    uint32_t lhs_shift = GetShift(lhs),
        rhs_shift = GetShift(rhs),
        shift = rhs_shift + lhs_shift;
    uint239_t number;
    uint32_t byte_shift = shift / 7, byte = 0, inside_byte_pos = shift - byte_shift * 7;
    for (uint32_t left_pos = 0, delta = 0, right_pos = 0; left_pos < 239; ++left_pos, ++right_pos) {
        uint32_t lhs_value = (lhs.data[34 - (left_pos + lhs_shift) / 7 % 35] >> (left_pos + lhs_shift) % 7) % 2;
        uint32_t rhs_value = (rhs.data[34 - (right_pos + rhs_shift) / 7 % 35] >> (right_pos + rhs_shift) % 7) % 2;
        number.data[34 - (byte + byte_shift) % 35] += (rhs_value + lhs_value + delta) % 2 * (1 << inside_byte_pos++);
        if (rhs_value + lhs_value + delta >= 2) {
            delta = 1;
        } else {
            delta = 0;
        }
        if (inside_byte_pos % 8 == 7) {
            number.data[34 - byte++] += shift % 2 << 7;
            shift /= 2;
            inside_byte_pos = 0;
        }
    }
    return number;
}

uint239_t operator-(const uint239_t& lhs, const uint239_t& rhs) {
    uint32_t lhs_shift = GetShift(lhs),
        rhs_shift = GetShift(rhs),
        shift = lhs_shift - rhs_shift;
    uint239_t number;
    uint32_t byte_shift = shift / 7, byte = 0, inside_byte_pos = shift % 7;

    bool flag = false;
    for (uint32_t left_pos = 0, delta = 0, right_pos = 0; left_pos < 239; ++left_pos, ++right_pos) {
        uint32_t lhs_value = (lhs.data[34 - (left_pos + lhs_shift) / 7 % 35] >> (left_pos + lhs_shift) % 7) % 2,
                 rhs_value = (rhs.data[34 - (right_pos + rhs_shift) / 7 % 35] >> (right_pos + rhs_shift) % 7) % 2;
        if (lhs_value - rhs_value + delta < 0) {
            delta += 2;
            flag = true;
        }
        number.data[34 - (byte + byte_shift) % 35] += (lhs_value - rhs_value + delta) << inside_byte_pos++;
        if (flag) {
            delta = -1;
            flag = false;
        }
        if (inside_byte_pos % 8 == 7) {
            number.data[34 - byte++] += shift % 2 << 7;
            shift /= 2;
            inside_byte_pos = 0;
        }
    }
    return number;
}

bool operator==(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);
    for (int left_pos = 0, right_pos = 0; left_pos < 239; ++left_pos, ++right_pos) {
        if ((lhs.data[34 - (left_pos + lhs_shift) / 7 % 35] >> (left_pos + lhs_shift) % 7) % 2 !=
            (rhs.data[34 - (right_pos + rhs_shift) / 7 % 35] >> (right_pos + rhs_shift) % 7) % 2) {
            return false;
        }
    }
    return true;
}

bool operator!=(const uint239_t& lhs, const uint239_t& rhs) {
    uint64_t lhs_shift = GetShift(lhs);
    uint64_t rhs_shift = GetShift(rhs);

    for (int left_pos = 0, right_pos = 0; left_pos < 239; ++left_pos, ++right_pos) {
        if ((lhs.data[34 - (left_pos + lhs_shift) / 7 % 35] >> (left_pos + lhs_shift) % 7) % 2 !=
            (rhs.data[34 - (right_pos + rhs_shift) / 7 % 35] >> (right_pos + rhs_shift) % 7) % 2) {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const uint239_t& value) {
    for (int i = 0; i < 35; ++i) {
        for (int j = 7; j >= 0; --j) {
            stream << (value.data[i] >> j) % 2;
        }
        stream << " ";
    }
    stream << "\n";
    return stream;
}

uint64_t GetShift(const uint239_t& value) {
    int shift = 0;
    for (int byte = 34, pow = 0; byte >= 0; --byte, ++pow) {
        shift += (1 << pow) * (value.data[byte] >> 7);
    }
    return shift;
}
