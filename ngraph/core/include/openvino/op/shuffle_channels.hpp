// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <memory>

#include "openvino/op/op.hpp"

namespace ov {
namespace op {
namespace v0 {
/// \brief Permutes data in the channel dimension of the input
class OPENVINO_API ShuffleChannels : public Op {
public:
    OPENVINO_RTTI_DECLARATION;

    ShuffleChannels() = default;
    /// \brief Constructs a ShuffleChannels node.
    ///
    /// \param data     Node producing the input tensor.
    /// \param axis     Channel dimension index in the data tensor.
    ///                 A negative value means that the index should be
    ///                 calculated from the back of the input data shape.
    /// \param group    Number of group the channel dimension should be split into.
    ///
    ShuffleChannels(const Output<Node>& data, const int64_t axis = 1, const int64_t group = 1);

    bool visit_attributes(AttributeVisitor& visitor) override;
    size_t get_zero_based_axis() const;

    void validate_and_infer_types() override;

    std::shared_ptr<Node> clone_with_new_inputs(const OutputVector& new_args) const override;

    int64_t get_axis() const {
        return m_axis;
    }
    int64_t get_group() const {
        return m_group;
    }
    bool evaluate(const HostTensorVector& outputs, const HostTensorVector& inputs) const override;
    bool has_evaluate() const override;

private:
    bool evaluate_shuffle_channels(const HostTensorVector& outputs, const HostTensorVector& inputs) const;

    int64_t m_axis;
    int64_t m_group;
};
}  // namespace v0
}  // namespace op
}  // namespace ov
