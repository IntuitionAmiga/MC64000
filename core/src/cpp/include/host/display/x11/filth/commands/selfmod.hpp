#ifdef FILTH_COMMAND_SELFMOD

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
 * FILTH processor (switch/case) handlers for script self-modification
 *
 * We can set, add and subtract values from given script offsets
 */

case FC_SET_BYTE:
    puCode += setImmediate<uint8>(puCode, roContext.puFilthScript);
    break;

case FC_SET_WORD:
    puCode += setImmediate<uint16>(puCode, roContext.puFilthScript);
    break;

case FC_SET_LONG:
    puCode += setImmediate<uint32>(puCode, roContext.puFilthScript);
    break;

case FC_ADD_BYTE:
    puCode += addToImmediate<uint8>(puCode, roContext.puFilthScript);
    break;

case FC_ADD_WORD:
    puCode += addToImmediate<uint16>(puCode, roContext.puFilthScript);
    break;

case FC_ADD_LONG:
    puCode += addToImmediate<uint32>(puCode, roContext.puFilthScript);
    break;

case FC_SUB_BYTE:
    puCode += subFromImmediate<uint8>(puCode, roContext.puFilthScript);
    break;

case FC_SUB_WORD:
    puCode += subFromImmediate<uint16>(puCode, roContext.puFilthScript);
    break;

case FC_SUB_LONG:
    puCode += subFromImmediate<uint32>(puCode, roContext.puFilthScript);
    break;

#undef FILTH_COMMAND_SELFMOD
#endif
