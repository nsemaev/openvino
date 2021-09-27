// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "cpp/ie_infer_request.hpp"

#include <map>
#include <memory>
#include <string>

#include "cpp_interfaces/interface/ie_iinfer_request_internal.hpp"
#include "ie_infer_async_request_base.hpp"
#include "ie_ngraph_utils.hpp"
#include "ie_remote_context.hpp"
#include "openvino/core/except.hpp"
#include "openvino/runtime/infer_request.hpp"
#include "transformations/utils/utils.hpp"

namespace InferenceEngine {

#define INFER_REQ_CALL_STATEMENT(...)                                     \
    if (_impl == nullptr)                                                 \
        IE_THROW(NotAllocated) << "Inference Request is not initialized"; \
    try {                                                                 \
        __VA_ARGS__                                                       \
    } catch (...) {                                                       \
        ::InferenceEngine::details::Rethrow();                            \
    }

#define OV_INFER_REQ_CALL_STATEMENT(...)                                    \
    OPENVINO_ASSERT(_impl != nullptr, "InferRequest was not initialized."); \
    try {                                                                   \
        __VA_ARGS__;                                                        \
    } catch (const std::exception& ex) {                                    \
        throw ov::Exception(ex.what());                                     \
    } catch (...) {                                                         \
        OPENVINO_ASSERT(false, "Unexpected exception");                     \
    }

InferRequest::InferRequest(const details::SharedObjectLoader& so, const IInferRequestInternal::Ptr& impl)
    : _so(so),
      _impl(impl) {
    IE_ASSERT(_impl != nullptr);
}

IE_SUPPRESS_DEPRECATED_START

void InferRequest::SetBlob(const std::string& name, const Blob::Ptr& data) {
    INFER_REQ_CALL_STATEMENT(_impl->SetBlob(name, data);)
}

Blob::Ptr InferRequest::GetBlob(const std::string& name) {
    Blob::Ptr blobPtr;
    INFER_REQ_CALL_STATEMENT(blobPtr = _impl->GetBlob(name);)
    std::string error = "Internal error: blob with name `" + name + "` is not allocated!";
    const bool remoteBlobPassed = blobPtr->is<RemoteBlob>();
    if (blobPtr == nullptr)
        IE_THROW() << error;
    if (!remoteBlobPassed && blobPtr->buffer() == nullptr)
        IE_THROW() << error;
    return blobPtr;
}

void InferRequest::SetBlob(const std::string& name, const Blob::Ptr& data, const PreProcessInfo& info) {
    INFER_REQ_CALL_STATEMENT(_impl->SetBlob(name, data, info);)
}

const PreProcessInfo& InferRequest::GetPreProcess(const std::string& name) const {
    INFER_REQ_CALL_STATEMENT(return _impl->GetPreProcess(name);)
}

void InferRequest::Infer() {
    INFER_REQ_CALL_STATEMENT(_impl->Infer();)
}

void InferRequest::Cancel() {
    INFER_REQ_CALL_STATEMENT(_impl->Cancel();)
}

std::map<std::string, InferenceEngineProfileInfo> InferRequest::GetPerformanceCounts() const {
    INFER_REQ_CALL_STATEMENT(return _impl->GetPerformanceCounts();)
}

void InferRequest::SetInput(const BlobMap& inputs) {
    INFER_REQ_CALL_STATEMENT(for (auto&& input : inputs) { _impl->SetBlob(input.first, input.second); })
}

void InferRequest::SetOutput(const BlobMap& results) {
    INFER_REQ_CALL_STATEMENT(for (auto&& result : results) { _impl->SetBlob(result.first, result.second); })
}

void InferRequest::SetBatch(const int batch) {
    INFER_REQ_CALL_STATEMENT(_impl->SetBatch(batch);)
}

void InferRequest::StartAsync() {
    INFER_REQ_CALL_STATEMENT(_impl->StartAsync();)
}

StatusCode InferRequest::Wait(int64_t millis_timeout) {
    INFER_REQ_CALL_STATEMENT(return _impl->Wait(millis_timeout);)
}

void InferRequest::SetCompletionCallbackImpl(std::function<void()> callbackToSet) {
    INFER_REQ_CALL_STATEMENT(_impl->SetCallback([callbackToSet](std::exception_ptr) {
        callbackToSet();
    });)
}

#define CATCH_IE_EXCEPTION_RETURN(StatusCode, ExceptionType) \
    catch (const ExceptionType&) {                           \
        return StatusCode;                                   \
    }

#define CATCH_IE_EXCEPTIONS_RETURN                                   \
    CATCH_IE_EXCEPTION_RETURN(GENERAL_ERROR, GeneralError)           \
    CATCH_IE_EXCEPTION_RETURN(NOT_IMPLEMENTED, NotImplemented)       \
    CATCH_IE_EXCEPTION_RETURN(NETWORK_NOT_LOADED, NetworkNotLoaded)  \
    CATCH_IE_EXCEPTION_RETURN(PARAMETER_MISMATCH, ParameterMismatch) \
    CATCH_IE_EXCEPTION_RETURN(NOT_FOUND, NotFound)                   \
    CATCH_IE_EXCEPTION_RETURN(OUT_OF_BOUNDS, OutOfBounds)            \
    CATCH_IE_EXCEPTION_RETURN(UNEXPECTED, Unexpected)                \
    CATCH_IE_EXCEPTION_RETURN(REQUEST_BUSY, RequestBusy)             \
    CATCH_IE_EXCEPTION_RETURN(RESULT_NOT_READY, ResultNotReady)      \
    CATCH_IE_EXCEPTION_RETURN(NOT_ALLOCATED, NotAllocated)           \
    CATCH_IE_EXCEPTION_RETURN(INFER_NOT_STARTED, InferNotStarted)    \
    CATCH_IE_EXCEPTION_RETURN(NETWORK_NOT_READ, NetworkNotRead)      \
    CATCH_IE_EXCEPTION_RETURN(INFER_CANCELLED, InferCancelled)

void InferRequest::SetCompletionCallbackImpl(std::function<void(InferRequest, StatusCode)> callbackToSet) {
    INFER_REQ_CALL_STATEMENT(
        auto weakThis =
            InferRequest{_so, std::shared_ptr<IInferRequestInternal>{_impl.get(), [](IInferRequestInternal*) {}}};
        _impl->SetCallback([callbackToSet, weakThis](std::exception_ptr exceptionPtr) {
            StatusCode statusCode = StatusCode::OK;
            if (exceptionPtr != nullptr) {
                statusCode = [&] {
                    try {
                        std::rethrow_exception(exceptionPtr);
                    }
                    CATCH_IE_EXCEPTIONS_RETURN catch (const std::exception&) {
                        return GENERAL_ERROR;
                    }
                    catch (...) {
                        return UNEXPECTED;
                    }
                }();
            }
            callbackToSet(weakThis, statusCode);
        });)
}

void InferRequest::SetCompletionCallbackImpl(IInferRequest::CompletionCallback callbackToSet) {
    INFER_REQ_CALL_STATEMENT(
        IInferRequest::Ptr weakThis =
            InferRequest{_so, std::shared_ptr<IInferRequestInternal>{_impl.get(), [](IInferRequestInternal*) {}}};
        _impl->SetCallback([callbackToSet, weakThis](std::exception_ptr exceptionPtr) {
            StatusCode statusCode = StatusCode::OK;
            if (exceptionPtr != nullptr) {
                statusCode = [&] {
                    try {
                        std::rethrow_exception(exceptionPtr);
                    }
                    CATCH_IE_EXCEPTIONS_RETURN catch (const std::exception&) {
                        return GENERAL_ERROR;
                    }
                    catch (...) {
                        return UNEXPECTED;
                    }
                }();
            }
            callbackToSet(weakThis, statusCode);
        });)
}

InferRequest::operator IInferRequest::Ptr() {
    INFER_REQ_CALL_STATEMENT(return std::make_shared<InferRequestBase>(_impl);)
}

std::vector<VariableState> InferRequest::QueryState() {
    std::vector<VariableState> controller;
    INFER_REQ_CALL_STATEMENT(for (auto&& state
                                  : _impl->QueryState()) {
        controller.emplace_back(VariableState{_so, state});
    })
    return controller;
}

bool InferRequest::operator!() const noexcept {
    return !_impl;
}

InferRequest::operator bool() const noexcept {
    return (!!_impl);
}

bool InferRequest::operator!=(const InferRequest& r) const noexcept {
    return !(r == *this);
}

bool InferRequest::operator==(const InferRequest& r) const noexcept {
    return r._impl == _impl;
}

}  // namespace InferenceEngine

namespace ov {
namespace runtime {

InferRequest::InferRequest(const std::shared_ptr<void>& so, const ie::IInferRequestInternal::Ptr& impl)
    : _so{so},
      _impl{impl} {
    OPENVINO_ASSERT(_impl != nullptr, "InferRequest was not initialized.");
}

void InferRequest::set_tensor(const std::string& name, const Tensor& tensor){
    OV_INFER_REQ_CALL_STATEMENT({ _impl->SetBlob(name, tensor._impl); })}

Tensor InferRequest::get_tensor(const std::string& name) {
    OV_INFER_REQ_CALL_STATEMENT({
        auto blob = _impl->GetBlob(name);
        const bool remoteBlobPassed = blob->is<ie::RemoteBlob>();
        if (blob == nullptr) {
            IE_THROW(NotAllocated) << "Internal tensor implementation with name `" << name << "` is not allocated!";
        }
        if (!remoteBlobPassed && blob->buffer() == nullptr) {
            IE_THROW(NotAllocated) << "Internal tensor implementation with name `" << name << "` is not allocated!";
        }
        auto tensorDesc = blob->getTensorDesc();
        auto dims = tensorDesc.getDims();
        return {_so, blob};
    })
}

void InferRequest::infer() {
    OV_INFER_REQ_CALL_STATEMENT(_impl->Infer();)
}

void InferRequest::cancel() {
    OV_INFER_REQ_CALL_STATEMENT(_impl->Cancel();)
}

std::vector<ProfilingInfo> InferRequest::get_profiling_info() const {
    OV_INFER_REQ_CALL_STATEMENT({
        auto ieInfos = _impl->GetPerformanceCounts();
        std::vector<ProfilingInfo> infos;
        infos.reserve(ieInfos.size());
        while (!ieInfos.empty()) {
            auto itIeInfo = std::min_element(
                std::begin(ieInfos),
                std::end(ieInfos),
                [](const decltype(ieInfos)::value_type& lhs, const decltype(ieInfos)::value_type& rhs) {
                    return lhs.second.execution_index < rhs.second.execution_index;
                });
            IE_ASSERT(itIeInfo != ieInfos.end());
            auto& ieInfo = itIeInfo->second;
            infos.push_back(ProfilingInfo{});
            auto& info = infos.back();
            switch (ieInfo.status) {
            case ie::InferenceEngineProfileInfo::NOT_RUN:
                info.status = ProfilingInfo::Status::NOT_RUN;
                break;
            case ie::InferenceEngineProfileInfo::OPTIMIZED_OUT:
                info.status = ProfilingInfo::Status::OPTIMIZED_OUT;
                break;
            case ie::InferenceEngineProfileInfo::EXECUTED:
                info.status = ProfilingInfo::Status::OPTIMIZED_OUT;
                break;
            }
            info.real_time = std::chrono::microseconds{ieInfo.realTime_uSec};
            info.cpu_time = std::chrono::microseconds{ieInfo.cpu_uSec};
            info.node_name = itIeInfo->first;
            info.exec_type = std::string{ieInfo.exec_type};
            info.node_type = std::string{ieInfo.layer_type};
            ieInfos.erase(itIeInfo);
        }
        return infos;
    })
}

void InferRequest::start_async() {
    OV_INFER_REQ_CALL_STATEMENT(_impl->StartAsync();)
}

void InferRequest::wait() {
    OV_INFER_REQ_CALL_STATEMENT(_impl->Wait(ie::InferRequest::RESULT_READY);)
}

bool InferRequest::wait_for(const std::chrono::milliseconds timeout) {
    OV_INFER_REQ_CALL_STATEMENT(return _impl->Wait(timeout.count()) == ie::OK;)
}

void InferRequest::set_callback(std::function<void(std::exception_ptr)> callback) {
    OV_INFER_REQ_CALL_STATEMENT(_impl->SetCallback(std::move(callback));)
}

std::vector<VariableState> InferRequest::query_state() {
    std::vector<VariableState> variable_states;
    OV_INFER_REQ_CALL_STATEMENT({
        for (auto&& state : _impl->QueryState()) {
            variable_states.emplace_back(VariableState{_so, state});
        }
    })
    return variable_states;
}

bool InferRequest::operator!() const noexcept {
    return !_impl;
}

InferRequest::operator bool() const noexcept {
    return (!!_impl);
}

bool InferRequest::operator!=(const InferRequest& r) const noexcept {
    return !(r == *this);
}

bool InferRequest::operator==(const InferRequest& r) const noexcept {
    return r._impl == _impl;
}

}  // namespace runtime
}  // namespace ov