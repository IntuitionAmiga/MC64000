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
#include <cstdio>
#include <cmath>
#include <machine/interpreter.hpp>
#include <machine/timing.hpp>
#include <bytecode/opcode.hpp>
#include <machine/inline.hpp>
#include <machine/gnarly.hpp>

namespace MC64K::Machine {

// Super gnarly macros for custom jump table

#define dsc()      goto begin_interpreter
#define end()      goto end_interpreter
#define dispatch() goto *((uint8*)&&begin_interpreter + uJumpTable[*puProgramCounter++])

#ifdef THREADED_DISPATCH
    #define SKIP_STATUS
    #define ssc() dispatch()
#else
    #define SKIP_STATUS skip_status_check:
    #define ssc() goto skip_status_check
#endif

// Operation Name
#define OP(NAME) op_ ## NAME

// Jump Table Entry
#define JTE(NAME) (uint16)((uint8 const*)&&OP(NAME) - (uint8 const*)&&begin_interpreter)

/**
 * @inheritDoc
 */
void Interpreter::run() {

    using namespace MC64K::ByteCode;

    if (!puProgramCounter) {
        return;
    }

    static uint16 const uJumpTable[256] = {
        // Control
        JTE(STOP),
        JTE(HOST),
        JTE(BRA_B), JTE(BRA),
        JTE(BSR_B), JTE(BSR),
        JTE(JMP),
        JTE(JSR),
        JTE(RTS),
        JTE(BMC),
        JTE(BDC),
        JTE(DBNZ),
        JTE(R_BMC),
        JTE(R2R_BDC),
        JTE(R_DBNZ),

        // Data move
        JTE(R2R_MOVE_L),  JTE(R2R_MOVE_Q),
        JTE(R2R_FMOVE_S), JTE(R2R_FMOVE_D),
        JTE(R2R_CLR_L),   JTE(R2R_CLR_Q),
        JTE(R2R_EXG),
        JTE(R2R_FEXG),
        JTE(R2R_SWAP),

        JTE(R2R_SWAP_L), JTE(R2R_SWAP_Q),

        JTE(MOVE_B), JTE(MOVE_W), JTE(MOVE_L), JTE(MOVE_Q),

        JTE(SAVEM),
        JTE(LOADM),
        JTE(FMOVEB_S), JTE(FMOVEB_D),
        JTE(FMOVEW_S), JTE(FMOVEW_D),
        JTE(FMOVEL_S), JTE(FMOVEL_D),
        JTE(FMOVEQ_S), JTE(FMOVEQ_D),
        JTE(FMOVES_L), JTE(FMOVES_Q),
        JTE(FMOVES_D), JTE(FMOVED_L),
        JTE(FMOVED_Q), JTE(FMOVED_S),
        JTE(FMOVE_S),  JTE(FMOVE_D),
        JTE(FINFO_S),  JTE(FINFO_D),
        JTE(CLR_B), JTE(CLR_W), JTE(CLR_L), JTE(CLR_Q),
        JTE(LINK),
        JTE(UNLK),
        JTE(LEA),
        JTE(PEA),
        JTE(SCM),
        JTE(SCD),

        JTE(R2R_AND_L), JTE(R2R_AND_Q),
        JTE(R2R_OR_L),  JTE(R2R_OR_Q),
        JTE(R2R_EOR_L), JTE(R2R_EOR_Q),
        JTE(R2R_NOT_L), JTE(R2R_NOT_Q),
        JTE(R2R_LSL_L), JTE(R2R_LSL_Q),
        JTE(R2R_LSR_L), JTE(R2R_LSR_Q),

        JTE(AND_B), JTE(AND_W), JTE(AND_L), JTE(AND_Q),
        JTE(OR_B),  JTE(OR_W),  JTE(OR_L),  JTE(OR_Q),
        JTE(EOR_B), JTE(EOR_W), JTE(EOR_L), JTE(EOR_Q),
        JTE(NOT_B), JTE(NOT_W), JTE(NOT_L), JTE(NOT_Q),
        JTE(LSL_B), JTE(LSL_W), JTE(LSL_L), JTE(LSL_Q),
        JTE(LSR_B), JTE(LSR_W), JTE(LSR_L), JTE(LSR_Q),
        JTE(ROL_B), JTE(ROL_W), JTE(ROL_L), JTE(ROL_Q),
        JTE(ROR_B), JTE(ROR_W), JTE(ROR_L), JTE(ROR_Q),

        JTE(BCLR_B), JTE(BCLR_W), JTE(BCLR_L), JTE(BCLR_Q),
        JTE(BSET_B), JTE(BSET_W), JTE(BSET_L), JTE(BSET_Q),

        JTE(BFFFO),
        JTE(BFCNT),

        // Arithmetic

        JTE(R2R_EXTB_L),  JTE(R2R_EXTB_Q),
        JTE(R2R_EXTW_L),  JTE(R2R_EXTW_Q),
        JTE(R2R_EXTL_Q),
        JTE(R2R_NEG_L),   JTE(R2R_NEG_Q),
        JTE(R2R_FNEG_S),  JTE(R2R_FNEG_D),
        JTE(R2R_FABS_S),  JTE(R2R_FABS_D),
        JTE(R2R_ADD_L),   JTE(R2R_ADD_Q),
        JTE(R2R_FADD_S),  JTE(R2R_FADD_D),
        JTE(R2R_SUB_L),   JTE(R2R_SUB_Q),
        JTE(R2R_FSUB_S),  JTE(R2R_FSUB_D),
        JTE(R2R_MULS_L),  JTE(R2R_MULS_Q),
        JTE(R2R_MULU_L),  JTE(R2R_MULU_Q),
        JTE(R2R_FMUL_S),  JTE(R2R_FMUL_D),
        JTE(R2R_FDIV_S),  JTE(R2R_FDIV_D),
        JTE(R2R_FMOD_S),  JTE(R2R_FMOD_D),
        JTE(R2R_FSQRT_S), JTE(R2R_FSQRT_D),
        JTE(R2R_FMACC_S), JTE(R2R_FMACC_D),
        JTE(R2R_FMADD_S), JTE(R2R_FMADD_D),

        JTE(EXTB_W), JTE(EXTB_L), JTE(EXTB_Q),
        JTE(EXTW_L), JTE(EXTW_Q),
        JTE(EXTL_Q),
        JTE(ASL_B),  JTE(ASL_W),  JTE(ASL_L),  JTE(ASL_Q),
        JTE(ASR_B),  JTE(ASR_W),  JTE(ASR_L),  JTE(ASR_Q),
        JTE(ADD_B),  JTE(ADD_W),  JTE(ADD_L),  JTE(ADD_Q),
        JTE(FADD_S), JTE(FADD_D),
        JTE(SUB_B),  JTE(SUB_W),  JTE(SUB_L),  JTE(SUB_Q),
        JTE(FSUB_S), JTE(FSUB_D),
        JTE(NEG_B),  JTE(NEG_W),  JTE(NEG_L),  JTE(NEG_Q),
        JTE(FNEG_S), JTE(FNEG_D), JTE(MULS_B),
        JTE(MULS_W), JTE(MULS_L), JTE(MULS_Q),
        JTE(MULU_B), JTE(MULU_W), JTE(MULU_L), JTE(MULU_Q),
        JTE(FMUL_S), JTE(FMUL_D),
        JTE(DIVS_L), JTE(DIVS_Q),
        JTE(MODS_L), JTE(MODS_Q),
        JTE(DIVU_L), JTE(DIVU_Q),
        JTE(MODU_L), JTE(MODU_Q),
        JTE(FDIV_S), JTE(FDIV_D),
        JTE(FMOD_S), JTE(FMOD_D),
        JTE(FABS_S), JTE(FABS_D),

        JTE(FSQRT_S),   JTE(FSQRT_D),
        JTE(FACOS_S),   JTE(FACOS_D),
        JTE(FASIN_S),   JTE(FASIN_D),
        JTE(FATAN_S),   JTE(FATAN_D),
        JTE(FCOS_S),    JTE(FCOS_D),
        JTE(FSIN_S),    JTE(FSIN_D),
        JTE(FSINCOS_S), JTE(FSINCOS_D),
        JTE(FTAN_S),    JTE(FTAN_D),
        JTE(FETOX_S),   JTE(FETOX_D),
        JTE(FLOGN_S),   JTE(FLOGN_D),
        JTE(FLOG2_S),   JTE(FLOG2_D),
        JTE(FTWOTOX_S), JTE(FTWOTOX_D),
        JTE(BAD), // 229
        JTE(BAD), // 230
        JTE(BAD), // 231
        JTE(BAD), // 232
        JTE(BAD), // 233
        JTE(BAD), // 234
        JTE(BAD), // 235
        JTE(BAD), // 236
        JTE(BAD), // 237
        JTE(BAD), // 238
        JTE(BAD), // 239
        JTE(0xF0), // 240
        JTE(BAD), // 241
        JTE(BAD), // 242
        JTE(BAD), // 243
        JTE(BAD), // 244
        JTE(BAD), // 245
        JTE(BAD), // 246
        JTE(BAD), // 247
        JTE(BAD), // 248
        JTE(BAD), // 249
        JTE(BAD), // 250
        JTE(BAD), // 251
        JTE(BAD), // 252
        JTE(BAD), // 253
        JTE(BAD), // 254
        JTE(BAD), // 255
    };

#ifdef DUMP_JUMP_TABLE
    std::printf("Jump Table\n");
    for (int i=0; i<256; ++i) {
        if (!(i & 7)) {
            std::printf("\n\t");
        }
        std::printf("| %3d: %5d ", i, (int)uJumpTable[i]);
    }
    std::printf("\n");
#endif

    initDisplacement();

    eStatus = RUNNING;
    int32 iCallDepth = 1;

    begin_interpreter:
    if (__builtin_expect(eStatus == RUNNING, 1)) {

        // ssc() jumps here when THREADED_DISPATCH is disabled
        SKIP_STATUS

        dispatch();

        OP(STOP): eStatus = CAUGHT_FIRE; end();
        OP(HOST):  handleHost(); dsc();
        OP(BRA_B): branchByte(); ssc();
        OP(BRA):   branchLong(); ssc();

        OP(BSR_B): {
            int8 iShortDisplacement = (int8)*puProgramCounter++;
            pushProgramCounter();
            puProgramCounter += iShortDisplacement;
            ++iCallDepth;
            ssc();
        }

        OP(BSR): {
            readDisplacement();
            pushProgramCounter();
            puProgramCounter += iDisplacement;
            ++iCallDepth;
            ssc();
        }

        OP(JMP): {
            monadic(SIZE_QUAD);
            puProgramCounter = (uint8 const*)pDstEA;
            dsc();
        }

        OP(JSR): {
            monadic(SIZE_QUAD);
            pushProgramCounter();
            puProgramCounter = (uint8 const*)pDstEA;
            ++iCallDepth;
            dsc();
        }

        OP(RTS): {
            if (0 == --iCallDepth) {
                eStatus = COMPLETED;
                end();
            } else {
                popProgramCounter();
                ssc();
            }
        }

        OP(BMC): handleBMC(); dsc();
        OP(BDC): handleBDC(); dsc();

        OP(DBNZ): {
            monadic(SIZE_WORD);
            readDisplacement();
            bcc(--asULong(pDstEA));
            dsc();
        }

        OP(R_BMC):   handleRBMC();   ssc();
        OP(R2R_BDC): handleR2RBDC(); ssc();

        OP(R_DBNZ): {
            readRegPair();
            readDisplacement();
            bcc(--dstGPRULong());
            ssc();
        }

        // DataMove - move
        // Fast Path
        OP(R2R_MOVE_L): {
            readRegPair();
            dstGPRULong() = srcGPRULong();
            ssc();
        }

        OP(R2R_MOVE_Q): {
            readRegPair();
            dstGPRUQuad() = srcGPRUQuad();
            ssc();
        }

        OP(R2R_FMOVE_S): {
            readRegPair();
            dstFPRULong() = srcFPRULong();
            ssc();
        }

        OP(R2R_FMOVE_D): {
            readRegPair();
            dstFPRUQuad() = srcFPRUQuad();
            ssc();
        }

        OP(R2R_CLR_L): {
            readRegPair();
            dstGPRULong() = 0;
            ssc();
        }

        OP(R2R_CLR_Q): {
            readRegPair();
            dstGPRUQuad() = 0;
            ssc();
        }

        OP(R2R_EXG): {
            readRegPair();
            uint64 uTemp  = srcGPRUQuad();
            srcGPRUQuad() = dstGPRUQuad();
            dstGPRUQuad() = uTemp;
            ssc();
        }

        OP(R2R_FEXG): {
            readRegPair();
            uint64 uTemp  = srcFPRUQuad();
            srcFPRUQuad() = dstFPRUQuad();
            dstFPRUQuad() = uTemp;
            ssc();
        }

        OP(R2R_SWAP): {
            readRegPair();
            uint32 uTemp  = srcGPRULong();
            dstGPRULong() = uTemp >> 16 | uTemp << 16;
            ssc();
        }

        OP(R2R_SWAP_L): {
            readRegPair();
            dstGPRULong() = __builtin_bswap32(srcGPRULong());
            ssc();
        }

        OP(R2R_SWAP_Q): {
            readRegPair();
            dstGPRUQuad() = __builtin_bswap64(srcGPRUQuad());
            ssc();
        }


        OP(R2R_AND_L): {
            readRegPair();
            dstGPRULong() &= srcGPRULong();
            ssc();
        }

        OP(R2R_AND_Q): {
            readRegPair();
            dstGPRUQuad() &= srcGPRUQuad();
            ssc();
        }

        OP(R2R_OR_L): {
            readRegPair();
            dstGPRULong() |= srcGPRULong();
            ssc();
        }

        OP(R2R_OR_Q): {
            readRegPair();
            dstGPRUQuad() |= srcGPRUQuad();
            ssc();
        }

        OP(R2R_EOR_L): {
            readRegPair();
            dstGPRULong() ^= srcGPRULong();
            ssc();
        }

        OP(R2R_EOR_Q): {
            readRegPair();
            dstGPRUQuad() ^= srcGPRUQuad();
            ssc();
        }

        OP(R2R_NOT_L): {
            readRegPair();
            dstGPRULong() = ~srcGPRULong();
            ssc();
        }

        OP(R2R_NOT_Q): {
            readRegPair();
            dstGPRUQuad() = ~srcGPRUQuad();
            ssc();
        }

        OP(R2R_LSL_L): {
            readRegPair();
            dstGPRULong() <<= (srcGPRULong() & 31);
            ssc();
        }

        OP(R2R_LSL_Q): {
            readRegPair();
            dstGPRUQuad() <<= (srcGPRUQuad() & 63);
            ssc();
        }

        OP(R2R_LSR_L): {
            readRegPair();
            dstGPRULong() >>= (srcGPRULong() & 31);
            ssc();
        }

        OP(R2R_LSR_Q): {
            readRegPair();
            dstGPRUQuad() >>= (srcGPRUQuad() & 63);
            ssc();
        }


        OP(BFFFO): {
            readRegPair();
            uint64 pTemp  = srcGPRUQuad();
            dstGPRUQuad() = pTemp ? (63 - __builtin_clzll(pTemp)) : ~pTemp;
            dsc();
        }

        OP(BFCNT): {
            readRegPair();
            dstGPRUQuad() = __builtin_popcountll(srcGPRUQuad());
            dsc();
        }

        // Arithmetic

        OP(R2R_EXTB_L): {
            readRegPair();
            dstGPRLong() = (int32)srcGPRByte();
            ssc();
        }

        OP(R2R_EXTB_Q): {
            readRegPair();
            dstGPRQuad() = (int64)srcGPRByte();
            ssc();
        }

        OP(R2R_EXTW_L): {
            readRegPair();
            dstGPRLong() = (int32)srcGPRWord();
            ssc();
        }

        OP(R2R_EXTW_Q): {
            readRegPair();
            dstGPRQuad() = (int64)srcGPRWord();
            ssc();
        }

        OP(R2R_EXTL_Q): {
            readRegPair();
            dstGPRQuad() = (int64)srcGPRLong();
            ssc();
        }

        OP(R2R_NEG_L): {
            readRegPair();
            dstGPRLong() = -srcGPRLong();
            ssc();
        }

        OP(R2R_NEG_Q): {
            readRegPair();
            dstGPRQuad() = -srcGPRQuad();
            ssc();
        }

        OP(R2R_FNEG_S): {
            readRegPair();
            dstFPRSingle() = -srcFPRSingle();
            ssc();
        }

        OP(R2R_FNEG_D): {
            readRegPair();
            dstFPRDouble() = -srcFPRDouble();
            ssc();
        }

        OP(R2R_FABS_S): {
            readRegPair();
            dstFPRSingle() = std::fabs(srcFPRSingle());
            ssc();
        }

        OP(R2R_FABS_D): {
            readRegPair();
            dstFPRDouble() = std::fabs(srcFPRDouble());
            ssc();
        }

        OP(R2R_ADD_L): {
            readRegPair();
            dstGPRLong() += srcGPRLong();
            ssc();
        }

        OP(R2R_ADD_Q): {
            readRegPair();
            dstGPRQuad() += srcGPRQuad();
            ssc();
        }
        OP(R2R_FADD_S): {
            readRegPair();
            dstFPRSingle() += srcFPRSingle();
            ssc();
        }

        OP(R2R_FADD_D): {
            readRegPair();
            dstFPRDouble() += srcFPRDouble();
            ssc();
        }

        OP(R2R_SUB_L): {
            readRegPair();
            dstGPRLong() -= srcGPRLong();
            ssc();
        }

        OP(R2R_SUB_Q): {
            readRegPair();
            dstGPRQuad() -= srcGPRQuad();
            ssc();
        }

        OP(R2R_FSUB_S): {
            readRegPair();
            dstFPRSingle() -= srcFPRSingle();
            ssc();
        }

        OP(R2R_FSUB_D): {
            readRegPair();
            dstFPRDouble() -= srcFPRDouble();
            ssc();
        }

        OP(R2R_MULS_L): {
            readRegPair();
            dstGPRLong() *= srcGPRLong();
            ssc();
        }

        OP(R2R_MULS_Q): {
            readRegPair();
            dstGPRQuad() *= srcGPRQuad();
            ssc();
        }

        OP(R2R_MULU_L): {
            readRegPair();
            dstGPRULong() *= srcGPRULong();
            ssc();
        }

        OP(R2R_MULU_Q): {
            readRegPair();
            dstGPRUQuad() *= srcGPRUQuad();
            ssc();
        }

        OP(R2R_FMUL_S): {
            readRegPair();
            dstFPRSingle() *= srcFPRSingle();
            ssc();
        }

        OP(R2R_FMUL_D): {
            readRegPair();
            dstFPRDouble() *= srcFPRDouble();
            ssc();
        }

        OP(R2R_FDIV_S): {
            readRegPair();
            dstFPRSingle() /= srcFPRSingle();
            ssc();
        }

        OP(R2R_FDIV_D): {
            readRegPair();
            dstFPRDouble() /= srcFPRDouble();
            ssc();
        }

        OP(R2R_FMOD_S): {
            readRegPair();
            dstFPRSingle() = std::fmod(dstFPRSingle(), srcFPRSingle());
            ssc();
        }

        OP(R2R_FMOD_D): {
            readRegPair();
            dstFPRDouble() = std::fmod(dstFPRDouble(), srcFPRDouble());
            ssc();
        }

        OP(R2R_FSQRT_S): {
            readRegPair();
            dstFPRSingle() = std::sqrt(srcFPRSingle());
            ssc();
        }

        OP(R2R_FSQRT_D): {
            readRegPair();
            dstFPRDouble() = std::sqrt(srcFPRDouble());
            ssc();
        }


        OP(R2R_FMACC_S): {
            // dst += src1 * src2
            uint16 uRegs = *(uint16 const*)puProgramCounter;
            puProgramCounter += sizeof(uint16);
            aoFPR[uRegs & 0x000F].fSingle +=
                aoFPR[(uRegs >> 4) & 0x000F].fSingle *
                aoFPR[(uRegs >> 8) & 0x000F].fSingle;
            ssc();
        }

        OP(R2R_FMACC_D): {
            // dst += src1 * src2
            uint16 uRegs = *(uint16 const*)puProgramCounter;
            puProgramCounter += sizeof(uint16);
            aoFPR[uRegs & 0x000F].fDouble +=
                aoFPR[(uRegs >> 4) & 0x000F].fDouble *
                aoFPR[(uRegs >> 8) & 0x000F].fDouble;
            ssc();
        }

        OP(R2R_FMADD_S): {
            // dst = src1 * src2 + src3
            uint16 uRegs = *(uint16 const*)puProgramCounter;
            puProgramCounter += sizeof(uint16);
            aoFPR[uRegs & 0x000F].fSingle =
                (aoFPR[(uRegs >> 4) & 0x000F].fSingle *
                aoFPR[(uRegs >> 8) & 0x000F].fSingle) +
                aoFPR[(uRegs >> 12) & 0x000F].fSingle;
            ssc();
        }

        OP(R2R_FMADD_D): {
            // dst = src1 * src2 + src3
            uint16 uRegs = *(uint16 const*)puProgramCounter;
            puProgramCounter += sizeof(uint16);
            aoFPR[uRegs & 0x000F].fDouble =
                (aoFPR[(uRegs >> 4) & 0x000F].fDouble *
                aoFPR[(uRegs >> 8) & 0x000F].fDouble) +
                aoFPR[(uRegs >> 12) & 0x000F].fDouble;
            ssc();
        }

        // EA-based path

        OP(MOVE_B): dyadic(SIZE_BYTE); asUByte(pDstEA) = asUByte(pSrcEA); dsc();
        OP(MOVE_W): dyadic(SIZE_WORD); asUWord(pDstEA) = asUWord(pSrcEA); dsc();
        OP(MOVE_L): dyadic(SIZE_LONG); asULong(pDstEA) = asULong(pSrcEA); dsc();
        OP(MOVE_Q): dyadic(SIZE_QUAD); asUQuad(pDstEA) = asUQuad(pSrcEA); dsc();

        // GPR save/restore
        OP(SAVEM): {
            uint8 uEAMode = *puProgramCounter++;
            readMask();
            saveRegisters(uMask, uEAMode);
            dsc();
        }

        OP(LOADM): {
            readMask();
            uint8 uEAMode = *puProgramCounter++;
            restoreRegisters(uMask, uEAMode);
            dsc();
        }

        // Integer/Float interconversion
        OP(FMOVEB_S):
            dyadic2(SIZE_LONG, SIZE_BYTE);
            asSingle(pDstEA) = (float32)asByte(pSrcEA);
            dsc();

        OP(FMOVEB_D):
            dyadic2(SIZE_QUAD, SIZE_BYTE);
            asDouble(pDstEA) = (float64)asByte(pSrcEA);
            dsc();

        OP(FMOVEW_S):
            dyadic2(SIZE_LONG, SIZE_WORD);
            asSingle(pDstEA) = (float32)asWord(pSrcEA);
            dsc();

        OP(FMOVEW_D):
            dyadic2(SIZE_QUAD, SIZE_WORD);
            asDouble(pDstEA) = (float64)asWord(pSrcEA);
            dsc();

        OP(FMOVEL_S):
            dyadic(SIZE_LONG);
            asSingle(pDstEA) = (float32)asLong(pSrcEA);
            dsc();

        OP(FMOVEL_D):
            dyadic2(SIZE_QUAD, SIZE_LONG);
            asDouble(pDstEA) = (float64)asLong(pSrcEA);
            dsc();

        OP(FMOVEQ_S):
            dyadic2(SIZE_LONG, SIZE_QUAD);
            asSingle(pDstEA) = (float32)asQuad(pSrcEA);
            dsc();

        OP(FMOVEQ_D):
            dyadic(SIZE_QUAD);
            asDouble(pDstEA) = (float64)asQuad(pSrcEA);
            dsc();

        OP(FMOVES_L):
            dyadic(SIZE_LONG);
            asLong(pDstEA)   = (int32)asSingle(pSrcEA);
            dsc();

        OP(FMOVES_Q):
            dyadic2(SIZE_QUAD, SIZE_LONG);
            asQuad(pDstEA)   = (int64)asSingle(pSrcEA);
            dsc();

        OP(FMOVES_D):
            dyadic(SIZE_QUAD);
            asDouble(pDstEA) = (float64)asSingle(pSrcEA);
            dsc();

        OP(FMOVED_L):
            dyadic2(SIZE_LONG, SIZE_QUAD);
            asLong(pDstEA)   = (int32)asDouble(pSrcEA);
            dsc();

        OP(FMOVED_Q):
            dyadic(SIZE_QUAD);
            asQuad(pDstEA)   = (int64)asDouble(pSrcEA);
            dsc();

        OP(FMOVED_S):
            dyadic2(SIZE_LONG, SIZE_QUAD);
            asSingle(pDstEA) = (float32)asDouble(pSrcEA);
            dsc();

        // FPR save/restore
        OP(FMOVE_S): {
            dyadic(SIZE_LONG);
            asULong(pDstEA) = asULong(pSrcEA);
            dsc();
        }
        OP(FMOVE_D):
            dyadic(SIZE_QUAD);
            asQuad(pDstEA) = asQuad(pSrcEA);
            dsc();

        OP(FINFO_S): {
            dyadic2(SIZE_BYTE, SIZE_LONG);
            asUByte(pDstEA) = classifyFloat(asSingle(pSrcEA));
            dsc();
        }

        OP(FINFO_D): {
            dyadic2(SIZE_BYTE, SIZE_QUAD);
            asUByte(pDstEA) = classifyFloat(asDouble(pSrcEA));
            dsc();
        }

        // DataMove - clr
        OP(CLR_B):    monadic(SIZE_BYTE); asUByte(pDstEA) = 0; dsc();
        OP(CLR_W):    monadic(SIZE_WORD); asUWord(pDstEA) = 0; dsc();
        OP(CLR_L):    monadic(SIZE_LONG); asULong(pDstEA) = 0; dsc();
        OP(CLR_Q):    monadic(SIZE_QUAD); asUQuad(pDstEA) = 0; dsc();

        OP(LINK): {
            // link r<N>, #d
            readDisplacement();
            readRegPair();
            // sp - 8 -> sp
            aoGPR[GPRegister::SP].puByte -= sizeof(uint64);
            // r<N> -> (sp)
            *(aoGPR[GPRegister::SP].puQuad) = dstGPRUQuad();
            // sp -> r<N>
            dstGPRUQuad() = aoGPR[GPRegister::SP].uQuad;
            // sp + d -> sp
            aoGPR[GPRegister::SP].puByte += iDisplacement;
            ssc();
        }

        OP(UNLK): {
            readRegPair();
            // r<N> -> sp
            aoGPR[GPRegister::SP].uQuad = dstGPRUQuad();
            // (sp) -> r<N>
            dstGPRUQuad() = *(aoGPR[GPRegister::SP].puQuad);
            // sp + 8 -> sp
            aoGPR[GPRegister::SP].puByte += sizeof(uint64);
            ssc();
        }

        OP(LEA): {
            dyadic(SIZE_QUAD);
            asUQuad(pDstEA) = (uint64)pSrcEA;
            dsc();
        }

        OP(PEA): {
            monadic(SIZE_QUAD);
            aoGPR[GPRegister::SP].puByte -= sizeof(uint64);
            *(aoGPR[GPRegister::SP].puQuad) = (uint64)pSrcEA;
            dsc();
        }

        // Compare and set dyadic
        OP(SCM): handleSMC(); dsc();

        // Compare and set dyadic
        OP(SCD): handleSDC(); dsc();

        // Logical
        OP(AND_B):  dyadic(SIZE_BYTE); asUByte(pDstEA) &=   asUByte(pSrcEA);       dsc();
        OP(AND_W):  dyadic(SIZE_WORD); asUWord(pDstEA) &=   asUWord(pSrcEA);       dsc();
        OP(AND_L):  dyadic(SIZE_LONG); asULong(pDstEA) &=   asULong(pSrcEA);       dsc();
        OP(AND_Q):  dyadic(SIZE_QUAD); asUQuad(pDstEA) &=   asUQuad(pSrcEA);       dsc();
        OP(OR_B):   dyadic(SIZE_BYTE); asUByte(pDstEA) |=   asUByte(pSrcEA);       dsc();
        OP(OR_W):   dyadic(SIZE_WORD); asUWord(pDstEA) |=   asUWord(pSrcEA);       dsc();
        OP(OR_L):   dyadic(SIZE_LONG); asULong(pDstEA) |=   asULong(pSrcEA);       dsc();
        OP(OR_Q):   dyadic(SIZE_QUAD); asUQuad(pDstEA) |=   asUQuad(pSrcEA);       dsc();
        OP(EOR_B):  dyadic(SIZE_BYTE); asUByte(pDstEA) ^=   asUByte(pSrcEA);       dsc();
        OP(EOR_W):  dyadic(SIZE_WORD); asUWord(pDstEA) ^=   asUWord(pSrcEA);       dsc();
        OP(EOR_L):  dyadic(SIZE_LONG); asULong(pDstEA) ^=   asULong(pSrcEA);       dsc();
        OP(EOR_Q):  dyadic(SIZE_QUAD); asUQuad(pDstEA) ^=   asUQuad(pSrcEA);       dsc();
        OP(NOT_B):  dyadic(SIZE_BYTE); asUByte(pDstEA) =   ~asUByte(pSrcEA);       dsc();
        OP(NOT_W):  dyadic(SIZE_WORD); asUWord(pDstEA) =   ~asUWord(pSrcEA);       dsc();
        OP(NOT_L):  dyadic(SIZE_LONG); asULong(pDstEA) =   ~asULong(pSrcEA);       dsc();
        OP(NOT_Q):  dyadic(SIZE_QUAD); asUQuad(pDstEA) =   ~asUQuad(pSrcEA);       dsc();
        OP(LSL_B):  dyadic(SIZE_BYTE); asUByte(pDstEA) <<= (asUByte(pSrcEA) & 7);  dsc();
        OP(LSL_W):  dyadic(SIZE_WORD); asUWord(pDstEA) <<= (asUByte(pSrcEA) & 15); dsc();
        OP(LSL_L):  dyadic(SIZE_LONG); asULong(pDstEA) <<= (asUByte(pSrcEA) & 31); dsc();
        OP(LSL_Q):  dyadic(SIZE_QUAD); asUQuad(pDstEA) <<= (asUByte(pSrcEA) & 63); dsc();
        OP(LSR_B):  dyadic(SIZE_BYTE); asUByte(pDstEA) >>= (asUByte(pSrcEA) & 7);  dsc();
        OP(LSR_W):  dyadic(SIZE_WORD); asUWord(pDstEA) >>= (asUByte(pSrcEA) & 15); dsc();
        OP(LSR_L):  dyadic(SIZE_LONG); asULong(pDstEA) >>= (asUByte(pSrcEA) & 31); dsc();
        OP(LSR_Q):  dyadic(SIZE_QUAD); asUQuad(pDstEA) >>= (asUByte(pSrcEA) & 63); dsc();
        OP(ROL_B):  dyadic(SIZE_BYTE); rolByte((uint8*)pDstEA,  asUByte(pSrcEA));  dsc();
        OP(ROL_W):  dyadic(SIZE_WORD); rolWord((uint16*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(ROL_L):  dyadic(SIZE_LONG); rolLong((uint32*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(ROL_Q):  dyadic(SIZE_QUAD); rolQuad((uint64*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(ROR_B):  dyadic(SIZE_BYTE); rorByte((uint8*)pDstEA,  asUByte(pSrcEA));  dsc();
        OP(ROR_W):  dyadic(SIZE_WORD); rorWord((uint16*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(ROR_L):  dyadic(SIZE_LONG); rorLong((uint32*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(ROR_Q):  dyadic(SIZE_QUAD); rorQuad((uint64*)pDstEA, asUByte(pSrcEA));  dsc();
        OP(BCLR_B): dyadic(SIZE_BYTE); asUByte(pDstEA) &= (uint8)  ~(1 << (asUByte(pSrcEA) & 7));  dsc();
        OP(BCLR_W): dyadic(SIZE_WORD); asUWord(pDstEA) &= (uint16) ~(1 << (asUByte(pSrcEA) & 15)); dsc();
        OP(BCLR_L): dyadic(SIZE_LONG); asULong(pDstEA) &= (uint32) ~(1 << (asUByte(pSrcEA) & 31)); dsc();
        OP(BCLR_Q): dyadic(SIZE_QUAD); asUQuad(pDstEA) &= (uint64) ~(1 << (asUByte(pSrcEA) & 63)); dsc();
        OP(BSET_B): dyadic(SIZE_BYTE); asUByte(pDstEA) |= (uint8)  (1 << (asUByte(pSrcEA) & 7));  dsc();
        OP(BSET_W): dyadic(SIZE_WORD); asUWord(pDstEA) |= (uint16) (1 << (asUByte(pSrcEA) & 15)); dsc();
        OP(BSET_L): dyadic(SIZE_LONG); asULong(pDstEA) |= (uint32) (1 << (asUByte(pSrcEA) & 31)); dsc();
        OP(BSET_Q): dyadic(SIZE_QUAD); asUQuad(pDstEA) |= (uint64) (1 << (asUByte(pSrcEA) & 63)); dsc();

        OP(EXTB_W): dyadic(SIZE_WORD); asWord(pDstEA)    = (int16)asByte(pSrcEA);   dsc();
        OP(EXTB_L): dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asByte(pSrcEA);   dsc();
        OP(EXTB_Q): dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asByte(pSrcEA);   dsc();
        OP(EXTW_L): dyadic(SIZE_LONG); asLong(pDstEA)    = (int32)asWord(pSrcEA);   dsc();
        OP(EXTW_Q): dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asWord(pSrcEA);   dsc();
        OP(EXTL_Q): dyadic(SIZE_QUAD); asQuad(pDstEA)    = (int64)asLong(pSrcEA);   dsc();
        OP(ASL_B):  dyadic(SIZE_BYTE); asByte(pDstEA)   <<= (asUByte(pSrcEA) & 7);  dsc();
        OP(ASL_W):  dyadic(SIZE_WORD); asWord(pDstEA)   <<= (asUByte(pSrcEA) & 15); dsc();
        OP(ASL_L):  dyadic(SIZE_LONG); asLong(pDstEA)   <<= (asUByte(pSrcEA) & 31); dsc();
        OP(ASL_Q):  dyadic(SIZE_QUAD); asQuad(pDstEA)   <<= (asUByte(pSrcEA) & 63); dsc();
        OP(ASR_B):  dyadic(SIZE_BYTE); asByte(pDstEA)   >>= (asUByte(pSrcEA) & 7);  dsc();
        OP(ASR_W):  dyadic(SIZE_WORD); asWord(pDstEA)   >>= (asUByte(pSrcEA) & 15); dsc();
        OP(ASR_L):  dyadic(SIZE_LONG); asLong(pDstEA)   >>= (asUByte(pSrcEA) & 31); dsc();
        OP(ASR_Q):  dyadic(SIZE_QUAD); asQuad(pDstEA)   >>= (asUByte(pSrcEA) & 63); dsc();
        OP(ADD_B):  dyadic(SIZE_BYTE); asByte(pDstEA)   +=   asByte(pSrcEA);        dsc();
        OP(ADD_W):  dyadic(SIZE_WORD); asWord(pDstEA)   +=   asWord(pSrcEA);        dsc();
        OP(ADD_L):  dyadic(SIZE_LONG); asLong(pDstEA)   +=   asLong(pSrcEA);        dsc();
        OP(ADD_Q):  dyadic(SIZE_QUAD); asQuad(pDstEA)   +=   asQuad(pSrcEA);        dsc();
        OP(FADD_S): dyadic(SIZE_LONG); asSingle(pDstEA) +=   asSingle(pSrcEA);      dsc();
        OP(FADD_D): dyadic(SIZE_QUAD); asDouble(pDstEA) +=   asDouble(pSrcEA);      dsc();
        OP(SUB_B):  dyadic(SIZE_BYTE); asByte(pDstEA)   -=   asByte(pSrcEA);        dsc();
        OP(SUB_W):  dyadic(SIZE_WORD); asWord(pDstEA)   -=   asWord(pSrcEA);        dsc();
        OP(SUB_L):  dyadic(SIZE_LONG); asLong(pDstEA)   -=   asLong(pSrcEA);        dsc();
        OP(SUB_Q):  dyadic(SIZE_QUAD); asQuad(pDstEA)   -=   asQuad(pSrcEA);        dsc();
        OP(FSUB_S): dyadic(SIZE_LONG); asSingle(pDstEA) -=   asSingle(pSrcEA);      dsc();
        OP(FSUB_D): dyadic(SIZE_QUAD); asDouble(pDstEA) -=   asDouble(pSrcEA);      dsc();
        OP(NEG_B):  dyadic(SIZE_BYTE); asByte(pDstEA)    =  -asByte(pSrcEA);        dsc();
        OP(NEG_W):  dyadic(SIZE_WORD); asWord(pDstEA)    =  -asWord(pSrcEA);        dsc();
        OP(NEG_L):  dyadic(SIZE_LONG); asLong(pDstEA)    =  -asLong(pSrcEA);        dsc();
        OP(NEG_Q):  dyadic(SIZE_QUAD); asQuad(pDstEA)    =  -asQuad(pSrcEA);        dsc();
        OP(FNEG_S): dyadic(SIZE_LONG); asSingle(pDstEA)  =  -asSingle(pSrcEA);      dsc();
        OP(FNEG_D): dyadic(SIZE_QUAD); asDouble(pDstEA)  =  -asDouble(pSrcEA);      dsc();
        OP(MULS_B): dyadic(SIZE_BYTE); asByte(pDstEA)   *=   asByte(pSrcEA);        dsc();
        OP(MULS_W): dyadic(SIZE_WORD); asWord(pDstEA)   *=   asWord(pSrcEA);        dsc();
        OP(MULS_L): dyadic(SIZE_LONG); asLong(pDstEA)   *=   asLong(pSrcEA);        dsc();
        OP(MULS_Q): dyadic(SIZE_QUAD); asQuad(pDstEA)   *=   asQuad(pSrcEA);        dsc();
        OP(MULU_B): dyadic(SIZE_BYTE); asUByte(pDstEA)  *=   asUByte(pSrcEA);       dsc();
        OP(MULU_W): dyadic(SIZE_WORD); asUWord(pDstEA)  *=   asUWord(pSrcEA);       dsc();
        OP(MULU_L): dyadic(SIZE_LONG); asULong(pDstEA)  *=   asULong(pSrcEA);       dsc();
        OP(MULU_Q): dyadic(SIZE_QUAD); asUQuad(pDstEA)  *=   asUQuad(pSrcEA);       dsc();
        OP(FMUL_S): dyadic(SIZE_LONG); asSingle(pDstEA) *=   asSingle(pSrcEA);      dsc();
        OP(FMUL_D): dyadic(SIZE_QUAD); asDouble(pDstEA) *=   asDouble(pSrcEA);      dsc();
        OP(DIVS_L): dyadic(SIZE_BYTE); asLong(pDstEA)   /=   asLong(pSrcEA);        dsc();
        OP(DIVS_Q): dyadic(SIZE_WORD); asQuad(pDstEA)   /=   asQuad(pSrcEA);        dsc();
        OP(MODS_L): dyadic(SIZE_LONG); asLong(pDstEA)   %=   asLong(pSrcEA);        dsc();
        OP(MODS_Q): dyadic(SIZE_QUAD); asQuad(pDstEA)   %=   asQuad(pSrcEA);        dsc();
        OP(DIVU_L): dyadic(SIZE_BYTE); asULong(pDstEA)  /=   asULong(pSrcEA);       dsc();
        OP(DIVU_Q): dyadic(SIZE_WORD); asUQuad(pDstEA)  /=   asUQuad(pSrcEA);       dsc();
        OP(MODU_L): dyadic(SIZE_LONG); asULong(pDstEA)  %=   asULong(pSrcEA);       dsc();
        OP(MODU_Q): dyadic(SIZE_QUAD); asUQuad(pDstEA)  %=   asUQuad(pSrcEA);       dsc();
        OP(FDIV_S): dyadic(SIZE_LONG); asSingle(pDstEA) /=   asSingle(pSrcEA);      dsc();
        OP(FDIV_D): dyadic(SIZE_QUAD); asDouble(pDstEA) /=   asDouble(pSrcEA);      dsc();

        OP(FMOD_S): {
            dyadic(SIZE_LONG);
            float32 f = asSingle(pDstEA);
            asSingle(pDstEA) = std::fmod(f, asSingle(pSrcEA));
            dsc();
        }

        OP(FMOD_D): {
            dyadic(SIZE_QUAD);
            float64 f = asDouble(pDstEA);
            asDouble(pDstEA) = std::fmod(f, asDouble(pSrcEA));
            dsc();
        }

        OP(FABS_S):    dyadic(SIZE_LONG); asSingle(pDstEA) = std::fabs(asSingle(pSrcEA)); dsc();
        OP(FABS_D):    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::fabs(asDouble(pSrcEA)); dsc();
        OP(FSQRT_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::sqrt(asSingle(pSrcEA)); dsc();
        OP(FSQRT_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::sqrt(asDouble(pSrcEA)); dsc();
        OP(FACOS_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::acos(asSingle(pSrcEA)); dsc();
        OP(FACOS_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::acos(asDouble(pSrcEA)); dsc();
        OP(FASIN_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::asin(asSingle(pSrcEA)); dsc();
        OP(FASIN_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::asin(asDouble(pSrcEA)); dsc();
        OP(FATAN_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::atan(asSingle(pSrcEA)); dsc();
        OP(FATAN_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::atan(asDouble(pSrcEA)); dsc();
        OP(FCOS_S):    dyadic(SIZE_LONG); asSingle(pDstEA) = std::cos(asSingle(pSrcEA));  dsc();
        OP(FCOS_D):    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::cos(asDouble(pSrcEA));  dsc();
        OP(FSIN_S):    dyadic(SIZE_LONG); asSingle(pDstEA) = std::sin(asSingle(pSrcEA));  dsc();
        OP(FSIN_D):    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::sin(asDouble(pSrcEA));  dsc();

        OP(FSINCOS_S): {
            dyadic(SIZE_LONG);
            float32 f = asSingle(pSrcEA);
            asSingle(pDstEA) = std::sin(f);
            asSingle(pSrcEA) = std::cos(f);
            dsc();
        }

        OP(FSINCOS_D): {
            dyadic(SIZE_QUAD);
            float64 f = asDouble(pSrcEA);
            asDouble(pDstEA) = std::sin(f);
            asDouble(pSrcEA) = std::cos(f);
            dsc();
        }

        OP(FTAN_S):    dyadic(SIZE_LONG); asSingle(pDstEA) = std::tan(asSingle(pSrcEA));  dsc();
        OP(FTAN_D):    dyadic(SIZE_QUAD); asDouble(pDstEA) = std::tan(asDouble(pSrcEA));  dsc();
        OP(FETOX_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::exp(asSingle(pSrcEA));  dsc();
        OP(FETOX_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::exp(asDouble(pSrcEA));  dsc();
        OP(FLOGN_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::log(asSingle(pSrcEA));  dsc();
        OP(FLOGN_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::log(asDouble(pSrcEA));  dsc();
        OP(FLOG2_S):   dyadic(SIZE_LONG); asSingle(pDstEA) = std::log2(asSingle(pSrcEA)); dsc();
        OP(FLOG2_D):   dyadic(SIZE_QUAD); asDouble(pDstEA) = std::log2(asDouble(pSrcEA)); dsc();
        OP(FTWOTOX_S): dyadic(SIZE_LONG); asSingle(pDstEA) = std::exp2(asSingle(pSrcEA)); dsc();
        OP(FTWOTOX_D): dyadic(SIZE_QUAD); asDouble(pDstEA) = std::exp2(asDouble(pSrcEA)); dsc();

        OP(0xF0):
            aoGPR[14].uQuad = Nanoseconds::mark();
            ssc();

        OP(BAD):
            eStatus = UNIMPLEMENTED_OPCODE;
    }
    end_interpreter:
    return;
}

}

