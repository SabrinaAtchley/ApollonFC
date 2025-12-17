#include "q16x16.h"

namespace {
  static inline Q16x16 wordPairToQ16x16(const uint16_t* v) {
    return ( ((uint32_t) v[1]) << 16 ) | ( (uint32_t) v[0]);
  }
}

Q16x16 q16x16_add_s(const Q16x16 a, const Q16x16 b) {
  uint16_t result[2];

  __asm__ volatile (
    // Load operands into registers
    // movw is available on the ATMEGA328P, but not all AVR boards
    "movw r22, %[a_low]\n\t"  // r23:r22 <- a[15:0]
    "movw r24, %[a_high]\n\t" // r25:r24 <- a[31:16]
    "movw r18, %[b_low]\n\t"  // r19:r18 <- b[15:0]
    "movw r20, %[b_high]\n\t" // r21:r20 <- b[31:16]

    // Add our operands
    "add r22, r18\n\t"
    "adc r23, r19\n\t"
    "adc r24, r20\n\t"
    "adc r25, r21\n\t"

    // Check for overflow
    "brvc .exit%=\n\t" // No overflow
    "brbs 4, .clamp_min%=\n\t" // S bit is high (underflow occurred)

    // Clamp max
    // Return 0x7FFF FFFF (Q16.16 max)
    "ldi r22, 0xFF\n\t"
    "ldi r23, 0xFF\n\t"
    "ldi r24, 0xFF\n\t"
    "ldi r25, 0x7F\n\t"
    "rjmp .exit%=\n\t"

    ".clamp_min%=:\n\t"
    // Return 0x8000 0000 (Q16.16 min)
    "ldi r22, 0x00\n\t"
    "ldi r23, 0x00\n\t"
    "ldi r24, 0x00\n\t"
    "ldi r25, 0x80\n\t"

    ".exit%=:\n\t"
    "movw %[result_low], r22\n\t"
    "movw %[result_high], r24\n\t"

    : [result_low] "=r" ( result[0] ), // Outputs
      [result_high] "=r" ( result[1] )
    : [a_low] "r" ( (uint16_t) (a & 0xFFFF) ), // Inputs
      [a_high] "r" ( (uint16_t) (a >> 16) ),
      [b_low] "r" ( (uint16_t) (b & 0xFFFF) ),
      [b_high] "r" ( (uint16_t) (b >> 16) )
    : "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "cc" // Clobbers
  );

  return wordPairToQ16x16(result);
}


Q16x16 q16x16_sub_s(const Q16x16 a, const Q16x16 b) {
  uint16_t result[2];

  __asm__ volatile (
    // Load operands into registers
    // movw is available on the ATMEGA328P, but not all AVR boards
    "movw r22, %[a_low]\n\t"  // r23:r22 <- a[15:0]
    "movw r24, %[a_high]\n\t" // r25:r24 <- a[31:16]
    "movw r18, %[b_low]\n\t"  // r19:r18 <- b[15:0]
    "movw r20, %[b_high]\n\t" // r21:r20 <- b[31:16]

    // Subtract our numbers
    "sub r22, r18\n\t"
    "sbc r23, r19\n\t"
    "sbc r24, r20\n\t"
    "sbc r25, r21\n\t"

    // Check for overflow
    "brvc .exit%=\n\t" // Jump if no over/underflow occurred
    "brbs 4, .clamp_min%=\n\t" // Underflow occurred

    // Clamp max
    // Return 0x7FFF FFFF (Q16.16 max)
    "ldi r22, 0xFF\n\t"
    "ldi r23, 0xFF\n\t"
    "ldi r24, 0xFF\n\t"
    "ldi r25, 0x7F\n\t"
    "rjmp .exit%=\n\t"

    ".clamp_min%=:\n\t"
    // Return 0x8000 0000 (Q16.16 min)
    "ldi r22, 0x00\n\t"
    "ldi r23, 0x00\n\t"
    "ldi r24, 0x00\n\t"
    "ldi r25, 0x80\n\t"

    ".exit%=:\n\t"
    "movw %[result_low], r22\n\t"
    "movw %[result_high], r24\n\t"

    : [result_low] "=r" ( result[0] ), // Outputs
      [result_high] "=r" ( result[1] )
    : [a_low] "r" ( (uint16_t) (a & 0xFFFF) ), // Inputs
      [a_high] "r" ( (uint16_t) (a >> 16) ),
      [b_low] "r" ( (uint16_t) (b & 0xFFFF) ),
      [b_high] "r" ( (uint16_t) (b >> 16) )
    : "r18", "r19", "r20", "r21", "r22", "r23", "r24", "r25", "cc" // Clobbers
  );

  return wordPairToQ16x16(result);
}


Q16x16 q16x16_mul_s(const Q16x16 a, const Q16x16 b) {
  uint16_t result[2];

  __asm__ volatile (
    // Initialize registers
    // r23:r22 are simply initialized to a0 * b0
    "clr r2\n\t" // zero-register, used for carry cascading
    "clr r23\n\t"
    "clr r24\n\t"
    "clr r25\n\t"
    "clr r26\n\t"
    "clr r27\n\t"
    "clt\n\t" // Clear T bit (used for sign tracking)

    // Load b
    "movw r16, %[b_low]\n\t"
    "movw r18, %[b_high]\n\t"

    // Check if b is signed
    "sbrs r19, 7\n\t" // skip if b is negative
    "rjmp .b_positive%=\n\t"
    // Take two's complement
    "clc\n\t" // clear carry
    "sbci r16, 1\n\t" // b0
    "sbci r17, 0\n\t" // b1
    "sbci r18, 0\n\t" // b2
    "sbci r19, 0\n\t" // b3
    "com r16\n\t"
    "com r17\n\t"
    "com r18\n\t"
    "com r19\n\t"
    // Set T
    "set\n\t" // Set T bit

    ".b_positive%=:\n\t"
    "movw r30, r18\n\t" // Backup b_high

    // Load a
    "movw r18, %[a_low]\n\t"
    "movw r20, %[a_high]\n\t"

    // check if a is signed
    "sbrs r21, 7\n\t" // skip if a is negative
    "rjmp .a_positive%=\n\t"
    "clc\n\t" // clear carry
    "sbci r18, 1\n\t" // a0
    "sbci r19, 0\n\t" // a1
    "sbci r20, 0\n\t" // a2
    "sbci r21, 0\n\t" // a3
    "com r18\n\t"
    "com r19\n\t"
    "com r20\n\t"
    "com r21\n\t"
    // Toggle T
    "bld r2, 0\n\t" // Load R2[0] <- T
    "inc r2\n\t" // Has the effect of toggling r2[0]
    "bst r2, 0\n\t"
    "clr r2\n\t"

    ".a_positive%=:\n\t"

    // Perform unsigned multiplication

    /****  b0  ****/
    // First we'll calculate the first significant byte (i.e. byte 3)
    // We do this so we can throw away the bottom two bytes ASAP to save registers
    "mul r16, r18\n\t" // b0 * a0
    "mov r22, r1\n\t" // Store result (discard low byte)


    "mul r16, r19\n\t" // b0 * a1
    "add r22, r0\n\t"
    "adc r23, r1\n\t"

    "mul r17, r18\n\t" // b1 * a0
    "add r22, r0\n\t"
    "adc r23, r1\n\t"
    "mov r22, r23\n\t" // Shift one byte down
    "clr r23\n\t" // re-initialize r23
    "adc r23, r2\n\t" // Carry

    "mul r16, r20\n\t" // b0 * a2
    "add r22, r0\n\t" // Reminder, we've shifted down two bytes
    "adc r23, r1\n\t"
    "adc r24, r2\n\t" // Carry


    "mul r16, r21\n\t" // b0 * a3
    "add r23, r0\n\t"
    "adc r24, r1\n\t"
    "adc r25, r2\n\t"


    /****  b1  ****/
    // Reminder, we already multiplied b1 * a0
    "mul r17, r19\n\t" // b1 * a1
    "add r22, r0\n\t"
    "adc r23, r1\n\t"
    "adc r24, r2\n\t"
    "adc r25, r2\n\t"
    "adc r26, r2\n\t"


    "mul r17, r20\n\t" // b1 * a2
    "add r23, r0\n\t"
    "adc r24, r1\n\t"
    "adc r25, r2\n\t"
    "adc r26, r2\n\t"


    "mul r17, r21\n\t" // b1 * a3
    "add r24, r0\n\t"
    "adc r25, r1\n\t"
    "adc r26, r2\n\t"


    // Load high byte of b
    "movw r16, r30\n\t"

    /****  b2  ****/
    "mul r16, r18\n\t" // b2 * a0
    "add r22, r0\n\t"
    "adc r23, r1\n\t"
    "adc r24, r2\n\t"
    "adc r25, r2\n\t"
    "adc r26, r2\n\t"

    "mul r16, r19\n\t" // b2 * a1
    "add r23, r0\n\t"
    "adc r24, r1\n\t"
    "adc r25, r2\n\t"
    "adc r26, r2\n\t"

    "mul r16, r20\n\t" // b2 * a2
    "add r24, r0\n\t"
    "adc r25, r1\n\t"
    "adc r26, r2\n\t"

    "mul r16, r21\n\t" // b2 * a3
    "add r25, r0\n\t"
    "adc r26, r1\n\t"
    "adc r27, r2\n\t"


    /****  b3  ****/
    "mul r17, r18\n\t" // b3 * a0
    "add r23, r0\n\t"
    "adc r24, r1\n\t"
    "adc r25, r2\n\t"
    "adc r26, r2\n\t"
    "adc r27, r2\n\t"

    "mul r17, r19\n\t" // b3 * a1
    "add r24, r0\n\t"
    "adc r25, r1\n\t"
    "adc r26, r2\n\t"
    "adc r27, r2\n\t"

    "mul r17, r20\n\t" // b3 * a2
    "add r25, r0\n\t"
    "adc r26, r1\n\t"
    "adc r27, r2\n\t"

    "mul r17, r21\n\t" // b3 * a3
    "add r26, r0\n\t"
    "adc r27, r1\n\t"


    // Check if over/underflow occurred
    // If r27 ∨ r26 ∨ r25[7], overflow occured
    "or r27, r26\n\t" // r27 ∨ r26
    "mov r26, r25\n\t"
    "andi r26, 0x80\n\t" // Copy r26 <- r25[7]
    "or r27, r26\n\t" // r27 ∨ r26 ∨ r25[7]
    "breq .sign_check%=\n\t" // No overflow

    // Overflow occured, check sign
    "brts .clamp_min%=\n\t" // Clamp min if sign(T) is set
    "rjmp .clamp_max%=\n\t"


    // No overflow occurred
    ".sign_check%=:\n\t"
    // Check if result should be negative
    "brtc .return_result%=\n\t" // Skip if result is negative
    "clc\n\t" // clear carry
    "sbci r22, 1\n\t"
    "sbci r23, 0\n\t"
    "sbci r24, 0\n\t"
    "sbci r25, 0\n\t"
    "com r22\n\t"
    "com r23\n\t"
    "com r24\n\t"
    "com r25\n\t"
    "rjmp .return_result%=\n\t"
    // Two high bytes don't need to be corrected since these are thrown out


    // Overflow return paths
    ".clamp_max%=:\n\t"
    "ser r22\n\t"
    "ser r23\n\t"
    "ser r24\n\t"
    "ldi r25, 0x7F\n\t"
    "rjmp .return_result%=\n\t"

    ".clamp_min%=:\n\t"
    "clr r22\n\t"
    "clr r23\n\t"
    "clr r24\n\t"
    "ldi r25, 0x80\n\t"

    ".return_result%=:\n\t"
    "clr r1\n\t" // GCC expects r1 to be zero
    "movw %[result_low], r22\n\t"
    "movw %[result_high], r24\n\t"

    ".exit%=:\n\t"

    : [result_low] "=r" ( result[0] ), // Outputs
      [result_high] "=r" ( result[1] )
    : [a_low] "r" ( (uint16_t) (a & 0xFFFF) ), // Inputs
      [a_high] "r" ( (uint16_t) (a >> 16) ),
      [b_low] "r" ( (uint16_t) (b & 0xFFFF) ),
      [b_high] "r" ( (uint16_t) (b >> 16) )
    : "r0", "r1", // Product registers
      "r2", // Zero register
      "r30", "r31", // scratch registers
      "r16", "r17", // b-word
      "r18", "r19", "r20", "r21", // a
      "r22", "r23", "r24", "r25", "r26", "r27", // Result
      "cc"
  );

  return wordPairToQ16x16(result);
}

/*
// Portable, but naive division function
// Better would be to recreate the bitwise division algorithm used in the
// assembly version
Q16x16 q16x16_div_s(const Q16x16 a, const Q16x16 b) {
  bool isSigned = (a & 0x80000000) != (b & 0x80000000); // exactly one is signed
  if (b == 0) {
    return isSigned ? 0x80000000 : 0x7FFFFFFF;
  }
  Q16x16 result = ((int64_t) a << 16) / b;
  if ((bool) (result & 0x80000000) != isSigned) { // Over/underflow occurred
    if (isSigned) {
      return 0x80000000;
    } else {
      return 0x7FFFFFFF;
    }
  } else { // No over/underflow occured
    return result;
  }
}
*/

// Safe signed Q16.16 division with saturation.
// Computes (a / b) in Q16.16, clamping on overflow.
// If b == 0, returns max or min depending on the sign of a.
Q16x16 q16x16_div_s(const Q16x16 a, const Q16x16 b) {
  uint16_t result[2];

  __asm__ volatile (
    // Initialize registers
    "movw r24, %[a_low]\n\t"
    "movw r26, %[a_high]\n\t" // Load r27:r24 <- a
    "movw r20, %[b_low]\n\t"
    "movw r22, %[b_high]\n\t" // Load r23:r20 <- b
    "clr r16\n\t"
    "clr r17\n\t"
    "clt\n\t" // Clear T flag
    "movw r18, r16\n\t" // Clear remainder register r19:r16

    // Check signs of operands, set T bit, and take Two's complement as needed
    // Check sign of a first, then check if b is zero. If so, jump to overflow_occurred
    // Check sign of a
    "sbrs r27, 7\n\t" // skip if a is negative
    "rjmp .a_positive%=\n\t"
    // Take two's complement
    "clc\n\t" // clear carry
    "sbci r24, 1\n\t" // a0
    "sbci r25, 0\n\t" // a1
    "sbci r26, 0\n\t" // a2
    "sbci r27, 0\n\t" // a3
    "com r24\n\t"
    "com r25\n\t"
    "com r26\n\t"
    "com r27\n\t"
    // Set T
    "set\n\t" // Set T bit
    ".a_positive%=:\n\t"

    // Check if b is zero
    "mov r30, r20\n\t"
    "or r30, r21\n\t"
    "or r30, r22\n\t"
    "or r30, r23\n\t"
    "brne .non_zero%=\n\t"
    "rjmp .overflow_occurred%=\n\t" // Needs to use rjmp for increased range

    ".non_zero%=:\n\t"

    // Check if b is negative
    "sbrs r23, 7\n\t" // skip if b is negative
    "rjmp .b_positive%=\n\t"
    // Take two's complement
    "clc\n\t" // clear carry
    "sbci r20, 1\n\t" // b0
    "sbci r21, 0\n\t" // b1
    "sbci r22, 0\n\t" // b2
    "sbci r23, 0\n\t" // b3
    "com r20\n\t"
    "com r21\n\t"
    "com r22\n\t"
    "com r23\n\t"
    // Toggle T
    "bld r30, 0\n\t" // Load R30[0] <- T
    "inc r30\n\t" // Has the effect of toggling r30[0]
    "bst r30, 0\n\t"
    ".b_positive%=:\n\t"

    // Initialize loop counter
    "ldi r30, 32\n\t"

    // Integer quotient loop
    ".integer_loop%=:\n\t"
    // Shift dividend
    "lsl r24\n\t"
    "rol r25\n\t"
    "rol r26\n\t"
    "rol r27\n\t"
    // Shift remainder (and move carry into LSB)
    "rol r16\n\t"
    "rol r17\n\t"
    "rol r18\n\t"
    "rol r19\n\t"
    // Check if r19:r16 >= r23:r20
    "cp r16, r20\n\t"
    "cpc r17, r21\n\t"
    "cpc r18, r22\n\t"
    "cpc r19, r23\n\t"
    // C=1 if remainder < b
    "brcs .integer_loop_no_sub%=\n\t"
    // Subtract b from remainder
    "sub r16, r20\n\t"
    "sbc r17, r21\n\t"
    "sbc r18, r22\n\t"
    "sbc r19, r23\n\t"
    "ori r24, 0x01\n\t" // Set bit 0 of dividend
    ".integer_loop_no_sub%=:\n\t"
    // Decrement loop counter
    "dec r30\n\t"
    // Jump if not zero
    "cpi r30, 0\n\t"
    "brne .integer_loop%=\n\t" // Loop 32 times

    // Early over/underflow detection
    // Checks two high bytes and r25[7]
    "mov r30, r25\n\t"
    "andi r30, 0x80\n\t" // Copy r25[7] to r0
    "or r30, r26\n\t"
    "or r30, r27\n\t"
    "brne .overflow_occurred%=\n\t"

    // No overflow, compute fractional part
    // First save integer part to the upper word of our quotient register
    "movw r26, r24\n\t"
    "clr r24\n\t"
    "clr r25\n\t" // Initialize quotient (16 bits!) to zero
    "ldi r30, 16\n\t" // Initialize loop counter

    ".fractional_loop%=:\n\t"
    // Shift dividend (only 16 bits now!)
    "lsl r24\n\t"
    "rol r25\n\t"
    // Shift remainder (still 32 bits!)
    "lsl r16\n\t"
    "rol r17\n\t"
    "rol r18\n\t"
    "rol r19\n\t"
    // Check if r19:r16 >= r23:r20
    "cp r16, r20\n\t"
    "cpc r17, r21\n\t"
    "cpc r18, r22\n\t"
    "cpc r19, r23\n\t"
    // C=1 if remainder < b
    "brcs .fractional_loop_no_sub%=\n\t"
    // Subtract b from remainder
    "sub r16, r20\n\t"
    "sbc r17, r21\n\t"
    "sbc r18, r22\n\t"
    "sbc r19, r23\n\t"
    "ori r24, 0x01\n\t" // Set bit 0 of dividend
    ".fractional_loop_no_sub%=:\n\t"
    // Decrement loop counter
    "dec r30\n\t"
    // Jump if not zero
    "cpi r30, 0\n\t"
    "brne .fractional_loop%=\n\t" // Loop 16 times

    // Additional overflow detection if necessary

    // Our unsigned result is now in r27:r24

    // Check sign, assuming no overflow
    // Check if result should be negative
    "brtc .return_result%=\n\t" // Skip if result is positive
    "clc\n\t" // clear carry
    "sbci r24, 1\n\t"
    "sbci r25, 0\n\t"
    "sbci r26, 0\n\t"
    "sbci r27, 0\n\t"
    "com r24\n\t"
    "com r25\n\t"
    "com r26\n\t"
    "com r27\n\t"
    "rjmp .return_result%=\n\t"


    ".overflow_occurred%=:\n\t"
    // Overflow occured, check sign
    "brts .clamp_min%=\n\t" // Clamp min if sign(T) is set
    "rjmp .clamp_max%=\n\t"

    ".clamp_min%=:\n\t"
    "ldi r27, 0x80\n\t"
    "clr r26\n\t"
    "clr r25\n\t"
    "clr r24\n\t"
    "rjmp .return_result%=\n\t"

    ".clamp_max%=:\n\t"
    "ldi r27, 0x7F\n\t"
    "ser r26\n\t"
    "ser r25\n\t"
    "ser r24\n\t" // Fall through to return result

    ".return_result%=:\n\t"
    "movw %[result_low], r24\n\t"
    "movw %[result_high], r26\n\t"

  : [result_low] "=r" ( result[0] ), // Outputs
    [result_high] "=r" ( result[1] )
  : [a_low] "r" ( (uint16_t) (a & 0xFFFF) ), // Inputs
    [a_high] "r" ( (uint16_t) (a >> 16) ),
    [b_low] "r" ( (uint16_t) (b & 0xFFFF) ),
    [b_high] "r" ( (uint16_t) (b >> 16) )
  : "r16", "r17", "r18", "r19", // Remainder
    "r20", "r21", "r22", "r23", // Divisor "b"
    "r24", "r25", "r26", "r27", // Dividend / quotient "a"
    "r30", // Loop counter / scratch
    "cc"
  );

  return wordPairToQ16x16(result);
}
