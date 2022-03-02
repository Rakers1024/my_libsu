"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_override_control)
"1"
#else
"0"
#endif
"cxx_override\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_range_for)
"1"
#else
"0"
#endif
"cxx_range_for\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_raw_string_literals)
"1"
#else
"0"
#endif
"cxx_raw_string_literals\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_reference_qualified_functions)
"1"
#else
"0"
#endif
"cxx_reference_qualified_functions\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_relaxed_constexpr)
"1"
#else
"0"
#endif
"cxx_relaxed_constexpr\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_return_type_deduction)
"1"
#else
"0"
#endif
"cxx_return_type_deduction\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __cplusplus >= 201103L
"1"
#else
"0"
#endif
"cxx_right_angle_brackets\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_rvalue_references)
"1"
#else
"0"
#endif
"cxx_rvalue_references\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __cplusplus >= 201103L
"1"
#else
"0"
#endif
"cxx_sizeof_member\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_static_assert)
"1"
#else
"0"
#endif
"cxx_static_assert\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_strong_enums)
"1"
#else
"0"
#endif
"cxx_strong_enums\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __cplusplus >= 199711L
"1"
#else
"0"
#endif
"cxx_template_template_parameters\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_thread_local)
"1"
#else
"0"
#endif
"cxx_thread_local\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_trailing_return)
"1"
#else
"0"
#endif
"cxx_trailing_return_types\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_unicode_literals)
"1"
#else
"0"
#endif
"cxx_unicode_literals\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_generalized_initializers)
"1"
#else
"0"
#endif
"cxx_uniform_initialization\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_unrestricted_unions)
"1"
#else
"0"
#endif
"cxx_unrestricted_unions\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_user_literals)
"1"
#else
"0"
#endif
"cxx_user_literals\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_variable_templates)
"1"
#else
"0"
#endif
"cxx_variable_templates\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __cplusplus >= 201103L
"1"
#else
"0"
#endif
"cxx_variadic_macros\n"
"CXX_FEATURE:"
#if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_variadic_templates)
"1"
#else
"0"
#endif
"cxx_variadic_templates\n"

};

int main(int argc, char** argv) { (void)argv; return features[argc]; }
