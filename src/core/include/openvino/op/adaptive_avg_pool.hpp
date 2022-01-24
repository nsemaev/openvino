// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "openvino/op/op.hpp"
#include "openvino/op/util/attr_types.hpp"

namespace ov {
namespace op {
namespace v8 {
/// \brief Adaptive average pooling operation.
///
class OPENVINO_API AdaptiveAvgPool : public Op {
public:
    OPENVINO_OP("AdaptiveAvgPool", "opset8");
    BWDCMP_RTTI_DECLARATION;

    AdaptiveAvgPool() = default;

    ///
    /// \brief    Constructs adaptive average pooling operation.
    ///
    /// \param    data            Input data
    ///
    /// \param    output_shape    1D tensor describing output shape for spatial
    ///                           dimensions.
    ///
    AdaptiveAvgPool(const Output<Node>& data, const Output<Node>& output_shape);

    void validate_and_infer_types() override;
    bool visit_attributes(AttributeVisitor& visitor) override;

    std::shared_ptr<Node> clone_with_new_inputs(const OutputVector& new_args) const override;
};
}  // namespace v8
}  // namespace op
}  // namespace ov
