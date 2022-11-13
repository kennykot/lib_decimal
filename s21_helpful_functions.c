#include "s21_decimal.h"

//  Set negative sign to decimal
void set_minus(s21_decimal* dst) {
  unsigned int num = 127 / 32;
  unsigned int tmp = dst->bits[num];
  dst->bits[num] = tmp | ~(~0u << 1) << 127 % 32;
}

// Returns string with decimal
const char* byte_to_binary(int x) {
  static char b[9];
  b[0] = '\0';
  unsigned int z;
  for (z = 129; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }
  return b;
}

//  Returns sign of decimal from bits[3]
int get_sign(s21_decimal* num) {
  unsigned int mask = 1u << 31;
  return !!(num->bits[3] & mask);
}

//  Sets sign of decimal to bits[3]
void set_sign(s21_decimal* num, int sign) {
  unsigned int mask = 1u << 31;
  if (sign != 0) {
    num->bits[3] |= mask;
  } else {
    num->bits[3] &= ~mask;
  }
}

//  Returns the required set with bits
int get_bits(s21_decimal num, int pos_bit) {
  return num.bits[(pos_bit % 96) / 32];
}

//  Returns the required pointer set with bits
unsigned int* rank_bits(s21_decimal* num, int pos_bit) {
  return num->bits + ((pos_bit % 96) / 32);
}

//  Checks bit is 0 or 1
int is_bit(int bits, int bit) { return (bits & (_2(bit % 32))) != 0; }

//  Gets bit from decimal
int get_bit(s21_decimal dec, int bit) {
  int res = 0;
  if (bit / 32 < 4) {
    unsigned int mask = 1u << (bit % 32);
    res = dec.bits[bit / 32] & mask;
  }
  return res;
}

//  Sets the desired bit to a certain position
int set_bit(int bits, int bit, int new_bit) {
  if (new_bit) {
    bits = bits | _2(bit % 32);
  } else {
    bits = bits & ~(_2(bit % 32));
  }
  return bits;
}

//  Checks decimal is 0 or not
int is_zero(s21_decimal num) {
  return !num.bits[0] && !num.bits[1] && !num.bits[2];
}

//  Get scale from bits[3]
int get_scale(s21_decimal* num) { return (char)((num->bits[3] << 1) >> 17); }

//  Set scale from bits[3]
void set_scale(s21_decimal* p_one, int a) {
  p_one->bits[3] = p_one->bits[3] & (~0u << 24);  // обнуляем биты с 0 до 23
  int num = a;
  for (int i = 112; i < 119; i++) {
    if (num % 2 == 1) {
      unsigned int tmp = p_one->bits[i / 32];
      p_one->bits[i / 32] = tmp | ~(~0u << 1) << i % 32;
    }
    num = num / 2;
  }
}

//  Moves bits to the left
void left_shift_decimal(s21_decimal* num, int step) {
  for (int s = 0; s < step; s++) {
    num->bits[2] = num->bits[2] << 1;
    if (get_bit(*num, 63) != 0) num->bits[2] = set_bit(num->bits[2], 0, 1);
    num->bits[1] = num->bits[1] << 1;
    if (get_bit(*num, 31) != 0) num->bits[1] = set_bit(num->bits[1], 0, 1);
    num->bits[0] = num->bits[0] << 1;
  }
}

//  Moves bits to the right
void right_shift_decimal(s21_decimal* num, int step) {
  for (int s = 0; s < step; s++) {
    num->bits[0] = num->bits[0] >> 1;
    if (get_bit(*num, 32) > 0) num->bits[0] = set_bit(num->bits[0], 31, 1);
    num->bits[1] = num->bits[1] >> 1;
    if (get_bit(*num, 64) > 0) num->bits[1] = set_bit(num->bits[1], 31, 1);
    num->bits[2] = num->bits[2] >> 1;
  }
}

//  Sets the same scale for both decimal
void normalize(s21_decimal* num1, s21_decimal* num2) {
  s21_decimal min_ten = DECIMAL_TEN;
  set_scale(&min_ten, 1);
  int delta = get_scale(num1) - get_scale(num2);
  while (delta != 0) {
    if (delta < 0) {
      mul_normalize(*num1, min_ten, num1);
      set_scale(num1, get_scale(num1));
    } else {
      mul_normalize(*num2, min_ten, num2);
      set_scale(num2, get_scale(num2));
    }
    delta = get_scale(num1) - get_scale(num2);
  }
}

//  Checks input decimal
int is_correct(s21_decimal* value) {
  int res = TRUE;
  int scale = get_scale(value);
  if (!value) {
    res = FALSE;
  } else if (scale < 0 || scale > 28) {
    res = FALSE;
  }
  return res;
}

//  Checks last digit is zero or not
int s21_is_last_zero(s21_decimal num1) {
  int sum = 0;
  for (int i = 0; i < 96; i++) {
    sum += get_bit(num1, i) % 10;
  }
  return sum % 10 == 0 ? 1 : 0;
}

//  Returns the length of decimal binary representation
int s21_decimal_len(s21_decimal num) {
  int len = 0;
  int i = -1;
  if (num.bits[2] != 0) {
    i = 95;
  } else if (num.bits[1] != 0) {
    i = 63;
  } else if (num.bits[0] != 0) {
    i = 31;
  }
  for (; i >= 0; i--) {
    if (get_bit(num, i) != 0) {
      len = i + 1;
      break;
    }
  }
  return len;
}

//  Checks last bit in decimal
int CheckRightBit(int num) {
  int res = 0;
  if (num & 0b00000000000000000000000000000001) {
    res = 1;
  }
  return res;
}

//  Division for truncate
void s21_bindiv(s21_decimal value, s21_decimal* result, double m) {
  double n = (double)value.bits[0];
  double fract = fmod(n, 10);
  s21_add(s21_rshift(value, 1), s21_rshift(value, 2), result);
  s21_add(*result, s21_rshift(*result, 4), result);
  s21_add(*result, s21_rshift(*result, 8), result);
  s21_add(*result, s21_rshift(*result, 16), result);
  s21_add(*result, s21_rshift(*result, 32), result);
  s21_add(*result, s21_rshift(*result, 64), result);
  *result = s21_rshift(*result, 3);
  if ((fract == 1 || fract == 0) && m > 1)
    s21_add(*result, DECIMAL_ONE, result);
}

//  Shifting for truncate
s21_decimal s21_rshift(s21_decimal num, int step) {
  for (int s = 0; s < step; s++) {
    num.bits[0] = num.bits[0] >> 1;
    if (get_bit(num, 32) > 0) num.bits[0] = set_bit(num.bits[0], 31, 1);
    num.bits[1] = num.bits[1] >> 1;
    if (get_bit(num, 64) > 0) num.bits[1] = set_bit(num.bits[1], 31, 1);
    num.bits[2] = num.bits[2] >> 1;
  }
  return num;
}

int s21_max(int value_1, int value_2) {
  int result = value_2;
  if (value_1 > value_2) result = value_1;
  return result;
}

int small_compare(s21_decimal d1, s21_decimal d2) {
  int result = 0;
  for (int i = 127; i >= 0; i--) {
    int check_bit_1 = is_set_bit(d1, i);
    int check_bit_2 = is_set_bit(d2, i);
    if (check_bit_1 == 0 && check_bit_2 != 0) {
      result = -1;
    }
    if (check_bit_1 != 0 && check_bit_2 == 0) {
      result = 1;
    }
    if (result != 0) {
      break;
    }
  }
  return result;
}

int big_compare(big_decimal d1, big_decimal d2) {
  int compare = small_compare(d1.big_bits[1], d2.big_bits[1]);
  if (compare == 0) compare = small_compare(d1.big_bits[0], d2.big_bits[0]);
  return compare;
}

big_decimal big_add(big_decimal decimal1, big_decimal decimal2) {
  big_decimal result = decimal1;
  big_decimal tmp = decimal2;
  while (!is_equal_zero(tmp.big_bits[0]) || !is_equal_zero(tmp.big_bits[1])) {
    big_decimal ostatok;
    ostatok.big_bits[0] = binand(result.big_bits[0], tmp.big_bits[0]);
    ostatok.big_bits[1] = binand(result.big_bits[1], tmp.big_bits[1]);
    ostatok = big_shift_L(ostatok, 1);
    result.big_bits[0] = binxor(result.big_bits[0], tmp.big_bits[0]);
    result.big_bits[1] = binxor(result.big_bits[1], tmp.big_bits[1]);
    tmp = ostatok;
  }
  return result;
}

big_decimal big_sub(big_decimal decimal1, big_decimal decimal2) {
  big_decimal result;
  decimal2.big_bits[0] = binnot(decimal2.big_bits[0]);
  decimal2.big_bits[1] = binnot(decimal2.big_bits[1]);
  big_decimal one = create_big_decimal(DECIMAL_ONE);
  decimal2 = big_add(decimal2, one);
  result = big_add(decimal1, decimal2);
  return result;
}

s21_decimal binand(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = DECIMAL_ZERO;
  result.bits[0] = decimal1.bits[0] & decimal2.bits[0];
  result.bits[1] = decimal1.bits[1] & decimal2.bits[1];
  result.bits[2] = decimal1.bits[2] & decimal2.bits[2];
  result.bits[3] = decimal1.bits[3] & decimal2.bits[3];
  return result;
}

s21_decimal binxor(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = DECIMAL_ZERO;
  result.bits[0] = decimal1.bits[0] ^ decimal2.bits[0];
  result.bits[1] = decimal1.bits[1] ^ decimal2.bits[1];
  result.bits[2] = decimal1.bits[2] ^ decimal2.bits[2];
  result.bits[3] = decimal1.bits[3] ^ decimal2.bits[3];
  return result;
}

s21_decimal binnot(s21_decimal decimal) {
  s21_decimal result = DECIMAL_ZERO;
  result.bits[0] = ~decimal.bits[0];
  result.bits[1] = ~decimal.bits[1];
  result.bits[2] = ~decimal.bits[2];
  result.bits[3] = ~decimal.bits[3];
  return result;
}

big_decimal big_shift_L(big_decimal decimal, int shift) {
  big_decimal result = decimal;
  while (shift > 0) {
    int check_bit_0 = is_set_bit(result.big_bits[0], 127);
    result.big_bits[0] = big_shift_L_one(result.big_bits[0]);
    result.big_bits[1] = big_shift_L_one(result.big_bits[1]);
    if (check_bit_0) {
      result.big_bits[1] = s21_decimal_set_bit(result.big_bits[1], 0);
    }
    --shift;
  }
  return result;
}

big_decimal big_shift_R(big_decimal decimal, int shift) {
  big_decimal result = decimal;
  while (shift > 0) {
    int check_bit_1 = is_set_bit(result.big_bits[1], 0);
    result.big_bits[0] = big_shift_R_one(result.big_bits[0]);
    result.big_bits[1] = big_shift_R_one(result.big_bits[1]);
    if (check_bit_1) {
      result.big_bits[0] = s21_decimal_set_bit(result.big_bits[0], 127);
    }
    --shift;
  }
  return result;
}

s21_decimal big_shift_L_one(s21_decimal decimal) {
  s21_decimal result = DECIMAL_ZERO;
  int check_bit_0 = s21_is_set_bit(decimal.bits[0], 31);
  unsigned int result0 = decimal.bits[0];
  result0 = result0 << 1;
  result.bits[0] = result0;

  int check_bit_1 = s21_is_set_bit(decimal.bits[1], 31);
  unsigned int result1 = decimal.bits[1];
  result1 = result1 << 1;
  result.bits[1] = result1;

  int check_bit_2 = s21_is_set_bit(decimal.bits[2], 31);
  unsigned int result2 = decimal.bits[2];
  result2 = result2 << 1;
  result.bits[2] = result2;

  unsigned int result3 = decimal.bits[3];
  result3 = result3 << 1;
  result.bits[3] = result3;

  if (check_bit_0) {
    result.bits[1] = s21_set_bit(result.bits[1], 0);
  }

  if (check_bit_1) {
    result.bits[2] = s21_set_bit(result.bits[2], 0);
  }

  if (check_bit_2) {
    result.bits[3] = s21_set_bit(result.bits[3], 0);
  }

  return result;
}

s21_decimal big_shift_R_one(s21_decimal decimal) {
  s21_decimal result = DECIMAL_ZERO;
  int check_bit_3 = s21_is_set_bit(decimal.bits[3], 0);
  unsigned int result3 = decimal.bits[3];
  result3 = result3 >> 1;
  result.bits[3] = result3;
  int check_bit_2 = s21_is_set_bit(decimal.bits[2], 0);
  unsigned int result2 = decimal.bits[2];
  result2 = result2 >> 1;
  result.bits[2] = result2;
  int check_bit_1 = s21_is_set_bit(decimal.bits[1], 0);
  unsigned int result1 = decimal.bits[1];
  result1 = result1 >> 1;
  result.bits[1] = result1;
  unsigned int result0 = decimal.bits[0];
  result0 = result0 >> 1;
  result.bits[0] = result0;
  if (check_bit_3) {
    result.bits[2] = s21_set_bit(result.bits[2], 31);
  }
  if (check_bit_2) {
    result.bits[1] = s21_set_bit(result.bits[1], 31);
  }
  if (check_bit_1) {
    result.bits[0] = s21_set_bit(result.bits[0], 31);
  }
  return result;
}

int s21_is_set_bit(int number, int index) { return !!(number & (1U << index)); }

int s21_set_bit(int number, int index) { return number | (1U << index); }

int is_set_bit(s21_decimal decimal, int index) {
  return s21_is_set_bit(decimal.bits[index / 32], index % 32);
}

s21_decimal s21_decimal_set_bit(s21_decimal decimal, int index) {
  decimal.bits[index / 32] = s21_set_bit(decimal.bits[index / 32], index % 32);
  return decimal;
}

int move_bit(s21_decimal decimal) {
  int result = -1;
  for (int i = 127; i >= 0; i--) {
    if (is_set_bit(decimal, i)) {
      result = i;
      break;
    }
  }
  return result;
}

big_decimal create_big_decimal(s21_decimal value_1) {
  big_decimal result;
  result.big_bits[0] = value_1;
  result.big_bits[1] = DECIMAL_ZERO;
  return result;
}

void big_normalize(s21_decimal value_1, s21_decimal value_2,
                   big_decimal* value_1l, big_decimal* value_2l) {
  int scale1 = get_scale(&value_1);
  int scale2 = get_scale(&value_2);
  s21_decimal tmp1 = value_1;
  s21_decimal tmp2 = value_2;

  decimal_only(&tmp1);
  decimal_only(&tmp2);

  if (scale1 > scale2) {
    *value_1l = create_big_decimal(tmp1);
    *value_2l = mul_helper(tmp2, get_pow(scale1 - scale2));
  } else if (scale1 < scale2) {
    *value_1l = mul_helper(tmp1, get_pow(scale2 - scale1));
    *value_2l = create_big_decimal(tmp2);
  } else {
    *value_1l = create_big_decimal(tmp1);
    *value_2l = create_big_decimal(tmp2);
  }
}

big_decimal mul_helper(s21_decimal decimal1, s21_decimal decimal2) {
  big_decimal int256_result = create_big_decimal(DECIMAL_ZERO);
  big_decimal int256_tmp1 = create_big_decimal(decimal1);

  int max_bit = move_bit(decimal2);

  for (int i = 0; i <= max_bit; i++) {
    if (is_set_bit(decimal2, i) != 0) {
      int256_result = big_add(int256_result, int256_tmp1);
    }
    int256_tmp1 = big_shift_L(int256_tmp1, 1);
  }

  return int256_result;
}

s21_decimal s21_abs(s21_decimal value_1) {
  s21_decimal result = value_1;
  set_sign_b(&result, 0);
  return result;
}

s21_decimal error(void) {
  s21_decimal result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF}};

  return result;
}

int is_equal_zero(s21_decimal decimal) {
  return decimal.bits[0] == 0 && decimal.bits[1] == 0 && decimal.bits[2] == 0 &&
         decimal.bits[3] == 0;
}

void decimal_only(s21_decimal* value) { value->bits[3] = 0; }

s21_decimal get_pow(int pow) { return all_ten_pows[pow]; }

int get_sign_b(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];
  return bits3.parts.sign;
}

int get_power(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.power;
}

void set_sign_b(s21_decimal* decimal, int sign) {
  decimal_bit3 bits3;
  bits3.i = decimal->bits[3];
  if (sign == 0) {
    bits3.parts.sign = 0;
  } else {
    bits3.parts.sign = 1;
  }

  decimal->bits[3] = bits3.i;
}

void set_power(s21_decimal* decimal, int power) {
  decimal_bit3 bits3;
  bits3.i = decimal->bits[3];
  bits3.parts.power = power;

  decimal->bits[3] = bits3.i;
}

int checking_decimal(big_decimal value) {
  int cnt = 0;
  if (!(is_equal_zero(value.big_bits[0]) && is_equal_zero(value.big_bits[1]))) {
    big_decimal max = create_big_decimal(DECIMAL_MAX);
    big_decimal quotient = big_div(value, max, NULL);
    while (1) {
      int compare = small_compare(quotient.big_bits[0], get_pow(cnt));
      if (compare == -1 || compare == 0) {
        break;
      }
      ++cnt;
    }
    big_decimal tmp = big_div(value, create_big_decimal(get_pow(cnt)), NULL);
    if (!is_zero(tmp.big_bits[1]) || tmp.big_bits[0].bits[3] != 0) {
      ++cnt;
    }
  }
  return cnt;
}
