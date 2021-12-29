/*
Copyright (c) 2013, Jurriaan Bremer
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of the darm developer(s) nor the names of its
  contributors may be used to endorse or promote products derived from this
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "thumb-tbl.h"
darm_enctype_t thumb_instr_types[] = {
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM,
    T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_SHIFT_IMM, T_THUMB_3REG,
    T_THUMB_3REG, T_THUMB_3REG, T_THUMB_3REG, T_THUMB_2REG_IMM,
    T_THUMB_2REG_IMM, T_THUMB_2REG_IMM, T_THUMB_2REG_IMM, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_GPI,
    T_THUMB_GPI, T_THUMB_GPI, T_THUMB_GPI, T_THUMB_MOD_SP_REG, T_THUMB_CMP,
    T_THUMB_MOV4, T_THUMB_BRANCH_REG, T_THUMB_LDR_PC, T_THUMB_LDR_PC,
    T_THUMB_LDR_PC, T_THUMB_LDR_PC, T_THUMB_LDR_PC, T_THUMB_LDR_PC,
    T_THUMB_LDR_PC, T_THUMB_LDR_PC, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO,
    T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO,
    T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO,
    T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMO,
    T_THUMB_RW_MEMO, T_THUMB_RW_MEMO, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_RW_MEMI,
    T_THUMB_RW_MEMI, T_THUMB_RW_MEMI, T_THUMB_STACK, T_THUMB_STACK,
    T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK,
    T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK,
    T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK, T_THUMB_STACK,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8, T_THUMB_HAS_IMM8,
    T_THUMB_ADD_SP_IMM, T_THUMB_ADD_SP_IMM, T_THUMB_ADD_SP_IMM,
    T_THUMB_ADD_SP_IMM, T_THUMB_ADD_SP_IMM, T_THUMB_ADD_SP_IMM,
    T_THUMB_ADD_SP_IMM, T_THUMB_ADD_SP_IMM, T_THUMB_MOD_SP_IMM, T_THUMB_CBZ,
    T_THUMB_EXTEND, T_THUMB_CBZ, T_THUMB_PUSHPOP, T_THUMB_PUSHPOP,
    T_THUMB_SETEND, T_INVLD, T_INVLD, T_THUMB_CBZ, T_THUMB_REV, T_THUMB_CBZ,
    T_THUMB_PUSHPOP, T_THUMB_PUSHPOP, T_THUMB_ONLY_IMM8, T_THUMB_IT_HINTS,
    T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG,
    T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG,
    T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG,
    T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG, T_THUMB_RW_REG,
    T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH,
    T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH,
    T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH,
    T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH,
    T_THUMB_COND_BRANCH, T_THUMB_COND_BRANCH, T_THUMB_ONLY_IMM8,
    T_THUMB_ONLY_IMM8, T_THUMB_UNCOND_BRANCH, T_THUMB_UNCOND_BRANCH,
    T_THUMB_UNCOND_BRANCH, T_THUMB_UNCOND_BRANCH, T_THUMB_UNCOND_BRANCH,
    T_THUMB_UNCOND_BRANCH, T_THUMB_UNCOND_BRANCH, T_THUMB_UNCOND_BRANCH,
    T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD,
    T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD,
    T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD, T_INVLD
};

darm_instr_t thumb_instr_labels[] = {
    I_LSL, I_LSL, I_LSL, I_LSL, I_LSL, I_LSL, I_LSL, I_LSL, I_LSR, I_LSR,
    I_LSR, I_LSR, I_LSR, I_LSR, I_LSR, I_LSR, I_ASR, I_ASR, I_ASR, I_ASR,
    I_ASR, I_ASR, I_ASR, I_ASR, I_ADD, I_ADD, I_SUB, I_SUB, I_ADD, I_ADD,
    I_SUB, I_SUB, I_MOV, I_MOV, I_MOV, I_MOV, I_MOV, I_MOV, I_MOV, I_MOV,
    I_CMP, I_CMP, I_CMP, I_CMP, I_CMP, I_CMP, I_CMP, I_CMP, I_ADD, I_ADD,
    I_ADD, I_ADD, I_ADD, I_ADD, I_ADD, I_ADD, I_SUB, I_SUB, I_SUB, I_SUB,
    I_SUB, I_SUB, I_SUB, I_SUB, I_LSR, I_SBC, I_TST, I_ORR, I_ADD, I_CMP,
    I_MOV, I_BX, I_LDR, I_LDR, I_LDR, I_LDR, I_LDR, I_LDR, I_LDR, I_LDR,
    I_STR, I_STR, I_STRH, I_STRH, I_STRB, I_STRB, I_LDRSB, I_LDRSB, I_LDR,
    I_LDR, I_LDRH, I_LDRH, I_LDRB, I_LDRB, I_LDRSH, I_LDRSH, I_STR, I_STR,
    I_STR, I_STR, I_STR, I_STR, I_STR, I_STR, I_LDR, I_LDR, I_LDR, I_LDR,
    I_LDR, I_LDR, I_LDR, I_LDR, I_STRB, I_STRB, I_STRB, I_STRB, I_STRB,
    I_STRB, I_STRB, I_STRB, I_LDRB, I_LDRB, I_LDRB, I_LDRB, I_LDRB, I_LDRB,
    I_LDRB, I_LDRB, I_STRH, I_STRH, I_STRH, I_STRH, I_STRH, I_STRH, I_STRH,
    I_STRH, I_LDRH, I_LDRH, I_LDRH, I_LDRH, I_LDRH, I_LDRH, I_LDRH, I_LDRH,
    I_STR, I_STR, I_STR, I_STR, I_STR, I_STR, I_STR, I_STR, I_LDR, I_LDR,
    I_LDR, I_LDR, I_LDR, I_LDR, I_LDR, I_LDR, I_ADR, I_ADR, I_ADR, I_ADR,
    I_ADR, I_ADR, I_ADR, I_ADR, I_ADD, I_ADD, I_ADD, I_ADD, I_ADD, I_ADD,
    I_ADD, I_ADD, I_SUB, I_CBZ, I_UXTH, I_CBZ, I_PUSH, I_PUSH, I_SETEND,
    I_INVLD, I_INVLD, I_CBZ, I_REVSH, I_CBZ, I_POP, I_POP, I_BKPT, I_YIELD,
    I_STM, I_STM, I_STM, I_STM, I_STM, I_STM, I_STM, I_STM, I_LDM, I_LDM,
    I_LDM, I_LDM, I_LDM, I_LDM, I_LDM, I_LDM, I_B, I_B, I_B, I_B, I_B, I_B,
    I_B, I_B, I_B, I_B, I_B, I_B, I_B, I_B, I_UDF, I_SVC, I_B, I_B, I_B, I_B,
    I_B, I_B, I_B, I_B, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD,
    I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD,
    I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD, I_INVLD,
    I_INVLD, I_INVLD
};

darm_instr_t type_gpi_instr_lookup[] = {
    I_AND, I_EOR, I_LSL, I_LSR, I_ASR, I_ADC, I_SBC, I_ROR, I_TST, I_RSB,
    I_CMP, I_CMN, I_ORR, I_MUL, I_BIC, I_MVN
};

darm_instr_t type_hints_instr_lookup[] = {
    I_NOP, I_YIELD, I_WFE, I_WFI, I_SEV
};

darm_instr_t type_extend_instr_lookup[] = {
    I_SXTH, I_SXTB, I_UXTH, I_UXTB
};

darm_instr_t type_rev_instr_lookup[] = {
    I_REV, I_REV16, I_INVLD, I_REVSH
};

