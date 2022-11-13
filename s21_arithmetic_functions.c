#include "s21_decimal.h"

int add_helper(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  int temp_bit = 0;
  for (int i = 0; i < 96; i++) {
    //  check bits
    int bit1 = is_bit(get_bits(value_1, i), i);
    int bit2 = is_bit(get_bits(value_2, i), i);
    //  get pointer to bit's rank
    *rank_bits(result, i) =
        set_bit(get_bits(*result, i), i, bit1 ^ bit2 ^ temp_bit);
    //  check and get bit "in the head"
    temp_bit = (bit1 && bit2) || (bit2 && temp_bit) || (bit1 && temp_bit);
  }
  if (temp_bit) {
    res = 1;
  }
  return res;
}

int check_sign(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  //  get signes from both big_bits
  int sign_1 = get_sign(&value_1) == 1;
  int sign_2 = get_sign(&value_2) == 1;
  if (sign_1 && sign_2) {
    //  if both big_bits are negative
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    res = add_helper(value_1, value_2, result);
    if (res) {
      *result = DECIMAL_ZERO;
      res = 2;
    } else {
      set_sign(result, 1);
    }
  } else if (sign_1) {
    //  if first decimal is negative
    set_sign(&value_1, 0);
    res = s21_sub(value_2, value_1, result);
  } else if (sign_2) {
    //  if second decimal is negative
    set_sign(&value_2, 0);
    res = s21_sub(value_1, value_2, result);
  } else {
    //  if both big_bits are positive
    res = add_helper(value_1, value_2, result);
    if (res) {
      *result = DECIMAL_ZERO;
      res = 1;
    }
  }
  return res;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  //  check scales
  if (result && is_correct(&value_1) && is_correct(&value_2)) {
    //  set result decimal by zero
    *result = DECIMAL_ZERO;
    normalize(&value_1, &value_2);
    //  straight add big_bits
    res = check_sign(value_1, value_2, result);
    //  set scale for resulting decimal
    int scale = get_scale(&value_1);
    set_scale(result, scale);
  } else {
    res = 1;
  }
  return res;
}

int sub_helper(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = get_bit(value_1, i);
    int bit2 = get_bit(value_2, i);
    if (bit2 && !bit1) {
      int j;
      for (j = i + 1; !get_bit(value_1, j) && j < 96;) j++;
      *rank_bits(&value_1, j) = set_bit(get_bits(value_1, j), j, 0);
      for (j--; j != i; j--) {
        *rank_bits(&value_1, j) = set_bit(get_bits(value_1, j), j, 1);
      }
      *rank_bits(result, i) = set_bit(get_bits(*result, i), i, 1);
    } else {
      *rank_bits(result, i) = set_bit(get_bits(*result, i), i, bit1 ^ bit2);
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  if (result && is_correct(&value_1) && is_correct(&value_2)) {
    //  set decimal by zero
    *result = DECIMAL_ZERO;
    normalize(&value_1, &value_2);
    //  set scale for resulting decimal
    int scale = get_scale(&value_1);
    set_scale(result, scale);

    //  get signes from both big_bits
    int sign_1 = get_sign(&value_1);
    int sign_2 = get_sign(&value_2);

    if (sign_1 && sign_2) {
      //  if both big_bits are negative
      set_sign(&value_1, !sign_1);
      set_sign(&value_2, !sign_2);
      res = s21_sub(value_2, value_1, result);
    } else if (sign_1) {
      //  if first decimal is negative
      set_sign(&value_1, !sign_1);
      res = add_helper(value_1, value_2, result);
      if (res) {
        *result = DECIMAL_ZERO;
        res = 2;
      } else {
        set_sign(result, 1);
      }
    } else if (sign_2) {
      //  if second decimal is negative
      set_sign(&value_2, !sign_2);
      res = add_helper(value_1, value_2, result);
    } else if (s21_is_less(value_1, value_2)) {
      // if first decimal is less than second
      res = sub_helper(value_2, value_1, result);
      set_sign(result, 1);
    } else {
      //  if both big_bits are positive
      sub_helper(value_1, value_2, result);
    }
  } else {
    res = 2;
  }
  return res;
}

int mul_normalize(s21_decimal value_1, s21_decimal value_2,
                  s21_decimal *result) {
  int res = 0;
  if (result && is_correct(&value_1) && is_correct(&value_2)) {
    *result = DECIMAL_ZERO;
    int res_scale = (get_scale(&value_1) + get_scale(&value_2));
    int res_sign = (get_sign(&value_1) ^ get_sign(&value_2));
    int i = 95;
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    set_scale(&value_1, 0);
    set_scale(&value_2, 0);
    for (; i >= 0; i--) {
      if (get_bit(value_2, i) != 0 && res == 0)
        res = s21_add(value_1, *result, result);
      if (res != 0) break;
      if (i != 0) {
        if (get_bit(*result, 95) == 0) {
          left_shift_decimal(result, 1);
        } else {
          res = 1;
          break;
        }
      }
    }
    if (res_sign == 1 && res == 1) res = 2;
    set_sign(result, res_sign);
    set_scale(result, res_scale);
    if (res != 0) *result = DECIMAL_ZERO;
  } else {
    res = 1;
  }
  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;

  if (!result) {
    code = 1;
  } else if (!is_correct(&value_1) || !is_correct(&value_2)) {
    code = 1;
    *result = error();
  } else {
    *result = DECIMAL_ZERO;
    s21_decimal res = DECIMAL_ZERO;

    int sign1 = get_sign_b(value_1);
    int sign2 = get_sign_b(value_2);

    if (sign1 == 0 && sign2 == 0) {
      code = mul_helper_2(value_1, value_2, &res);
    } else if (sign1 == 0 && sign2 == 1) {
      code = mul_helper_2(value_1, s21_abs(value_2), &res);
      s21_negate(res, &res);
    } else if (sign1 == 1 && sign2 == 0) {
      code = mul_helper_2(s21_abs(value_1), value_2, &res);
      s21_negate(res, &res);
    } else if (sign1 == 1 && sign2 == 1) {
      code = mul_helper_2(s21_abs(value_1), s21_abs(value_2), &res);
    }
    if (code == 1) {
      if (get_sign_b(res) == 1) {
        code = 2;
      }
    }
    if (code == 0 && s21_is_not_equal(value_1, DECIMAL_ZERO) &&
        s21_is_not_equal(value_2, DECIMAL_ZERO) &&
        s21_is_equal(res, DECIMAL_ZERO)) {
      code = 2;
    }
    *result = res;
  }
  return code;
}

int mul_helper_2(s21_decimal value_1, s21_decimal value_2,
                 s21_decimal *result) {
  int code = 0;
  int power1 = get_power(value_1);
  int power2 = get_power(value_2);
  decimal_only(&value_1);
  decimal_only(&value_2);

  big_decimal res = mul_helper(value_1, value_2);
  int shift = checking_decimal(res);
  int res_power = power1 + power2 - shift;

  if (res_power < 0) {
    code = 1;
    *result = error();
  } else {
    while (shift > 28) {
      res = big_div(res, create_big_decimal(DECIMAL_TEN), NULL);
      --shift;
    }
    if (res_power > 28) {
      big_decimal tmp = res;
      int tmp_power = res_power;
      while (tmp_power > 28) {
        tmp = big_div(tmp, create_big_decimal(DECIMAL_TEN), NULL);
        --tmp_power;
      }
      shift = res_power - tmp_power + shift;
      res_power = tmp_power;
    }

    big_decimal remainder = create_big_decimal(DECIMAL_ZERO);
    big_decimal powerten = create_big_decimal(get_pow(shift));
    res = big_div(res, powerten, &remainder);
    set_power(&remainder.big_bits[0], shift);
    set_power(&res.big_bits[0], res_power);

    if (!is_zero(res.big_bits[1]) || !is_correct(&res.big_bits[0])) {
      code = 1;
      *result = error();
    } else {
      *result = res.big_bits[0];
    }
  }
  return code;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  *result = DECIMAL_ZERO;
  s21_decimal mod_res = DECIMAL_ZERO, new_result = DECIMAL_ZERO;
  //  проверка деления на ноль
  if (s21_is_equal(value_2, DECIMAL_ZERO)) {
    res = 3;
    //  проверка на валидность децималов
  } else if (result && is_correct(&value_1) && is_correct(&value_2)) {
    //  здесь разбираемся со знаками
    int sign_1 = get_sign(&value_1);
    int sign_2 = get_sign(&value_2);
    set_sign(&value_1, 0);
    set_sign(&value_2, 0);
    //  здесь целочисленное деление
    res = div_int(value_1, value_2, &new_result);
    if (res) return res;
    //  а здесь начинается работа с остатком
    res = s21_mod(value_1, value_2, &mod_res);
    if (res) return res;
    float ost = 0, del = 0;
    s21_from_decimal_to_float(mod_res, &ost);
    s21_from_decimal_to_float(value_2, &del);
    float division = ost / del;
    s21_from_float_to_decimal(division, &mod_res);
    //  здесь мы добавляем в результат целую часть
    res = s21_add(*result, new_result, result);
    if (res) return res;
    //  здесь мы добавляем в результат дробную часть
    res = s21_add(*result, mod_res, result);
    if (res) return res;
    //  возвращаем разницу знаков
    if (sign_1 ^ sign_2) {
      set_sign(result, 1);
    }
  } else {
    res = 2;
  }
  return res;
}

int div_int(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = DECIMAL_ZERO;
  int res = 0;
  s21_decimal one = DECIMAL_ONE;
  s21_decimal temp = DECIMAL_ZERO;
  *result = one;
  //  проверка на равенство децималов
  if (s21_is_equal(value_1, value_2)) {
    *result = DECIMAL_ONE;
    //  проверка меньшинство первого децимала
  } else if (s21_is_less(value_1, value_2)) {
    *result = DECIMAL_ZERO;
  } else {
    s21_decimal divcopy = value_2;
    //  сдвигаем делитель влево, пока не станет больше делимого
    while (s21_is_greater(value_1, value_2)) {
      left_shift_decimal(&value_2, 1);
      left_shift_decimal(result, 1);
    }
    //  если перебрали со сдвигом, то сдвигаем вправо 1 раз
    if (s21_is_less(value_1, value_2)) {
      right_shift_decimal(&value_2, 1);
      right_shift_decimal(result, 1);
    }
    //  вычитаем и запоминаем результат
    res = s21_sub(value_1, value_2, &temp);
    if (res) return res;
    //  результат также делим на делитель
    res = div_int(temp, divcopy, &one);
    if (res) return res;
    //  результат записываем из one в result
    res = s21_add(*result, one, result);
    if (res) return res;
  }
  return res;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;

  if (!result) {
    code = 1;
  } else if (!is_correct(&value_1) || !is_correct(&value_2)) {
    code = 1;
    *result = error();
  } else if (s21_is_equal(value_2, DECIMAL_ZERO)) {
    code = 3;
    *result = error();
  } else if (s21_is_less(s21_abs(value_1), s21_abs(value_2))) {
    code = 0;
    *result = value_1;
  } else {
    *result = DECIMAL_ZERO;
    int sign1 = get_sign_b(value_1);
    int scale1 = get_power(value_1);
    int scale2 = get_power(value_2);
    int max_scale = s21_max(scale1, scale2);
    big_decimal big_value_1;
    big_decimal big_value_2;
    big_decimal ostatok = create_big_decimal(DECIMAL_ZERO);
    big_normalize(value_1, value_2, &big_value_1, &big_value_2);
    big_div(big_value_1, big_value_2, &ostatok);
    set_power(&ostatok.big_bits[0], max_scale);
    *result = ostatok.big_bits[0];
    set_sign_b(result, sign1);
  }
  return code;
}

big_decimal big_div(big_decimal decimal1, big_decimal decimal2,
                    big_decimal *mantissa) {
  big_decimal result;
  big_decimal ostatok = create_big_decimal(DECIMAL_ZERO);
  big_decimal del = create_big_decimal(DECIMAL_ZERO);

  if (is_equal_zero(decimal1.big_bits[0]) &&
      is_equal_zero(decimal1.big_bits[1])) {
    del = create_big_decimal(DECIMAL_ZERO);
    ostatok = create_big_decimal(DECIMAL_ZERO);
  } else if (big_compare(decimal1, decimal2) == -1) {
    del = create_big_decimal(DECIMAL_ZERO);
    ostatok = decimal1;
  } else {
    int move1 = move_bit(decimal1.big_bits[1]);
    if (move1 == -1) {
      move1 = move_bit(decimal1.big_bits[0]);
    } else {
      move1 = 128 + move1;
    }

    int move2 = move_bit(decimal2.big_bits[1]);
    if (move2 == -1) {
      move2 = move_bit(decimal2.big_bits[0]);
    } else {
      move2 = 128 + move2;
    }
    int shift = move1 - move2;
    big_decimal move_difference = big_shift_L(decimal2, shift);
    big_decimal dec_copy = decimal1;
    int flag = 1;

    while (shift >= 0) {
      if (flag == 1) {
        ostatok = big_sub(dec_copy, move_difference);
      } else {
        ostatok = big_add(dec_copy, move_difference);
      }
      del = big_shift_L(del, 1);
      if (is_set_bit(ostatok.big_bits[1], 127) == 0) {
        del.big_bits[0] = s21_decimal_set_bit(del.big_bits[0], 0);
      }
      dec_copy = big_shift_L(ostatok, 1);
      if (is_set_bit(ostatok.big_bits[1], 127) == 0) {
        flag = 1;
      } else {
        flag = 0;
      }
      --shift;
    }
    if (is_set_bit(ostatok.big_bits[1], 127)) {
      ostatok = big_add(ostatok, move_difference);
    }
    ostatok = big_shift_R(ostatok, move1 - move2);
  }
  result = del;
  if (mantissa != NULL) {
    *mantissa = ostatok;
  }
  return result;
}
