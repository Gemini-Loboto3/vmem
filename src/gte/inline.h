#ifndef __GTE_INLINE2_H
#define __GTE_INLINE2_H

// macros for DQA and DQB
#define gte_SetDQA( r0 ) __asm__ volatile (		\
	"ctc2	%0, $27;"		\
	:						\
	: "r"( r0 ) )

#define gte_SetDQB( r0 ) __asm__ volatile (		\
	"ctc2	%0, $28;"		\
	:						\
	: "r"( r0 ) )

// a few extra macros for faster code
// these don't abuse the stack to store GTE calculation results
#define gte_stotz_m( r0 ) __asm__ volatile (	\
	"mfc2	%0, $7;"		\
	: "=r"( r0 )			\
	:)

#define gte_stflg_m( r0 ) __asm__ volatile (	\
	"mfc2	%0, $31;"		\
	: "=r"( r0 )			\
	:)

#define gte_stopz_m( r0 ) __asm__ volatile (	\
	"mfc2	%0, $24;"		\
	: "=r"( r0 )			\
	:)

// direct access to POLY_GT4.rgb3
#define gte_strr3_gt4( r0 ) __asm__ volatile (	\
	"swc2	$22, 40( %0 );"	\
	:						\
	: "r"( r0 )				\
	: "memory" )

// direct access to POLY_GT4.xy3
#define gte_stsxy_gt4_3( r0 ) __asm__ volatile (\
	"swc2	$14, 44( %0 )"	\
	:						\
	: "r"( r0 )				\
	: "memory" )

// direct access to POLY_G4.rgb3
#define gte_strr3_g4( r0 ) __asm__ volatile (	\
	"swc2	$22, 28( %0 );"	\
	:						\
	: "r"( r0 )				\
	: "memory" )

// direct access to POLY_G4.xy3
#define gte_stsxy_g4_3( r0 ) __asm__ volatile (\
	"swc2	$14, 32( %0 )"	\
	:						\
	: "r"( r0 )				\
	: "memory" )

#endif
