#pragma once

#include <functional>
#include <tuple>
#include <variant>

namespace fsm {

/**
 * @struct tuple_runtime_access_table
 * @brief Generates a lookup-table (LUT), during compile time, for each element in an std::tuple.
 * @details Generates a LUT that allows mapping a function of type ConverterFunction to each
 * element in the tuple.
 * @tparam TemplateTuple : type of the tupple.
 * @tparam ReturnType : return type.
 * @param ConverterFunction : function type (applied to each tuple element).
 * @tparam Idxs : indices to the tuple (parameter pack).
 */
template <typename TemplateTuple, typename ReturnType, typename ConverterFunction, std::size_t... Idxs>
struct tuple_runtime_access_table
{
	using tuple_type = TemplateTuple;
	using return_type = ReturnType;
	using converter_fun = ConverterFunction;
	const static auto table_size = std::tuple_size_v<tuple_type>;

	/**
	 * @brief Apply converter function f to the Ind'th element of the tuple, and return the result.
	 * @tparam Ind : Index
	 * @param[in] t : tuple.
	 * @param[in] f : converter function.
	 * @return Converted value.
	 */
	template <std::size_t Ind>
	static return_type accessTemplateTuplele(tuple_type& t, converter_fun& func)
	{
		return func(std::get<Ind>(t));
	}

	/**
	 * @brief Accessor function pointer type.
	 */
	using accessor_fun_ptr = return_type (*)(tuple_type&, converter_fun&);

	/**
	 * @brief std::array containing accessor functors for each tuple-element.
	 */
	constexpr static std::array<accessor_fun_ptr, table_size> lookup_table = {{&accessTemplateTuplele<Idxs>...}};
};

/**
 * @brief Convenience function for calling tuple_runtime_access_table in order to
 * automatically deduce templated types.
 * @details Automatic template parameter type deduction only works with functions.
 * @tparam ReturnType : return type.
 * @tparam TemplateTuple : tuple type.
 * @tparam ConverterFunction : converter function type.
 * @tparam Idxs : index parameter pack.
 * @return Converted value.
 */
template <typename ReturnType, typename TemplateTuple, typename ConverterFunction, std::size_t... Idxs>
auto call_access_function(TemplateTuple& t, std::size_t i, ConverterFunction f, std::index_sequence<Idxs...>)
{
	auto& table = tuple_runtime_access_table<TemplateTuple, ReturnType, ConverterFunction, Idxs...>::lookup_table;
	auto* access_function = table[i];
	return access_function(t, f);
}

/**
 * @brief Common tuple access.
 * @details Generic template.
 * @param Ts : parameter pack defining types in the std::tuple
 */
template <typename TemplateTuple>
struct commonTemplateTuplele_access;

/**
 * @brief Common tuple access, template specialization for std::tuple-type.
 * @details Defines the type as std::variant containing pointers to Ts types.
 * @tparam Ts : parameter pack defining types in the std::tuple
 */
template <typename... Ts>
struct commonTemplateTuplele_access<std::tuple<Ts...>>
{
	using type = std::variant<Ts*...>;
};

/**
 * @brief Common tuple access, template specialization for std::pair-type.
 * @details Defines the type as std::variant containing pointers to Ts types.
 * @tparam T1 : first pair type.
 * @tparam T2 : second pair type.
 */
template <typename T1, typename T2>
struct commonTemplateTuplele_access<std::pair<T1, T2>>
{
	using type = std::variant<T1*, T2*>;
};

/**
 * @brief Common tuple access, template specialization for std::pair-type.
 * @details Defines the type as std::variant containing pointers to Ts types.
 * @tparam T : element type.
 * @tparam N : number of elements in the std::array.
 */
template <typename T, auto N>
struct commonTemplateTuplele_access<std::array<T, N>>
{
	using type = std::variant<T*>;
};

template <typename TemplateTuple>
using commonTemplateTuplele_access_t = typename commonTemplateTuplele_access<TemplateTuple>::type;

/**
 * @brief Get access to a element in a std::pair in runtime.
 * @tparam TemplateTuple : type of the std::tuple.
 * @param[in] t : tuple.
 * @param[in] i : index of the element that is retrieved.
 * @return std::variant with pointer to the tuple.
 */
template <typename TemplateTuple>
auto runtime_get(TemplateTuple& t, std::size_t i)
{
	return call_access_function<commonTemplateTuplele_access_t<TemplateTuple>>(t, i, [](auto& element) { return &element; },
	                                                         std::make_index_sequence<std::tuple_size_v<TemplateTuple>>{});
}

}  // End of namespace fsm
