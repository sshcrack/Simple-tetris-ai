#pragma once

#include <array>
#include <cstdint>

namespace tetris {

inline constexpr std::array<std::array<uint16_t, 4>, 7> SHAPE_LIST = {{// O piece
                                                                       {0x3300, 0x3300, 0x3300, 0x3300},
                                                                       // I piece
                                                                       {0x00F0, 0x2222, 0x00F0, 0x2222},
                                                                       // Z piece
                                                                       {0x0630, 0x0264, 0x0630, 0x0264},
                                                                       // S piece
                                                                       {0x06C0, 0x0462, 0x06C0, 0x0462},
                                                                       // L piece
                                                                       {0x6220, 0x0170, 0x4460, 0x0740},
                                                                       // J piece
                                                                       {0x0710, 0x2260, 0x0470, 0x6440},
                                                                       // T piece
                                                                       {0x0270, 0x0232, 0x0072, 0x0262}}};

inline constexpr std::array<int, 7> SHAPE_ROTATIONS = {1, 2, 2, 2, 4, 4, 4};

} // namespace tetris
