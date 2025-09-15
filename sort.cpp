//
// Created by Андрей Подвысоцкий on 15.09.2025.
//

#pragma once
#include <utility>   // std::move, std::swap
#include <cstddef>   // std::ptrdiff_t
#include <cassert>

namespace qs {

#ifndef QS_INSERTION_CUTOFF
#define QS_INSERTION_CUTOFF 16   // можно переопределить флагом компиляции
#endif

// insertion sort на [lo, hi) с использованием move для сдвигов
template <typename T, typename Compare>
static inline void insertion_sort(T* lo, T* hi, Compare comp) {
    for (T* i = lo + 1; i < hi; ++i) {
        T key = std::move(*i);           // перемещаем ключ
        T* j = i;
        // сдвигаем элементы вправо, пока они > key
        while (j > lo && comp(key, *(j - 1))) {
            *j = std::move(*(j - 1));    // двигаем (move-assign)
            --j;
        }
        *j = std::move(key);             // вставляем ключ
    }
}

// медиана трёх: возвращает УКАЗАТЕЛЬ на выбранный опорный элемент
template <typename T, typename Compare>
static inline T* median_of_three(T* a, T* b, T* c, Compare comp) {
    // хотим вернуть адрес элемента, который по порядку стоит посередине
    // компаратор: comp(x,y) == true, если x < y
    if (comp(*a, *b)) {         // a < b
        if (comp(*b, *c))       // a < b < c
            return b;
        else if (comp(*a, *c))  // a < c <= b
            return c;
        else                    // c <= a < b
            return a;
    } else {                    // b <= a
        if (comp(*a, *c))       // b <= a < c
            return a;
        else if (comp(*b, *c))  // b < c <= a
            return c;
        else                    // c <= b <= a
            return b;
    }
}

// разделение (Lomuto) с опорным в *(hi-1); возвращает указатель на новую позицию опорного
template <typename T, typename Compare>
static inline T* partition_lomuto(T* lo, T* hi, Compare comp) {
    // предполагается, что pivot уже находится по адресу pivot_pos = hi-1
    T* pivot_pos = hi - 1;
    T* i = lo;
    for (T* j = lo; j < pivot_pos; ++j) {
        if (comp(*j, *pivot_pos)) {
            if (i != j) {
                T tmp = std::move(*i);
                *i = std::move(*j);
                *j = std::move(tmp);
            }
            ++i;
        }
    }
    if (i != pivot_pos) {
        T tmp = std::move(*i);
        *i = std::move(*pivot_pos);
        *pivot_pos = std::move(tmp);
    }
    return i;
}

// публичный API
template <typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    // итеративная быстрая сортировка + рекурсия на меньшую часть + вставки на коротких
    while (true) {
        std::ptrdiff_t n = last - first;
        if (n <= 1) return;

        if (n <= QS_INSERTION_CUTOFF) {
            insertion_sort(first, last, comp);
            return;
        }

        T* lo = first;
        T* hi = last;

        T* mid = lo + n / 2;
        T* med = median_of_three(lo, mid, hi - 1, comp);
        // переносим опорный в конец для Lomuto
        if (med != hi - 1) {
            T tmp = std::move(*(hi - 1));
            *(hi - 1) = std::move(*med);
            *med = std::move(tmp);
        }

        T* p = partition_lomuto(lo, hi, comp); // p указывает на место опорного

        // теперь два подотрезка: [lo, p) и [p+1, hi)
        std::ptrdiff_t left = p - lo;
        std::ptrdiff_t right = hi - (p + 1);

        // хвостовая рекурсия: рекурсивно сортируем меньший отрезок
        if (left < right) {
            if (left > 1) sort(lo, p, comp);
            // итеративно продолжим на большом хвосте
            first = p + 1;
        } else {
            if (right > 1) sort(p + 1, hi, comp);
            last = p;
        }
    }
}

} // namespace qs
