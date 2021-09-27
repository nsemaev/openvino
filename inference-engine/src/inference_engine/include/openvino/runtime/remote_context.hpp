// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @brief This is a header file for the OpenVINO Runtime RemoteContext class
 *
 * @file openvino/runtime/remote_context.hpp
 */
#pragma once

#include <map>
#include <memory>
#include <string>

#include "ie_remote_context.hpp"
#include "openvino/core/shape.hpp"
#include "openvino/core/type/element_type.hpp"
#include "openvino/runtime/common.hpp"
#include "openvino/runtime/parameter.hpp"
#include "openvino/runtime/remote_tensor.hpp"

namespace InferenceEngine {
class RemoteBlob;
}  // namespace InferenceEngine

namespace ov {
namespace runtime {

class Core;

/**
 * @brief This class represents an abstraction
 * for remote (non-CPU) accelerator device-specific execution context.
 * Such context represents a scope on the device within which executable
 * networks and remote memory blobs can exist, function and exchange data.
 */
class OPENVINO_RUNTIME_API RemoteContext {
    std::shared_ptr<void> _so;
    std::shared_ptr<ie::RemoteContext> _impl;

    /**
     * @brief Constructs RemoteContext from the initialized std::shared_ptr
     * @param so Plugin to use. This is required to ensure that RemoteContext can work properly even if plugin
     * object is destroyed.
     * @param impl Initialized shared pointer
     */
    RemoteContext(const std::shared_ptr<void>& so, const std::shared_ptr<ie::RemoteContext>& impl);
    friend class Core;

public:
    /**
     * @brief Default constructor
     */
    RemoteContext() = default;

    /**
     * @brief Checks if the RemoteContext object can be cast to the type T*
     *
     * @tparam T Type to be checked. Must represent a class derived from the RemoteContext
     * @return true if this object can be dynamically cast to the type T*. Otherwise, false
     */
    template <typename T,
              typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, int>::type = 0,
              typename std::enable_if<std::is_base_of<ie::RemoteContext, T>::value, int>::type = 0>
    bool is() noexcept {
        return dynamic_cast<T*>(_impl.get()) != nullptr;
    }

    /**
     * @brief Checks if the RemoteContext object can be cast to the type const T*
     *
     * @tparam T Type to be checked. Must represent a class derived from the RemoteContext
     * @return true if this object can be dynamically cast to the type const T*. Otherwise, false
     */
    template <typename T,
              typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, int>::type = 0,
              typename std::enable_if<std::is_base_of<ie::RemoteContext, T>::value, int>::type = 0>
    bool is() const noexcept {
        return dynamic_cast<const T*>(_impl.get()) != nullptr;
    }

    /**
     * @brief Casts this RemoteContext object to the type T*.
     *
     * @tparam T Type to cast to. Must represent a class derived from the RemoteContext
     * @return Raw pointer to the object of the type T or nullptr on error
     */
    template <typename T,
              typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, int>::type = 0,
              typename std::enable_if<std::is_base_of<ie::RemoteContext, T>::value, int>::type = 0>
    T* as() noexcept {
        return dynamic_cast<T*>(_impl.get());
    }

    /**
     * @brief Casts this RemoteContext object to the type const T*.
     *
     * @tparam T Type to cast to. Must represent a class derived from the RemoteContext
     * @return Raw pointer to the object of the type const T or nullptr on error
     */
    template <typename T,
              typename std::enable_if<!std::is_pointer<T>::value && !std::is_reference<T>::value, int>::type = 0,
              typename std::enable_if<std::is_base_of<ie::RemoteContext, T>::value, int>::type = 0>
    const T* as() const noexcept {
        return dynamic_cast<const T*>(_impl.get());
    }

    /**
     * @brief Returns name of the device on which underlying object is allocated.
     * Abstract method.
     * @return A device name string in fully specified format `<device_name>[.<device_id>[.<tile_id>]]`.
     */
    std::string get_device_name() const;

    /**
     * @brief Allocates memory tensor in device memory or wraps user-supplied memory handle
     * using the specified tensor description and low-level device-specific parameters.
     * Returns a pointer to the object which implements RemoteTensor interface.
     * @param type Defines the element type of the tensor
     * @param shape Defines the shape of the tensor
     * @param params Map of the low-level tensor object parameters.
     * Abstract method.
     * @return A pointer to plugin object that implements RemoteTensor interface.
     */
    RemoteTensor create_tensor(const element::Type& type, const Shape& shape, const ParamMap& params = {});

    /**
     * @brief Returns a map of device-specific parameters required for low-level
     * operations with underlying object.
     * Parameters include device/context handles, access flags,
     * etc. Content of the returned map depends on remote execution context that is
     * currently set on the device (working scenario).
     * Abstract method.
     * @return A map of name/parameter elements.
     */
    ParamMap get_params() const;
};

}  // namespace runtime
}  // namespace ov
