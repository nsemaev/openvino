// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <assert.h>
#include <functional>
#include <memory>
#include <iterator>
#include <ostream>

#include <ngraph/node.hpp>
#include <ngraph/variant.hpp>

#include "transformations/rt_info/fused_names_attribute.hpp"

using namespace ngraph;
using namespace ov;

std::string FusedNames::getNames() const {
    std::string res;
    for (auto &name : fused_names) {
        res += (res.empty() ? name : "," + name);
    }
    return res;
}

std::vector<std::string> FusedNames::getVectorNames() const {
    return std::vector<std::string>(fused_names.begin(), fused_names.end());
}

void FusedNames::fuseWith(const FusedNames &names) {
    for (const auto & name : names.fused_names) {
        fused_names.insert(name);
    }
}

std::string ngraph::getFusedNames(const std::shared_ptr<ngraph::Node> &node) {
    const auto &rtInfo = node->get_rt_info();
    using FusedNamesWrapper = VariantWrapper<FusedNames>;

    if (!rtInfo.count(FusedNamesWrapper::get_type_info_static().name)) return {};

    const auto &attr = rtInfo.at(FusedNamesWrapper::get_type_info_static().name);
    FusedNames fusedNames = ov::as_type_ptr<FusedNamesWrapper>(attr)->get();
    return fusedNames.getNames();
}

std::vector<std::string> ngraph::getFusedNamesVector(const std::shared_ptr<ngraph::Node> &node) {
    if (!node) return {};

    const auto &rtInfo = node->get_rt_info();
    using FusedNamesWrapper = VariantWrapper<FusedNames>;

    if (!rtInfo.count(FusedNamesWrapper::get_type_info_static().name)) return {};

    const auto &attr = rtInfo.at(FusedNamesWrapper::get_type_info_static().name);
    FusedNames fusedNames = ov::as_type_ptr<FusedNamesWrapper>(attr)->get();
    return fusedNames.getVectorNames();
}

template class ov::VariantImpl<FusedNames>;

BWDCMP_RTTI_DEFINITION(VariantWrapper<FusedNames>);

std::shared_ptr<ngraph::Variant> VariantWrapper<FusedNames>::merge(const ngraph::NodeVector & nodes) {
    FusedNames mergedNames;
    for (auto &node : nodes) {
        const auto &rtInfo = node->get_rt_info();
        if (!rtInfo.count(VariantWrapper<FusedNames>::get_type_info_static().name)) continue;

        const auto attr = rtInfo.at(VariantWrapper<FusedNames>::get_type_info_static().name);
        if (auto fusedNames = std::dynamic_pointer_cast<VariantWrapper<FusedNames> >(attr)) {
            mergedNames.fuseWith(fusedNames->get());
        }
    }
    return std::make_shared<VariantWrapper<FusedNames> >(mergedNames);
}

std::shared_ptr<ngraph::Variant> VariantWrapper<FusedNames>::init(const std::shared_ptr<ngraph::Node> & node) {
    return std::make_shared<VariantWrapper<FusedNames> > (FusedNames(node->get_friendly_name()));
}
