//
// Created by Андрей Подвысоцкий on 15.09.2025.
//

#include "sort.cpp"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

using qs::sort;

// Вспомогательная проверка упорядоченности
template <typename T, typename Comp>
static bool is_sorted_range(const std::vector<T>& v, Comp comp) {
    for (size_t i = 1; i < v.size(); ++i)
        if (comp(v[i], v[i-1])) return false;
    return true;
}

// move-only нетривиальный тип
struct MoveOnly {
    int key;
    MoveOnly(int k=0): key(k) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& o) noexcept : key(o.key) {}
    MoveOnly& operator=(MoveOnly&& o) noexcept { key = o.key; return *this; }
};

TEST(QSort, EmptyAndSingle) {
    std::vector<int> v;
    sort(v.data(), v.data(), [](const int& a, const int& b){ return a < b; });
    v.push_back(42);
    sort(v.data(), v.data()+1, [](const int& a, const int& b){ return a < b; });
    EXPECT_EQ(v[0], 42);
}

TEST(QSort, AlreadySorted) {
    std::vector<int> v(100);
    for (int i=0;i<100;++i) v[i]=i;
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](int a,int b){return a<b;}));
}

TEST(QSort, Reversed) {
    std::vector<int> v(100);
    for (int i=0;i<100;++i) v[i]=100-i;
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](int a,int b){return a<b;}));
}

TEST(QSort, Duplicates) {
    std::vector<int> v = {5,1,3,3,2,5,5,1,0,0,4,4,4};
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](int a,int b){return a<b;}));
    // проверим мультимножество
    std::vector<int> v2 = v;
    std::sort(v2.begin(), v2.end());
    EXPECT_EQ(v, v2);
}

TEST(QSort, RandomLarge) {
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> d(-100000, 100000);
    std::vector<int> v(5000);
    for (auto& x : v) x = d(rng);
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](int a,int b){return a<b;}));
}

TEST(QSort, Strings) {
    std::vector<std::string> v = {"delta","alpha","charlie","bravo","alpha"};
    sort(v.data(), v.data()+v.size(), [](const std::string& a, const std::string& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](const std::string& a,const std::string& b){return a<b;}));
}

TEST(QSort, CustomComparatorDescending) {
    std::vector<int> v = {1,2,3,4,5};
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a > b; });
    for (int i=0;i<5;++i) EXPECT_EQ(v[i], 5-i);
}

TEST(QSort, MoveOnlyType) {
    std::vector<MoveOnly> v;
    for (int i=10;i>=1;--i) v.emplace_back(i);
    sort(v.data(), v.data()+v.size(), [](const MoveOnly& a, const MoveOnly& b){ return a.key < b.key; });
    for (int i=0;i<10;++i) EXPECT_EQ(v[i].key, i+1);
}

TEST(QSort, SmallCutoffBehaves) {
    // Маленькие массивы должны идти через вставки
    std::vector<int> v = {3,2,1,0};
    sort(v.data(), v.data()+v.size(), [](const int& a, const int& b){ return a < b; });
    EXPECT_TRUE(is_sorted_range(v, [](int a,int b){return a<b;}));
}
