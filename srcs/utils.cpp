/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnelson <lnelson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 15:25:55 by lnelson           #+#    #+#             */
/*   Updated: 2022/07/11 17:19:06 by aedouard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	serverLogMssg(std::string to_print)
{
	*logStream << "(SERVER): " << to_print << std::endl;
}

std::ostream clientLogMssg(std::string to_print)
{
	*logStream << "(CLIENT): " << to_print << std::endl;
}
