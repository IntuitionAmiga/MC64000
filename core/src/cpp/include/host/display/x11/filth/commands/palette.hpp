#ifdef FILTH_COMMAND_PALLETE

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
 * FILTH processor (switch/case) handlers for palette related commands
 *
 * Note that these are included in templated code
 */

case FC_SET_PALETTE: {
    uint8 uIndex = *puCode++;
    puPalette[uIndex] = getImmediate<T>(puCode);
    puCode += sizeof(T);
    break;
}

case FC_ADD_PALETTE_RGB: {
    // todo - this sucks and will not work for non 32-bit displays
    uint8 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[0] += puCode[0];
    ((uint8*)(&puPalette[uIndex]))[1] += puCode[1];
    ((uint8*)(&puPalette[uIndex]))[2] += puCode[2];
    puCode += sizeof(uint32);
    break;
}

case FC_SUB_PALETTE_RGB: {
    // todo - this sucks and will not work for non 32-bit displays
    uint8 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[0] -= puCode[0];
    ((uint8*)(&puPalette[uIndex]))[1] -= puCode[1];
    ((uint8*)(&puPalette[uIndex]))[2] -= puCode[2];
    puCode += sizeof(uint32);
    break;
}

case FC_SET_PALETTE_R: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[2] = *puCode++;
    break;
}

case FC_ADD_PALETTE_R: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[2] += *puCode++;
    break;
}

case FC_SUB_PALETTE_R: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[2] -= *puCode++;
    break;
}

case FC_SET_PALETTE_G: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[1] = *puCode++;
    break;
}

case FC_ADD_PALETTE_G: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[1] += *puCode++;
    break;
}

case FC_SUB_PALETTE_G: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[1] -= *puCode++;
    break;
}

case FC_SET_PALETTE_B: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[0] = *puCode++;
    break;
}

case FC_ADD_PALETTE_B: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[0] += *puCode++;
    break;
}

case FC_SUB_PALETTE_B: {
    // todo - this sucks and will not work for non 32-bit displays
    uint32 uIndex = *puCode++;
    ((uint8*)(&puPalette[uIndex]))[0] -= *puCode++;
    break;
}

case FC_SWP_PALETTE: {
    uint8 uIndexA = *puCode++;
    uint8 uIndexB = *puCode++;
    T uRGB = puPalette[uIndexA];
    puPalette[uIndexA] = puPalette[uIndexB];
    puPalette[uIndexB] = uRGB;
    break;
}

#undef FILTH_COMMAND_PALLETE
#endif
