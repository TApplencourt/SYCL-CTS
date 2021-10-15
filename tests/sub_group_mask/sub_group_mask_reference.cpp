/*******************************************************************************
//
//  SYCL 2020 Extension Conformance Test
//
//  Provides tests to check sub_group_mask operator[] const and reference
//
*******************************************************************************/

#include "sub_group_mask_common.h"

#define TEST_NAME sub_group_mask_reference

namespace TEST_NAMESPACE {

using namespace sycl_cts;
#ifdef SYCL_EXT_ONEAPI_SUB_GROUP_MASK

struct check_result_reference {
  bool operator()(sycl::ext::oneapi::sub_group_mask &sub_group_mask,
                  const sycl::sub_group &) {
    for (size_t N = 0; N < sub_group_mask.size(); N++) {
      auto ref = sub_group_mask[sycl::id(N)];
      if (ref != (N % 2 == 0)) return false;
      switch (N % 5) {
        case 0:
          ref = (N % 2 != 0);
          if (sub_group_mask[sycl::id(N)] != (N % 2 != 0)) return false;
          break;
        case 1:
          if (N == sub_group_mask.size() - 1) break;
          ref = sub_group_mask[sycl::id(N + 1)];
          if (sub_group_mask[sycl::id(N)] != ((N + 1) % 2 == 0)) return false;
          break;
        case 2:
          if (~ref != (N % 2 != 0)) return false;
          break;
        case 3:
          if ((bool)ref != (N % 2 == 0)) return false;
          break;
        case 4:
          if (!std::is_same<sycl::ext::oneapi::sub_group_mask::reference &,
                            decltype(ref.flip())>::value)
            return false;
          if (ref.flip() != (N % 2 != 0)) return false;
          break;
      }
    }
    return true;
  }
};

struct check_type_reference {
  bool operator()(sycl::ext::oneapi::sub_group_mask &sub_group_mask) {
    return std::is_same<sycl::ext::oneapi::sub_group_mask::reference,
                        decltype(sub_group_mask[sycl::id()])>::value;
  }
};
#endif  // SYCL_EXT_ONEAPI_SUB_GROUP_MASK

/** test sycl::oneapi::sub_group_mask interface
 */
class TEST_NAME : public util::test_base {
 public:
  /** return information about this test
   */
  void get_info(test_base::info &out) const override {
    set_test_info(out, TOSTRING(TEST_NAME), TEST_FILE);
  }

  /** execute the test
   */
  void run(util::logger &log) override {
#ifdef SYCL_EXT_ONEAPI_SUB_GROUP_MASK
    check_non_const_api<check_result_reference, check_type_reference,
                        even_predicate>(log);
#else
    log.note("SYCL_EXT_ONEAPI_SUB_GROUP_MASK is not defined, test is skipped");
#endif  // SYCL_EXT_ONEAPI_SUB_GROUP_MASK
  }
};

// register this test with the test_collection.
util::test_proxy<TEST_NAME> proxy;

} /* namespace TEST_NAMESPACE */
