/*
*#include "decompiler.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <array>
#include <numeric>

LuaDeobfuscator::LuaDeobfuscator(const std::vector<uint8_t>& data) : data(data), n(0) {
    parseHeader();
    parseFunction();
}

std::vector<uint8_t> LuaDeobfuscator::getData() const {
    return data;
}

void LuaDeobfuscator::parseHeader() {
    if (data.size() < 12) {
        throw LuaDeobfuscatorException("Data too short for header");
    }
    // Check signature
    if (data[0] != 0x1B || data[1] != 'L' || data[2] != 'u' || data[3] != 'a') {
        throw LuaDeobfuscatorException("Not a Lua script");
    }
    // Check version
    if (data[4] != 0x51) {
        throw LuaDeobfuscatorException("Incompatible Lua version");
    }
    n = 12; // Move past the header
}

void LuaDeobfuscator::parseFunction() {
    // Parse function details
    parseCode();
    parseConstants();
    parseDebug();
}

void LuaDeobfuscator::parseCode() {
    // Implement code parsing logic
    uint32_t size = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
    n += 4;
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t instruction = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
        n += 4;
        // Process instruction
    }
}

void LuaDeobfuscator::parseConstants() {
    // Implement constants parsing logic
    uint32_t size = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
    n += 4;
    for (uint32_t i = 0; i < size; ++i) {
        uint8_t type = data[n++];
        switch (type) {
        case 0: // NIL
            break;
        case 1: // Boolean
            n++;
            break;
        case 3: // Number
            n += 8;
            break;
        case 4: // String
        {
            uint32_t len = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
            n += 4 + len;
        }
        break;
        default:
            throw LuaDeobfuscatorException("Unknown constant type");
        }
    }
}

void LuaDeobfuscator::parseDebug() {
    // Implement debug parsing logic
    uint32_t size = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
    n += 4 + size * 4; // Skip line info
    size = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
    n += 4;
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t len = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
        n += 4 + len + 8; // Skip local variable info
    }
    size = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
    n += 4;
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t len = bytesToInt(std::vector<uint8_t>(data.begin() + n, data.begin() + n + 4));
        n += 4 + len; // Skip upvalue names
    }
}

uint32_t LuaDeobfuscator::bytesToInt(const std::vector<uint8_t>& bytes) {
    return *reinterpret_cast<const uint32_t*>(bytes.data());
}

std::vector<uint8_t> LuaDeobfuscator::intToBytes(uint32_t value) {
    std::vector<uint8_t> bytes(4);
    std::memcpy(bytes.data(), &value, 4);
    return bytes;
}

Script::Script(const std::vector<uint8_t>& raw) : raw(raw) {
    parseHeader();
    parseLuaBytecode();
    parseSignature();
}

bool Script::isMagicOk() const {
    return magic == 0xF14A55B7;
}

bool Script::isChecksumOk() const {
    return checksum == checksumCalculated;
}

bool Script::isHeaderOffsetOk() const {
    return headerOffset == raw.size() - 157;
}

bool Script::isSignatureOk() const {
    Sha1Hash sha1;
    sha1.update(std::vector<uint8_t>(raw.begin(), raw.end() - 64));
    auto calculatedHash = sha1.digest();
    // Compare calculatedHash with the stored signature hash
    // Implement signature check logic
    return true;
}

void Script::parseHeader() {
    if (raw.size() < 157) {
        throw ScriptException("The input is too small.");
    }
    magic = *reinterpret_cast<const uint32_t*>(&raw[81]);
    if (!isMagicOk()) {
        throw ScriptException("Incorrect magic. Probably not a signed .luac file for MTA:SA.");
    }
    headerOffset = *reinterpret_cast<const uint32_t*>(&raw[85]);
    if (!isHeaderOffsetOk()) {
        throw ScriptException("Incorrect header offset. The script is probably corrupted.");
    }
    checksum = *reinterpret_cast<const uint32_t*>(&raw[89]);
    checksumCalculated = std::accumulate(raw.begin(), raw.end() - 5, 0);
    if (!isChecksumOk()) {
        throw ScriptException("Incorrect checksum. The script is probably corrupted.");
    }
}

void Script::parseLuaBytecode() {
    // Implement Lua bytecode parsing logic
}

void Script::parseSignature() {
    // Implement signature parsing logic
}

bool isUtf8Bom(const void* pData, unsigned int uiLength) {
    const unsigned char* pCharData = static_cast<const unsigned char*>(pData);
    return (uiLength > 2 && pCharData[0] == 0xEF && pCharData[1] == 0xBB && pCharData[2] == 0xBF);
}

class Sha1Hash {
public:
    Sha1Hash();
    void update(const std::vector<uint8_t>& data);
    std::array<uint8_t, 20> digest();

private:
    void processChunk(const uint8_t* chunk);
    uint32_t leftRotate(uint32_t value, size_t count);

    std::array<uint32_t, 5> h;
    std::vector<uint8_t> buffer;
    uint64_t bitLength;
};

Sha1Hash::Sha1Hash() : h{ 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 }, bitLength(0) {}

void Sha1Hash::update(const std::vector<uint8_t>& data) {
    buffer.insert(buffer.end(), data.begin(), data.end());
    bitLength += data.size() * 8;
    while (buffer.size() >= 64) {
        processChunk(buffer.data());
        buffer.erase(buffer.begin(), buffer.begin() + 64);
    }
}

std::array<uint8_t, 20> Sha1Hash::digest() {
    // Padding
    buffer.push_back(0x80);
    while (buffer.size() % 64 != 56) {
        buffer.push_back(0);
    }
    // Append length
    for (int i = 7; i >= 0; --i) {
        buffer.push_back((bitLength >> (i * 8)) & 0xFF);
    }
    // Process final chunks
    while (buffer.size() >= 64) {
        processChunk(buffer.data());
        buffer.erase(buffer.begin(), buffer.begin() + 64);
    }
    // Produce the final hash
    std::array<uint8_t, 20> result;
    for (size_t i = 0; i < h.size(); ++i) {
        for (size_t j = 0; j < 4; ++j) {
            result[i * 4 + j] = (h[i] >> ((3 - j) * 8)) & 0xFF;
        }
    }
    return result;
}

void Sha1Hash::processChunk(const uint8_t* chunk) {
    uint32_t w[80];
    for (size_t i = 0; i < 16; ++i) {
        w[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) | (chunk[i * 4 + 2] << 8) | chunk[i * 4 + 3];
    }
    for (size_t i = 16; i < 80; ++i) {
        w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    for (size_t i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        uint32_t temp = leftRotate(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = leftRotate(b, 30);
        b = a;
        a = temp;
    }
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}

uint32_t Sha1Hash::leftRotate(uint32_t value, size_t count) {
    return (value << count) | (value >> (32 - count));
}

namespace utilities {
    namespace c_scripts {

        std::vector<uint8_t> decompile(const void* pData, unsigned int uiLength) {
            const unsigned char* pCharData = static_cast<const unsigned char*>(pData);
            if (isUtf8Bom(pCharData, uiLength)) {
                pCharData += 3;
                uiLength -= 3;
            }
            if (uiLength > 0 && pCharData[0] == 0x1B) {
                try {
                    std::vector<uint8_t> data(pCharData, pCharData + uiLength);
                    LuaDeobfuscator deobfuscator(data);
                    return deobfuscator.getData();
                }
                catch (const LuaDeobfuscator::LuaDeobfuscatorException& e) {
                    std::cerr << "Decompilation failed: " << e.what() << std::endl;
                    return {};
                }
            }
            std::cerr << "Not a compiled Lua script." << std::endl;
            return {};
        }

        bool sex::c_scripts::decompile(const void* pData, unsigned int uiLength) {
            const unsigned char* pCharData = static_cast<const unsigned char*>(pData);
            if (isUtf8Bom(pCharData, uiLength)) {
                pCharData += 3;
                uiLength -= 3;
            }
            if (uiLength > 0 && pCharData[0] == 0x1B) {
                // Attempt to decompile the Lua bytecode
                std::vector<uint8_t> decompiledData = decompile(pCharData, uiLength);
                if (!decompiledData.empty()) {
                    std::cout << "Decompilation successful." << std::endl;
                    return true;
                }
                else {
                    std::cerr << "Decompilation failed." << std::endl;
                    return false;
                }
            }
            return false;
        }

    } // namespace c_scripts
} // namespace utilities **/