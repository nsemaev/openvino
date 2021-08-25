// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cstring>
#include <map>

#include "ngraph/descriptor/tensor.hpp"
#include "ngraph/partial_shape.hpp"
#include "ngraph/shape.hpp"
#include "ngraph/type/element_type.hpp"
#include "ngraph/variant.hpp"

namespace ngraph {
class Node;

template <typename NodeType>
class Output;

template <typename NodeType>
class Input {};

/// \brief A handle for one of a node's inputs.
template <>
class NGRAPH_API Input<Node> {
public:
    /// \brief Constructs a Input.
    /// \param node Pointer to the node for the input handle.
    /// \param index The index of the input.
    Input(Node* node, size_t index);

    /// \return A pointer to the node referenced by this input handle.
    Node* get_node() const;
    /// \return The index of the input referred to by this input handle.
    size_t get_index() const;
    /// \return The element type of the input referred to by this input handle.
    const element::Type& get_element_type() const;
    /// \return The shape of the input referred to by this input handle.
    const Shape& get_shape() const;
    /// \return The partial shape of the input referred to by this input handle.
    const PartialShape& get_partial_shape() const;
    /// \return A handle to the output that is connected to this input.
    Output<Node> get_source_output() const;
    /// \return A reference to the tensor descriptor for this input.
    descriptor::Tensor& get_tensor() const;
    /// \return A shared pointer to the tensor descriptor for this input.
    std::shared_ptr<descriptor::Tensor> get_tensor_ptr() const;
    /// \return true if this input is relevant to its node's output shapes; else false.
    bool get_is_relevant_to_shapes() const;
    /// \return true if this input is relevant to its node's output values; else false.
    bool get_is_relevant_to_values() const;

    /// \brief Replaces the source output of this input.
    /// \param new_source_output A handle for the output that will replace this input's source.
    void replace_source_output(const Output<Node>& new_source_output) const;

    /// \return The reference to runtime info map
    RTMap& get_rt_info();
    /// \return The constant reference to runtime info map
    const RTMap& get_rt_info() const;

    bool operator==(const Input& other) const;
    bool operator!=(const Input& other) const;
    bool operator<(const Input& other) const;
    bool operator>(const Input& other) const;
    bool operator<=(const Input& other) const;
    bool operator>=(const Input& other) const;

private:
    Node* const m_node;
    const size_t m_index;
};

/// \brief A handle for one of a node's inputs.
template <>
class NGRAPH_API Input<const Node> {
public:
    /// \brief Constructs a Input.
    /// \param node Pointer to the node for the input handle.
    /// \param index The index of the input.
    Input(const Node* node, size_t index);

    /// \return A pointer to the node referenced by this input handle.
    const Node* get_node() const;
    /// \return The index of the input referred to by this input handle.
    size_t get_index() const;
    /// \return The element type of the input referred to by this input handle.
    const element::Type& get_element_type() const;
    /// \return The shape of the input referred to by this input handle.
    const Shape& get_shape() const;
    /// \return The partial shape of the input referred to by this input handle.
    const PartialShape& get_partial_shape() const;
    /// \return A handle to the output that is connected to this input.
    Output<Node> get_source_output() const;
    /// \return A reference to the tensor descriptor for this input.
    descriptor::Tensor& get_tensor() const;
    /// \return A shared pointer to the tensor descriptor for this input.
    std::shared_ptr<descriptor::Tensor> get_tensor_ptr() const;
    /// \return true if this input is relevant to its node's output shapes; else false.
    bool get_is_relevant_to_shapes() const;
    /// \return true if this input is relevant to its node's output values; else false.
    bool get_is_relevant_to_values() const;

    /// \return The constant reference to runtime info map
    const RTMap& get_rt_info() const;

    bool operator==(const Input& other) const;
    bool operator!=(const Input& other) const;
    bool operator<(const Input& other) const;
    bool operator>(const Input& other) const;
    bool operator<=(const Input& other) const;
    bool operator>=(const Input& other) const;

private:
    const Node* const m_node;
    const size_t m_index;
};

NGRAPH_API std::ostream& operator<<(std::ostream& out, const Input<Node>& input);
NGRAPH_API std::ostream& operator<<(std::ostream& out, const Input<const Node>& input);
}  // namespace ngraph
