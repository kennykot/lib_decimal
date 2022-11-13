#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  while (get_scale(&src) > 0) {
    int bmod;
    unsigned long long buf;
    buf = src.bits[2];
    for (int j = 2; j > 0;) {
      bmod = buf % 10;
      src.bits[j] = buf / 10;
      j--;
      buf = bmod * (4294967295 + 1) + src.bits[j];
      if (j == 0) {
        src.bits[j] = buf / 10;
      }
    }
    set_scale(&src, (get_scale(&src) - 1));
  }
  for (int i = 32; i <= 111; i++) {
    if (get_bit(src, i)) {
      return 1;
    }
  }
  *dst = src.bits[0];
  if (get_sign(&src)) {
    *dst *= -1;
  }
  return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  convert_result result = 0;
  if (dst) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    if (src >= 0) {
      dst->bits[0] = src;
    } else {
      set_sign(dst, 1);
      //  сбиваем знак
      src *= -1;
      dst->bits[0] = src;
    }
  } else {
    result = CONVERT_ERROR;
  }
  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  int new = (int)src;
  int i = 0;
  int res = 0;
  if (!dst) {
    res = 1;
  } else {
    if (src < 0) {
      set_minus(dst);
      src *= -1;
    }
    while (src - ((float)new / (int)(pow(10, i))) != 0) {
      i++;
      new = src *(int)(pow(10, i));
    }
    dst->bits[0] = new;
    set_scale(dst, i);
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = TRUE;
  double temp = 0;
  int fin = 0;
  //  пробегаем ячейки и закидываем биты
  for (int i = 0; i < 95; i++) {
    if ((src.bits[i / 32] & (1 << i % 32)) != 0) {
      //  прибавляем 2 в степени ячейки
      temp += pow(2, i);
    }
  }
  if ((fin = (src.bits[3] & ~0x80000000) >> 16) > 0) {
    for (int i = fin; i > 0; i--) {
      temp /= 10.0;
    }
  }
  *dst = (float)temp;
  //  установка знака
  *dst *= src.bits[3] >> 31 ? -1 : 1;
  result = FALSE;
  return result;
}
