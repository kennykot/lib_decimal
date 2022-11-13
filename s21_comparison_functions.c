#include "s21_decimal.h"

//  Функция сравнения
int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int is_less = 0;
  int sign_1 = get_sign(&dec1);
  int sign_2 = get_sign(&dec2);
  normalize(&dec1, &dec2);
  if (!sign_1 && !sign_2) {  // Если оба числа отрицательные выравниваем их по
                             // плавающей точке
    is_less = s21_is_less_simple(
        dec1, dec2);  // и простым перебором ищем отличающиеся биты
  } else if (sign_1 ^
             sign_2) {  // Если знаки разные, то присваем переменной 1 или 0
    is_less = (sign_1) ? 1 : 0;
  } else {
    is_less = !s21_is_less_simple(dec1, dec2);
  }
  return is_less;
}

//  Функция сравнения для простых условий
int s21_is_less_simple(s21_decimal dec1, s21_decimal dec2) {
  int is_less = 0;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(dec1, i) ^ get_bit(dec2, i)) {  //  Перебор битов
      is_less = get_bit(dec2, i) != 0 ? 1 : 0;
      break;
    }
  }
  return is_less;
}

/* Логика условий понятна по предыдущей функции */
int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int is_equal;
  int sign_1 = get_sign(&dec1);
  int sign_2 = get_sign(&dec2);
  normalize(&dec1, &dec2);
  if (sign_1 ^ sign_2) {
    is_equal = 0;
    if (s21_is_equal_simple(dec1, DECIMAL_ZERO) &&
        s21_is_equal_simple(dec2, DECIMAL_ZERO)) {
      is_equal = 1;
    }
  } else {
    is_equal = s21_is_equal_simple(dec1, dec2);
  }
  return is_equal;
}

//  Еще одна функция сравнения когда все охуенно
int s21_is_equal_simple(s21_decimal dec1, s21_decimal dec2) {
  int is_equal = 1;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(dec1, i) != get_bit(dec2, i)) {  // Здесь тоже
      is_equal = 0;
      break;
    }
  }
  return is_equal;
}

/* Производные первых двух функций */
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_less(dec2, dec1);
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal(dec1, dec2);
}
