#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = CONVERT_GOOD;
  if (result && is_correct(&value)) {
    *result = value;
    //  set the reverse sign
    set_sign(result, !get_sign(&value));
  } else {
    res = CONVERT_ERROR;
  }
  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = CONVERT_GOOD;
  s21_decimal val = value;
  int sign = get_sign(&value);
  set_scale(&val, 0);
  if (result && is_correct(&value)) {
    int scale = get_scale(&value);
    if (scale) {
      set_scale(&value, 0);
      set_sign(&value, 0);
      if (!CheckRightBit(val.bits[0]) &&
          (value.bits[1] != 0 || value.bits[2] != 0)) {
        value.bits[0] |= 0b0000000000000000000000000000111;
      }
      double m = (double)value.bits[0];
      for (; scale > 0; scale--) {
        s21_bindiv(value, result, m);
        value = *result;
      }
      if (sign) set_sign(result, 1);
    } else {
      *result = value;
    }
  } else {
    res = CONVERT_ERROR;
  }
  return res;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = CONVERT_GOOD;
  *result = DECIMAL_ZERO;

  s21_decimal half = {{5, 0, 0, 0}};
  s21_decimal mod_res = {{0, 0, 0, 0}};
  set_scale(&half, 1);

  s21_mod(value, DECIMAL_ONE, &mod_res);
  set_sign(&mod_res, 0);
  s21_truncate(value, result);
  if (!s21_is_less(mod_res, half)) {
    res = add_helper(*result, DECIMAL_ONE, result);
  }
  return res;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = CONVERT_GOOD;
  *result = DECIMAL_ZERO;
  s21_decimal res2 = DECIMAL_ZERO;
  if (result && is_correct(&value)) {
    int sign = get_sign(&value);
    if (sign == 1) {
      s21_negate(value, &value);
    }
    s21_truncate(value, result);
    s21_sub(*result, value, &res2);
    if (!s21_is_equal(res2, DECIMAL_ZERO)) {
      if (sign == 1) {
        add_helper(*result, DECIMAL_ONE, result);
      }
    }
    set_sign(result, sign);
  } else {
    res = CONVERT_ERROR;
  }
  return res;
}
