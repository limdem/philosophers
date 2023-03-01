/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgauvin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 02:32:27 by dgauvin           #+#    #+#             */
/*   Updated: 2022/07/05 02:32:45 by dgauvin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int	nbrlen(int n)
{
	long	nbr_len;
	long	nb;

	nbr_len = 1;
	nb = (long)n;
	if (nb < 0)
	{
		nbr_len++;
		nb = -nb;
	}	
	while (nb / 10 >= 1)
	{	
		nbr_len++;
		nb /= 10;
	}
	return (nbr_len);
}	

static void	fillstr(char *str, int n, int index)
{
	long	nb;

	nb = (long)n;
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	if (nb > 9)
		fillstr(str, nb / 10, index - 1);
	str[index] = nb % 10 + 48;
}	

char	*ft_itoa(unsigned long long n)
{
	char	*str;

	str = malloc(nbrlen(n) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	fillstr(str, n, nbrlen(n) - 1);
	str[nbrlen(n)] = '\0';
	return (str);
}	

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	str = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	result;
	int	i;

	sign = 1;
	result = 0;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - 48;
		i++;
	}
	return (sign * result);
}	
