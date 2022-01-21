// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <inference_engine.hpp>
#include "cpu_shape.h"

#include <algorithm>
#include <cassert>

namespace MKLDNNPlugin {

template<typename T, typename U>
inline T div_up(const T a, const U b) {
    assert(b);
    return (a + b - 1) / b;
}

template<typename T, typename U>
inline T rnd_up(const T a, const U b) {
    return div_up(a, b) * b;
}

template <typename T, typename P>
constexpr bool one_of(T val, P item) { return val == item; }

template <typename T, typename P, typename... Args>
constexpr bool one_of(T val, P item, Args... item_others) {
    return val == item || one_of(val, item_others...);
}

template <typename T, typename P>
constexpr bool everyone_is(T val, P item) { return val == item; }

template <typename T, typename P, typename... Args>
constexpr bool everyone_is(T val, P item, Args... item_others) {
    return val == item && everyone_is(val, item_others...);
}

constexpr inline bool implication(bool cause, bool cond) {
    return !cause || !!cond;
}

template<typename T>
std::string vec2str(const std::vector<T> &vec) {
    if (!vec.empty()) {
        std::ostringstream result;
        result << "(";
        std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T>(result, "."));
        result << vec.back() << ")";
        return result.str();
    }
    return std::string("()");
}

/**
 * @brief Compares that two dims are equal and defined
 * @param lhs
 * first dim
 * @param rhs
 * second dim
 * @return result of comparison
 */
inline bool dimsEqualStrong(size_t lhs, size_t rhs) {
    return (lhs == rhs && lhs != Shape::UNDEFINED_DIM && rhs != Shape::UNDEFINED_DIM);
}

/**
 * @brief Compares that two shapes are equal
 * @param lhs
 * first shape
 * @param rhs
 * second shape
 * @return result of comparison
 */
inline bool dimsEqualStrong(const std::vector<size_t>& lhs, const std::vector<size_t>& rhs, size_t skipAxis = Shape::UNDEFINED_DIM) {
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); i++) {
        if (i != skipAxis && !dimsEqualStrong(lhs[i], rhs[i]))
            return false;
    }

    return true;
}

/**
 * @brief Compares that two dims are equal or undefined
 * @param lhs
 * first dim
 * @param rhs
 * second dim
 * @return result of comparison
 */
inline bool dimsEqualWeak(size_t lhs, size_t rhs) {
    return (lhs == Shape::UNDEFINED_DIM || rhs == Shape::UNDEFINED_DIM || lhs == rhs);
}

/**
 * @brief Compares that two shapes are equal or undefined
 * @param lhs
 * first shape
 * @param rhs
 * second shape
 * @param skipAxis
 * marks shape axis which shouldn't be validated
 * @return result of comparison
 */
inline bool dimsEqualWeak(const std::vector<size_t>& lhs, const std::vector<size_t>& rhs, size_t skipAxis = Shape::UNDEFINED_DIM) {
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); i++) {
        if (i != skipAxis && !dimsEqualWeak(lhs[i], rhs[i]))
            return false;
    }

    return true;
}

inline InferenceEngine::Precision getMaxPrecision(std::vector<InferenceEngine::Precision> precisions) {
    if (!precisions.empty()) {
        return *std::max_element(precisions.begin(), precisions.end(),
                                 [](const InferenceEngine::Precision &lhs, const InferenceEngine::Precision &rhs) {
                                     return lhs.size() > rhs.size();
                                 });
    }

    return InferenceEngine::Precision::UNSPECIFIED;
}

}  // namespace MKLDNNPlugin
