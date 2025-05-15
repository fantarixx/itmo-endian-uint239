# 💾 ITMO Endian: Custom Unsigned Integer `uint239_t`

This project is a C++ implementation of a custom unsigned integer type `uint239_t` using a non-standard memory representation called **ITMO Endian**. Designed as a lab assignment at ITMO University, the goal was to build a fixed-width integer type with unique byte-level encoding, bit-shifting behavior, and arithmetic support—all without using standard containers or bit manipulation utilities.

---

## 📐 Overview

The `uint239_t` type is an unsigned 239-bit integer, packed into exactly **35 bytes**, using a custom encoding scheme:

- Each byte holds **7 data bits** and **1 service bit** (MSB).
- The service bits across all 35 bytes encode a **circular left shift** amount for the significant bits.
- The format includes **6 padding bits** (always 0) due to the misalignment between 239 and 7.

---

## 🔍 ITMO Endian Format

The binary representation of a `uint239_t` consists of:

- **239 significant bits** (data bits),
- **35 service bits** (one per byte),
- **6 zero-padding bits** (non-significant but shiftable).

The value stored is interpreted after reversing the circular left shift defined by the 35 service bits treated as a binary number.

### Example

A value like `2047` might be stored in multiple valid ITMO Endian representations, such as:

```
2047 = < 0 000000 ... 0001111 0111111 > , shift = 0
2047 = < 1 1111110 ... > , shift = 1
2047 = < 0 1111100 ... > , shift = 2
```

Each format stores the same numeric value, just with different bit alignments determined by the shift.

---

## ✨ Features

The `uint239_t` type supports the following operations:

- ✅ Conversion from `uint32_t`
- ✅ Conversion from string (guaranteed input < 2^64)
- ➕ Addition
- ➖ Subtraction
- ✅ Stream output (`std::ostream`)
- ↔️ Shift extraction (`GetShift`)
- ✅ Equality and inequality comparisons

🔒 **Multiplication and division** are conditionally implemented and may be skipped depending on the course group.

---

## ⚙️ Implementation Notes

- Code is implemented entirely in C++ without `std::vector`, `std::bitset`, or other STL containers.
- All bit-level manipulation is done manually using primitive types and bitwise operators.
- Shift encoding and decoding are handled as described above.
- Arithmetic operations correctly propagate shifts:  
  - **Addition/Multiplication**: shift = sum of operand shifts  
  - **Subtraction/Division**: shift = difference of operand shifts

### Examples of Shift Behavior

| Operation | Operand A (shift 3) | Operand B (shift 5) | Result shift |
|----------|----------------------|----------------------|--------------|
| A + B    | 239 + 30             | 269                  | 8            |
| A - B    | 239 - 30             | 209                  | 2³⁵ - 2      |
| A * B    | 123 * 10             | 1230                 | 10           |
| A / B    | 42 / 5               | 8                    | 5            |

---

## 🧪 Testing

The project includes a suite of unit tests written with Google Test. Run them via:

```bash
cmake --build . --target number_tests && ctest -V
```

Or run through your favorite C++ IDE. You can add your own test cases if desired.

> Note: multiplication and division tests can be commented out depending on your assignment requirements.

---

## 📁 Project Structure

```
.
├── bin/       # Optional CLI utilities
├── lib/       # Core implementation: number.cpp / number.h
└── tests/     # Google Test cases for uint239_t
```

---

## 🚫 Restrictions

To keep the implementation low-level and educational, the following are **not allowed**:

- ❌ STL containers (`std::vector`, `std::list`, etc.)
- ❌ `std::bitset`
- ❌ Changing project structure or filenames

---

## 📎 Usage

To convert and print a number:

```cpp
uint239_t a = FromInt(42, 5);
std::cout << a << std::endl;
```

---

## 📚 Learning Goals

This project helps you:

- Understand low-level bit and byte manipulation.
- Work with custom memory layouts and alignment.
- Implement arithmetic for non-standard data types.
- Practice clean C++ design under constraints.

---

## 📌 Disclaimer

This was originally designed as a lab assignment at ITMO University. Feel free to reuse or adapt the ideas, but please give credit to the original concept if you build on it.
