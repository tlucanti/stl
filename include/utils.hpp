/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:29:33 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN

// ------------------------------------ min ------------------------------------
template <class type_T>
constexpr const type_T  &min(const type_T &a, const type_T &b)
{
    return (b < a) ? b : a;
}

// ------------------------------------ max ------------------------------------
template <class type_T>
constexpr const type_T  &max(const type_T &a, const type_T &b)
{
    return (a < b) ? b : a;
}

// ----------------------------------- sign ------------------------------------
template <class type_T>
constexpr int           sign(const type_T &val) {
    return (type_T() < val) - (val < type_T());
}

TLU_NAMESPACE_END

#endif /* UTILS_HPP */
