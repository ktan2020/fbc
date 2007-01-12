#ifndef __AST_OP_BI__
#define __AST_OP_BI__

''	FreeBASIC - 32-bit BASIC Compiler.
''	Copyright (C) 2004-2007 The FreeBASIC development team.
''
''	This program is free software; you can redistribute it and/or modify
''	it under the terms of the GNU General Public License as published by
''	the Free Software Foundation; either version 2 of the License, or
''	(at your option) any later version.
''
''	This program is distributed in the hope that it will be useful,
''	but WITHOUT ANY WARRANTY; without even the implied warranty of
''	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
''	GNU General Public License for more details.
''
''	You should have received a copy of the GNU General Public License
''	along with this program; if not, write to the Free Software
''	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA.


'' if changed, update ast_opTB at ast.bas
enum AST_OP
	'' ini self ops (see below: AST_OP_SELFBASE and AST_OP_SELFOPS)
	AST_OP_ASSIGN		= 0
	AST_OP_ADD_SELF
	AST_OP_SUB_SELF
	AST_OP_MUL_SELF
	AST_OP_DIV_SELF
	AST_OP_INTDIV_SELF
	AST_OP_MOD_SELF
	AST_OP_AND_SELF
	AST_OP_OR_SELF
	AST_OP_XOR_SELF
	AST_OP_EQV_SELF
	AST_OP_IMP_SELF
	AST_OP_SHL_SELF
	AST_OP_SHR_SELF
	AST_OP_POW_SELF
	AST_OP_CONCAT_SELF
	AST_OP_NEW_SELF
	AST_OP_NEW_VEC_SELF
	AST_OP_DEL_SELF
	AST_OP_DEL_VEC_SELF
	AST_OP_ADDROF
	AST_OP_FOR
	AST_OP_STEP
	AST_OP_NEXT
	AST_OP_CAST
	'' end self ops

	AST_OP_ADD
	AST_OP_SUB
	AST_OP_MUL
	AST_OP_DIV
	AST_OP_INTDIV
	AST_OP_MOD
	AST_OP_AND
	AST_OP_OR
	AST_OP_XOR
	AST_OP_EQV
	AST_OP_IMP
	AST_OP_SHL
	AST_OP_SHR
	AST_OP_POW
	AST_OP_CONCAT

	AST_OP_EQ
	AST_OP_GT
	AST_OP_LT
	AST_OP_NE
	AST_OP_GE
	AST_OP_LE

	AST_OP_NOT
	AST_OP_PLUS
	AST_OP_NEG

	AST_OP_ABS
	AST_OP_SGN
	AST_OP_SIN
	AST_OP_ASIN
	AST_OP_COS
	AST_OP_ACOS
	AST_OP_TAN
	AST_OP_ATAN
	AST_OP_ATAN2
	AST_OP_SQRT
	AST_OP_LOG
	AST_OP_EXP
	AST_OP_FLOOR
	AST_OP_FIX
	AST_OP_FRAC

	AST_OP_DEREF
	AST_OP_FLDDEREF

	AST_OP_NEW									'' global
	AST_OP_NEW_VEC								'' /
	AST_OP_DEL                                  '' /
	AST_OP_DEL_VEC                              '' /

	AST_OP_TOINT
	AST_OP_TOFLT

	AST_OP_LOAD
	AST_OP_LOADRES

	AST_OP_SPILLREGS

	AST_OP_PUSH
	AST_OP_POP
	AST_OP_PUSHUDT
	AST_OP_STACKALIGN

	AST_OP_JEQ
	AST_OP_JGT
	AST_OP_JLT
	AST_OP_JNE
	AST_OP_JGE
	AST_OP_JLE
	AST_OP_JMP
	AST_OP_CALL

	AST_OP_LABEL
	AST_OP_RET
	AST_OP_CALLFUNCT
	AST_OP_CALLPTR
	AST_OP_JUMPPTR

	AST_OP_MEMMOVE
	AST_OP_MEMSWAP
	AST_OP_MEMCLEAR
	AST_OP_STKCLEAR

	AST_OP_DBG_LINEINI
	AST_OP_DBG_LINEEND
	AST_OP_DBG_SCOPEINI
	AST_OP_DBG_SCOPEEND

	AST_OP_LIT_COMMENT
	AST_OP_LIT_ASM

	'' used internally, not passed to IR
	AST_OP_TOSIGNED
	AST_OP_TOUNSIGNED

	AST_OPCODES									'' total
end enum

const AST_OP_SELFBASE = AST_OP_ASSIGN
const AST_OP_SELFOPS  = (AST_OP_CAST - AST_OP_SELFBASE) + 1

#endif '' __AST_OP_BI__
