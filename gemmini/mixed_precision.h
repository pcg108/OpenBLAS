#ifndef GEMMINI_MIXED_PRECISION_H
#define GEMMINI_MIXED_PRECISION_H

#include <stdint.h>
#include <string.h>

static float to_half(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(float));

    uint32_t sign = (bits >> 31) & 0x1;
    uint32_t exponent = (bits >> 23) & 0xFF;
    uint32_t mantissa = bits & 0x7FFFFF;

    uint32_t fp16_sign = sign;
    uint32_t fp16_exponent;
    uint32_t fp16_mantissa;

    // Handle special cases first
    if (exponent == 0xFF) {  // Infinity or NaN
        fp16_exponent = 0x1F;  // Max exponent for fp16
        fp16_mantissa = (mantissa != 0) ? 0x3FF : 0;  // NaN or Infinity
    }
    else if (exponent == 0) {  // Zero or denormal
        fp16_exponent = 0;
        fp16_mantissa = 0;  // Flush denormals to zero (common approach)
    }
    else {  // Normal numbers
        // Convert exponent from fp32 bias (127) to fp16 bias (15)
        int actual_exponent = ((int)exponent) - 127;
        int fp16_exp_biased = actual_exponent + 15;
        
        // Handle overflow to infinity
        if (fp16_exp_biased >= 31) {
            fp16_exponent = 0x1F;
            fp16_mantissa = 0;  // Infinity
        }
        // Handle underflow (including denormals - we'll flush to zero)
        else if (fp16_exp_biased <= 0) {
            fp16_exponent = 0;
            fp16_mantissa = 0;  // Flush to zero
        }
        else {
            fp16_exponent = fp16_exp_biased;
            
            // Round mantissa from 23 bits to 10 bits
            // We need to look at bit 12 (round bit) and bits 11-0 (sticky bits)
            uint32_t round_bit = (mantissa >> 12) & 1;
            uint32_t sticky_bits = mantissa & 0xFFF;  // Bottom 12 bits
            fp16_mantissa = mantissa >> 13;  // Take top 10 bits
            
            // Round to nearest even
            if (round_bit && (sticky_bits > 0 || (fp16_mantissa & 1))) {
                fp16_mantissa++;
                // Handle mantissa overflow
                if (fp16_mantissa == 0x400) {  // 2^10
                    fp16_mantissa = 0;
                    fp16_exponent++;
                    // Check for exponent overflow after rounding
                    if (fp16_exponent >= 31) {
                        fp16_exponent = 31;
                        fp16_mantissa = 0;  // Infinity
                    }
                }
            }
        }
    }
    
    uint32_t result_bits = (fp16_sign << 31) | (fp16_exponent << 23) | fp16_mantissa;
    
    float result;
    memcpy(&result, &result_bits, sizeof(float));
    return result;
}

#endif  // GEMMINI_MIXED_PRECISION_H