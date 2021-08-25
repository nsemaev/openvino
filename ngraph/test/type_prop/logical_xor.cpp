#include "logical_ops.hpp"
#include "util/type_prop.hpp"

using Type = ::testing::Types<LogicalOperatorType<ngraph::op::v1::LogicalXor, ngraph::element::boolean>>;

INSTANTIATE_TYPED_TEST_SUITE_P(Type_prop_test, LogicalOperatorTypeProp, Type, LogicalOperatorTypeName);
