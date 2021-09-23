// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "ie_network_reader.hpp"

#include <fstream>
#include <istream>
#include <map>
#include <mutex>

#include "details/ie_so_pointer.hpp"
#include "file_utils.h"
#include "frontend_manager/frontend_manager.hpp"
#include "ie_ir_version.hpp"
#include "ie_itt.hpp"
#include "ie_reader.hpp"

namespace InferenceEngine {

namespace details {

/**
 * @brief This class defines the name of the fabric for creating an IReader object in DLL
 */
template <>
class SOCreatorTrait<IReader> {
public:
    /**
     * @brief A name of the fabric for creating IReader object in DLL
     */
    static constexpr auto name = "CreateReader";
};

}  // namespace details

/**
 * @brief This class is a wrapper for reader interfaces
 */
class Reader : public IReader {
    InferenceEngine::details::SOPointer<IReader> ptr;
    std::once_flag readFlag;
    std::string name;
    std::string location;

    InferenceEngine::details::SOPointer<IReader> getReaderPtr() {
        std::call_once(readFlag, [&]() {
            ov::util::FilePath libraryName = ov::util::to_file_path(location);
            ov::util::FilePath readersLibraryPath =
                FileUtils::makePluginLibraryName(getInferenceEngineLibraryPath(), libraryName);

            if (!FileUtils::fileExist(readersLibraryPath)) {
                IE_THROW() << "Please, make sure that Inference Engine ONNX reader library "
                           << ov::util::from_file_path(::FileUtils::makePluginLibraryName({}, libraryName)) << " is in "
                           << getIELibraryPath();
            }
            ptr = {readersLibraryPath};
        });

        return ptr;
    }

    InferenceEngine::details::SOPointer<IReader> getReaderPtr() const {
        return const_cast<Reader*>(this)->getReaderPtr();
    }

public:
    using Ptr = std::shared_ptr<Reader>;
    Reader(const std::string& name, const std::string location) : name(name), location(location) {}
    bool supportModel(std::istream& model) const override {
        OV_ITT_SCOPED_TASK(ov::itt::domains::IE, "Reader::supportModel");
        auto reader = getReaderPtr();
        return reader->supportModel(model);
    }
    CNNNetwork read(std::istream& model, const std::vector<IExtensionPtr>& exts) const override {
        auto reader = getReaderPtr();
        return reader->read(model, exts);
    }
    CNNNetwork read(std::istream& model,
                    const Blob::CPtr& weights,
                    const std::vector<IExtensionPtr>& exts) const override {
        auto reader = getReaderPtr();
        return reader->read(model, weights, exts);
    }
    std::vector<std::string> getDataFileExtensions() const override {
        auto reader = getReaderPtr();
        return reader->getDataFileExtensions();
    }
    std::string getName() const {
        return name;
    }
};

namespace {

// Extension to plugins creator
std::multimap<std::string, Reader::Ptr> readers;

static ngraph::frontend::FrontEndManager& get_frontend_manager() {
    static ngraph::frontend::FrontEndManager manager;
    return manager;
}

void registerReaders() {
    OV_ITT_SCOPED_TASK(ov::itt::domains::IE, "registerReaders");
    static bool initialized = false;
    static std::mutex readerMutex;
    std::lock_guard<std::mutex> lock(readerMutex);
    if (initialized)
        return;

    // TODO: Read readers info from XML
    auto create_if_exists = [](const std::string name, const std::string library_name) {
        ov::util::FilePath libraryName = ov::util::to_file_path(library_name);
        ov::util::FilePath readersLibraryPath =
            FileUtils::makePluginLibraryName(getInferenceEngineLibraryPath(), libraryName);

        if (!FileUtils::fileExist(readersLibraryPath))
            return std::shared_ptr<Reader>();
        return std::make_shared<Reader>(name, library_name);
    };

    // try to load IR reader v7 if library exists
    auto irReaderv7 =
        create_if_exists("IRv7", std::string("inference_engine_ir_v7_reader") + std::string(IE_BUILD_POSTFIX));
    if (irReaderv7)
        readers.emplace("xml", irReaderv7);

    initialized = true;
}

void assertIfIRv7LikeModel(std::istream& modelStream) {
    auto irVersion = details::GetIRVersion(modelStream);
    bool isIRv7 = irVersion > 1 && irVersion <= 7;

    if (!isIRv7)
        return;

    for (auto&& kvp : readers) {
        Reader::Ptr reader = kvp.second;
        if (reader->getName() == "IRv7") {
            return;
        }
    }

    IE_THROW() << "The support of IR v" << irVersion
               << " has been removed from the product. "
                  "Please, convert the original model using the Model Optimizer which comes with this "
                  "version of the OpenVINO to generate supported IR version.";
}

ov::Extensions get_extensions_map(const std::vector<InferenceEngine::IExtensionPtr>& exts) {
    ov::Extensions extensions;
    for (const auto& ext : exts) {
        for (const auto& item : ext->getOpSets()) {
            if (extensions.count(item.first)) {
                IE_THROW() << "Extension with " << item.first << " name already exists";
            }
            extensions[item.first] = item.second;
        }
    }
    return extensions;
}

}  // namespace

CNNNetwork details::ReadNetwork(const std::string& modelPath,
                                const std::string& binPath,
                                const std::vector<IExtensionPtr>& exts) {
    // Register readers if it is needed
    registerReaders();

    // Fix unicode name
#if defined(OPENVINO_ENABLE_UNICODE_PATH_SUPPORT) && defined(_WIN32)
    std::wstring model_path = ov::util::string_to_wstring(modelPath.c_str());
#else
    std::string model_path = modelPath;
#endif
    // Try to open model file
    std::ifstream modelStream(model_path, std::ios::binary);
    if (!modelStream.is_open())
        IE_THROW() << "Model file " << modelPath << " cannot be opened!";

    assertIfIRv7LikeModel(modelStream);

    // TODO: this code is needed only by V7 IR reader. So we need to remove it in future.
    auto fileExt = modelPath.substr(modelPath.find_last_of(".") + 1);
    for (auto it = readers.lower_bound(fileExt); it != readers.upper_bound(fileExt); it++) {
        auto reader = it->second;
        // Check that reader supports the model
        if (reader->supportModel(modelStream)) {
            // Find weights
            std::string bPath = binPath;
            if (bPath.empty()) {
                auto pathWoExt = modelPath;
                auto pos = modelPath.rfind('.');
                if (pos != std::string::npos)
                    pathWoExt = modelPath.substr(0, pos);
                for (const auto& ext : reader->getDataFileExtensions()) {
                    bPath = pathWoExt + "." + ext;
                    if (!FileUtils::fileExist(bPath)) {
                        bPath.clear();
                    } else {
                        break;
                    }
                }
            }
            if (!bPath.empty()) {
                // Open weights file
#if defined(OPENVINO_ENABLE_UNICODE_PATH_SUPPORT) && defined(_WIN32)
                std::wstring weights_path = ov::util::string_to_wstring(bPath.c_str());
#else
                std::string weights_path = bPath;
#endif
                std::ifstream binStream;
                binStream.open(weights_path, std::ios::binary);
                if (!binStream.is_open())
                    IE_THROW() << "Weights file " << bPath << " cannot be opened!";

                binStream.seekg(0, std::ios::end);
                size_t fileSize = binStream.tellg();
                binStream.seekg(0, std::ios::beg);

                Blob::Ptr weights = make_shared_blob<uint8_t>({Precision::U8, {fileSize}, C});

                {
                    OV_ITT_SCOPE(FIRST_INFERENCE, ov::itt::domains::IE_RT, "ReadNetworkWeights");
                    weights->allocate();
                    binStream.read(weights->buffer(), fileSize);
                    binStream.close();
                }

                // read model with weights
                auto network = reader->read(modelStream, weights, exts);
                modelStream.close();
                return network;
            }
            // read model without weights
            return reader->read(modelStream, exts);
        }
    }

    // Try to load with FrontEndManager
    auto& manager = get_frontend_manager();
    ngraph::frontend::FrontEnd::Ptr FE;
    ngraph::frontend::InputModel::Ptr inputModel;

    ov::VariantVector params{ov::make_variant(model_path)};
    if (!exts.empty()) {
        params.emplace_back(ov::make_variant(get_extensions_map(exts)));
    }

    if (!binPath.empty()) {
#if defined(OPENVINO_ENABLE_UNICODE_PATH_SUPPORT) && defined(_WIN32)
        const std::wstring& weights_path = ov::util::string_to_wstring(binPath.c_str());
#else
        const std::string& weights_path = binPath;
#endif
        params.emplace_back(ov::make_variant(weights_path));
    }

    FE = manager.load_by_model(params);
    if (FE)
        inputModel = FE->load(params);

    if (inputModel) {
        auto ngFunc = FE->convert(inputModel);
        return CNNNetwork(ngFunc, exts);
    }
    IE_THROW(NetworkNotRead) << "Unable to read the model: " << modelPath
                             << " Please check that model format: " << fileExt
                             << " is supported and the model is correct.";
}

CNNNetwork details::ReadNetwork(const std::string& model,
                                const Blob::CPtr& weights,
                                const std::vector<IExtensionPtr>& exts) {
    // Register readers if it is needed
    registerReaders();
    std::istringstream modelStringStream(model);
    std::istream& modelStream = modelStringStream;

    assertIfIRv7LikeModel(modelStream);

    for (auto it = readers.begin(); it != readers.end(); it++) {
        auto reader = it->second;
        if (reader->supportModel(modelStream)) {
            if (weights)
                return reader->read(modelStream, weights, exts);
            return reader->read(modelStream, exts);
        }
    }

    // Try to load with FrontEndManager
    auto& manager = get_frontend_manager();
    ngraph::frontend::FrontEnd::Ptr FE;
    ngraph::frontend::InputModel::Ptr inputModel;

    ov::VariantVector params{ov::make_variant(&modelStream)};
    if (weights) {
        char* data = weights->cbuffer().as<char*>();
        ov::Weights weights_buffer =
            std::make_shared<ngraph::runtime::SharedBuffer<Blob::CPtr>>(data, weights->byteSize(), weights);
        params.emplace_back(ov::make_variant(weights_buffer));
    }
    if (!exts.empty()) {
        params.emplace_back(ov::make_variant(get_extensions_map(exts)));
    }

    FE = manager.load_by_model(params);
    if (FE)
        inputModel = FE->load(params);
    if (inputModel) {
        auto ngFunc = FE->convert(inputModel);
        return CNNNetwork(ngFunc, exts);
    }

    IE_THROW(NetworkNotRead)
        << "Unable to read the model. Please check if the model format is supported and model is correct.";
}

}  // namespace InferenceEngine
