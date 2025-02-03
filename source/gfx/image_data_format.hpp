#pragma once

enum class ImageDataFormat : int {
    Invalid = -1,
    Unknown = 0,
    A8 = 8,
    R5G6B5 = 565,
    A1R5G5B5 = 1555,
    A4R4G4B4 = 4444,
    A8R8G8B8 = 8888
};