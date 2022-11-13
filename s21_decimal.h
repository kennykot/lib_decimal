#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

#define MAX 4294967295
#define _2(bit) (1 << bit)
#define DECIMAL_TEN ((s21_decimal){{10, 0, 0, 0}})
#define DECIMAL_ZERO ((s21_decimal){{0, 0, 0, 0}})
#define DECIMAL_ONE ((s21_decimal){{1, 0, 0, 0}})
#define DECIMAL_MAX ((s21_decimal){{4294967295, 4294967295, 4294967295, 0}})

typedef enum { CONVERT_GOOD = 0, CONVERT_ERROR = 1 } convert_result;

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct big_decimal {
  s21_decimal big_bits[2];
} big_decimal;

typedef union decimal_bit3 {
  int i;
  struct {
    uint32_t empty2 : 16;
    uint32_t power : 8;
    uint32_t empty1 : 7;
    uint32_t sign : 1;
  } parts;
} decimal_bit3;

static const s21_decimal all_ten_pows[39] = {
    [0] = {{0x1, 0x0, 0x0, 0x0}},
    [1] = {{0xA, 0x0, 0x0, 0x0}},
    [2] = {{0x64, 0x0, 0x0, 0x0}},
    [3] = {{0x3E8, 0x0, 0x0, 0x0}},
    [4] = {{0x2710, 0x0, 0x0, 0x0}},
    [5] = {{0x186A0, 0x0, 0x0, 0x0}},
    [6] = {{0xF4240, 0x0, 0x0, 0x0}},
    [7] = {{0x989680, 0x0, 0x0, 0x0}},
    [8] = {{0x5F5E100, 0x0, 0x0, 0x0}},
    [9] = {{0x3B9ACA00, 0x0, 0x0, 0x0}},
    [10] = {{0x540BE400, 0x2, 0x0, 0x0}},
    [11] = {{0x4876E800, 0x17, 0x0, 0x0}},
    [12] = {{0xD4A51000, 0xE8, 0x0, 0x0}},
    [13] = {{0x4E72A000, 0x918, 0x0, 0x0}},
    [14] = {{0x107A4000, 0x5AF3, 0x0, 0x0}},
    [15] = {{0xA4C68000, 0x38D7E, 0x0, 0x0}},
    [16] = {{0x6FC10000, 0x2386F2, 0x0, 0x0}},
    [17] = {{0x5D8A0000, 0x1634578, 0x0, 0x0}},
    [18] = {{0xA7640000, 0xDE0B6B3, 0x0, 0x0}},
    [19] = {{0x89E80000, 0x8AC72304, 0x0, 0x0}},
    [20] = {{0x63100000, 0x6BC75E2D, 0x5, 0x0}},
    [21] = {{0xDEA00000, 0x35C9ADC5, 0x36, 0x0}},
    [22] = {{0xB2400000, 0x19E0C9BA, 0x21E, 0x0}},
    [23] = {{0xF6800000, 0x2C7E14A, 0x152D, 0x0}},
    [24] = {{0xA1000000, 0x1BCECCED, 0xD3C2, 0x0}},
    [25] = {{0x4A000000, 0x16140148, 0x84595, 0x0}},
    [26] = {{0xE4000000, 0xDCC80CD2, 0x52B7D2, 0x0}},
    [27] = {{0xE8000000, 0x9FD0803C, 0x33B2E3C, 0x0}},
    [28] = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}},
    [29] = {{0xA0000000, 0x6D7217CA, 0x431E0FAE, 0x1}},
    [30] = {{0x40000000, 0x4674EDEA, 0x9F2C9CD0, 0xC}},
    [31] = {{0x80000000, 0xC0914B26, 0x37BE2022, 0x7E}},
    [32] = {{0x0, 0x85ACEF81, 0x2D6D415B, 0x4EE}},
    [33] = {{0x0, 0x38C15B0A, 0xC6448D93, 0x314D}},
    [34] = {{0x0, 0x378D8E64, 0xBEAD87C0, 0x1ED09}},
    [35] = {{0x0, 0x2B878FE8, 0x72C74D82, 0x134261}},
    [36] = {{0x0, 0xB34B9F10, 0x7BC90715, 0xC097CE}},
    [37] = {{0x0, 0xF436A0, 0xD5DA46D9, 0x785EE10}},
    [38] = {{0x0, 0x98A2240, 0x5A86C47A, 0x4B3B4CA8}}};

// Arithmetic Operators
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Comparison Operators
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_less_simple(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
int s21_is_equal_simple(s21_decimal dec1, s21_decimal dec2);

// Convertors and parsers
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Another functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// Help functions
const char *byte_to_binary(int x);
int get_sign(s21_decimal *num);
void set_sign(s21_decimal *num, int sign);
int get_bits(s21_decimal num, int global_ind);
unsigned int *rank_bits(s21_decimal *num, int global_ind);
int is_bit(int bits, int bit);
int get_bit(s21_decimal num, int bit);
int set_bit(int bits, int bit, int new_bit);
int is_zero(s21_decimal num);
int get_scale(s21_decimal *num);
void set_scale(s21_decimal *p_one, int a);
int add_helper(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int check_sign(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int sub_helper(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void set_minus(s21_decimal *dst);
void left_shift_decimal(s21_decimal *num, int step);
void right_shift_decimal(s21_decimal *num, int step);
void normalize(s21_decimal *num1, s21_decimal *num2);
int is_correct(s21_decimal *value);
s21_decimal s21_rshift(s21_decimal num, int step);
void s21_bindiv(s21_decimal value, s21_decimal *result, double m);
int div_int(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int CheckRightBit(int num);
int s21_decimal_len(s21_decimal num);
int is_last_zero(s21_decimal num1);

int s21_max(int value_1, int value_2);
void big_normalize(s21_decimal value_1, s21_decimal value_2,
                   big_decimal *value_1l, big_decimal *value_2l);
s21_decimal get_pow(int pow);
void decimal_only(s21_decimal *value);
big_decimal create_big_decimal(s21_decimal value_1);
int is_set_bit(s21_decimal decimal, int index);
s21_decimal s21_decimal_set_bit(s21_decimal decimal, int index);
int s21_is_set_bit(int number, int index);
int s21_set_bit(int number, int index);
int move_bit(s21_decimal decimal);
int mul_normalize(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result);
int small_compare(s21_decimal d1, s21_decimal d2);
int big_compare(big_decimal d1, big_decimal d2);
s21_decimal binand(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal binxor(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal binnot(s21_decimal decimal);
big_decimal big_add(big_decimal decimal1, big_decimal decimal2);
big_decimal big_sub(big_decimal decimal1, big_decimal decimal2);
big_decimal big_div(big_decimal decimal1, big_decimal decimal2,
                    big_decimal *remainder);
big_decimal mul_helper(s21_decimal decimal1, s21_decimal decimal2);
s21_decimal big_shift_L_one(s21_decimal decimal);
s21_decimal big_shift_R_one(s21_decimal decimal);
big_decimal big_shift_L(big_decimal decimal, int shift);
big_decimal big_shift_R(big_decimal decimal, int shift);
s21_decimal error(void);
s21_decimal s21_abs(s21_decimal value_1);
void set_power(s21_decimal *decimal, int power);
void set_sign_b(s21_decimal *decimal, int sign);
int get_power(s21_decimal decimal);
int get_sign_b(s21_decimal decimal);
int mul_helper_2(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int checking_decimal(big_decimal value);
int is_equal_zero(s21_decimal decimal);

#endif  // S21_DECIMAL_H
