#define MAX_OPERANDS 5
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <tables.h>
#include <opflags.h>
#include <insnsi.h>

#define OP_GENVAL(val, bits, shift)     (((val) & ((UINT64_C(1) << (bits)) - 1)) << (shift))

/*
 *  * Opmask register number
 *   * identical to EVEX.aaa
 *    *
 *     * Bits: 0 - 3
 *      */
#define OPMASK_SHIFT            (0)
#define OPMASK_BITS             (4)
#define OPMASK_MASK             OP_GENMASK(OPMASK_BITS, OPMASK_SHIFT)
#define GEN_OPMASK(bit)         OP_GENBIT(bit, OPMASK_SHIFT)
#define VAL_OPMASK(val)         OP_GENVAL(val, OPMASK_BITS, OPMASK_SHIFT)

/*
 *  * zeroing / merging control available
 *   * matching to EVEX.z
 *    *
 *     * Bits: 4
 *      */
#define Z_SHIFT                 (4)
#define Z_BITS                  (1)
#define Z_MASK                  OP_GENMASK(Z_BITS, Z_SHIFT)
#define GEN_Z(bit)              OP_GENBIT(bit, Z_SHIFT)

/*
 *  * broadcast - Whether this operand can be broadcasted
 *   *
 *    * Bits: 5
 *     */
#define BRDCAST_SHIFT           (5)
#define BRDCAST_BITS            (1)
#define BRDCAST_MASK            OP_GENMASK(BRDCAST_BITS, BRDCAST_SHIFT)
#define GEN_BRDCAST(bit)        OP_GENBIT(bit, BRDCAST_SHIFT)

/*
 *  * Whether this instruction can have a static rounding mode.
 *   * It goes with the last simd operand because the static rounding mode
 *    * decorator is located between the last simd operand and imm8 (if any).
 *     *
 *      * Bits: 6
 *       */
#define STATICRND_SHIFT         (6)
#define STATICRND_BITS          (1)
#define STATICRND_MASK          OP_GENMASK(STATICRND_BITS, STATICRND_SHIFT)
#define GEN_STATICRND(bit)      OP_GENBIT(bit, STATICRND_SHIFT)

/*
 *  * SAE(Suppress all exception) available
 *   *
 *    * Bits: 7
 *     */
#define SAE_SHIFT               (7)
#define SAE_BITS                (1)
#define SAE_MASK                OP_GENMASK(SAE_BITS, SAE_SHIFT)
#define GEN_SAE(bit)            OP_GENBIT(bit, SAE_SHIFT)

/*
 *  * Broadcasting element size.
 *   *
 *    * Bits: 8 - 9
 *     */
#define BRSIZE_SHIFT            (8)
#define BRSIZE_BITS             (2)
#define BRSIZE_MASK             OP_GENMASK(BRSIZE_BITS, BRSIZE_SHIFT)
#define GEN_BRSIZE(bit)         OP_GENBIT(bit, BRSIZE_SHIFT)

#define BR_BITS32               GEN_BRSIZE(0)
#define BR_BITS64               GEN_BRSIZE(1)

/*
 *  * Number of broadcasting elements
 *   *
 *    * Bits: 10 - 11
 *     */

#define BRNUM_SHIFT             (10)
#define BRNUM_BITS              (2)
#define BRNUM_MASK              OP_GENMASK(BRNUM_BITS, BRNUM_SHIFT)
#define VAL_BRNUM(val)          OP_GENVAL(val, BRNUM_BITS, BRNUM_SHIFT)

#define BR_1TO2                 VAL_BRNUM(0)
#define BR_1TO4                 VAL_BRNUM(1)
#define BR_1TO8                 VAL_BRNUM(2)
#define BR_1TO16                VAL_BRNUM(3)

#define MASK                    OPMASK_MASK             /* Opmask (k1 ~ 7) can be used */
#define Z                       Z_MASK
#define B32                     (BRDCAST_MASK|BR_BITS32) /* {1to16} : broadcast 32b * 16 to zmm(512b) */
#define B64                     (BRDCAST_MASK|BR_BITS64) /* {1to8}  : broadcast 64b *  8 to zmm(512b) */
#define ER                      STATICRND_MASK          /* ER(Embedded Rounding) == Static rounding mode */
#define SAE                     SAE_MASK                /* SAE(Suppress All Exception) */
