// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "openvino/op/op.hpp"

namespace ov {
namespace op {
namespace v4 {
/// \brief A Self Regularized Non-Monotonic Neural Activation Function
/// f(x) =  x * tanh(log(exp(x) + 1.))
///
class OPENVINO_API Mish : public Op {
public:
    OPENVINO_RTTI_DECLARATION;

    Mish() = default;
    /// \brief Constructs an Mish operation.
    ///
    /// \param data Input tensor
    Mish(const Output<Node>& arg);
    bool visit_attributes(AttributeVisitor& visitor) override;
    void validate_and_infer_types() override;

    std::shared_ptr<Node> clone_with_new_inputs(const OutputVector& new_args) const override;

    bool evaluate(const HostTensorVector& outputs, const HostTensorVector& inputs) const override;
    bool has_evaluate() const override;
};
}  // namespace v4
}  // namespace op
}  // namespace ov
