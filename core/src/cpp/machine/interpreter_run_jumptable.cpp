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

#ifndef HAVE_PTR_DEPS
extern uint8 const* puProgramCounter;
extern void* pDstEA;
#endif

// Super gnarly macros for custom jump table

#define status()   goto begin_interpreter
#define end()      goto end_interpreter
#define dispatch() goto *((uint8*)&&begin_interpreter + uJumpTable[*puProgramCounter++])

#ifdef THREADED_DISPATCH
    #define SKIP_STATUS
    #define next() dispatch()
#else
    #define SKIP_STATUS skip_status_check:
    #define next() goto skip_status_check
#endif

// Operation Name
#define OP(NAME) op_ ## NAME

// Define Operation
#define defOp(NAME) op_ ## NAME:

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

    /**
     * Custom 16 bit PC relative jump table
     */
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

        #include <machine/opcode_handlers.hpp>

        // Super undocumented timing opcode ftw
        defOp(0xF0) {
            aoGPR[14].uQuad = Nanoseconds::mark();
            next();
        }

        defOp(BAD) {
            eStatus = UNIMPLEMENTED_OPCODE;
        }
    }
    end_interpreter:
    return;
}

}

