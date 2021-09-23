// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief Defines primitives priority attribute
 * @file primitives_priority_attribute.hpp
 */

#pragma once

#include <assert.h>
#include <functional>
#include <memory>
#include <string>
#include <set>

#include <ngraph/node.hpp>
#include <ngraph/variant.hpp>
#include <transformations_visibility.hpp>

namespace ngraph {

/**
 * @ingroup ie_runtime_attr_api
 * @brief PrimitivesPriority class represents runtime info attribute that
 * can be used for plugins specific primitive choice.
 */
class TRANSFORMATIONS_API PrimitivesPriority {
private:
    std::string primitives_priority;

public:
    /**
     * A default constructor
     */
    PrimitivesPriority() = default;

    /**
     * @brief      Constructs a new object consisting of a single name     *
     * @param[in]  name  The primitives priority value
     */
    explicit PrimitivesPriority(const std::string &primitives_priority) : primitives_priority(primitives_priority) {}

    /**
     * @brief return string with primitives priority value
     */
    std::string getPrimitivesPriority() const;
};
/**
 * @ingroup ie_runtime_attr_api
 * @brief getPrimitivesPriority return string with primitive priorities value
 * @param[in] node The node will be used to get PrimitivesPriority attribute
 */
TRANSFORMATIONS_API std::string getPrimitivesPriority(const std::shared_ptr<ngraph::Node> & node);

}  // namespace ngraph

namespace ov {

extern template class TRANSFORMATIONS_API VariantImpl<ngraph::PrimitivesPriority>;

template<>
class TRANSFORMATIONS_API VariantWrapper<ngraph::PrimitivesPriority> : public VariantImpl<ngraph::PrimitivesPriority> {
public:
    OPENVINO_RTTI("VariantWrapper<PrimitivesPriority>");

    VariantWrapper(const value_type &value) : VariantImpl<value_type>(value) {}

    std::shared_ptr<ov::Variant> merge(const ngraph::NodeVector & nodes) override;

    std::shared_ptr<ov::Variant> init(const std::shared_ptr<ngraph::Node> & node) override;
};

}  // namespace ov
