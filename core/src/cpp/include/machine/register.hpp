#ifndef __MC64K_MACHINE_REGISTER_HPP__
#   ifndef __MC64K_MACHINE_HPP__
#       error "Header not included at correct scope"
#   endif
#   define __MC64K_MACHINE_REGISTER_HPP__

/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

/**
 * MC64K::Register
 */
namespace Register {
    union GeneralPurpose {
        private:
            uint64*  pu64;
            int64*   pi64;
            uint32*  pu32;
            int32*   pi32;
            uint16*  pu16;
            int16*   pi16;
            uint8*   pu8;
            int8*    pi8;
            float64* pf64;
            float32* pf32;
            void*    p;

            uint64  u64;
            int64   i64;
            uint32  u32;
            int32   i32;
            uint16  u16;
            int16   i16;
            uint8   u8;
            int8    i8;

        public:
            GeneralPurpose() : u64(0) {}
    };

    union FloatingPoint {
        float64 f64;
        float32 f32;
    };
};

#endif
