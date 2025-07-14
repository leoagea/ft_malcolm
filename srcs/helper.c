/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lagea < lagea@student.s19.be >             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 11:09:59 by lagea             #+#    #+#             */
/*   Updated: 2025/07/14 11:35:51 by lagea            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_malcolm.h"

static int	checkerror(char *str)
{
	int	i = 0;
	int	j = 0;

	if (str[0] == '\0' || str[1] == '\0')
		return (0);
	while (str[i] != '\0'){
		if (str[i] <= 32 || str[i] == 127 || str[i] == 43 || str[i] == 45)
			return (0);
		j = i + 1;
		while (str[j] != '\0'){
			if (str[i] == str[j])
				return (0);
			j++;
		}
		i++;
	}
	
	return (i);
}

static int	nb_base(char str, char *base)
{
	int	nb = 0;

	while (base[nb] != '\0'){
		if (str == base[nb])
			return (nb);
		nb++;
	}

	return (-1);
}

static int	whitespaces(char *str, int *ptr_i)
{
	int	count = 1;
	int	i = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	while (str[i] && (str[i] == 43 || str[i] == 45)){
		if (str[i] == 45)
			count *= -1;
		i++;
	}
	*ptr_i = i;

	return (count);
}

int	ft_atoi_base(char *str, char *base)
{
	int	i = 0;
	int	negative = 0;
	int	nb = 0;
	int	nb2 = 0;
	int	begin_len = checkerror(base);

	if (begin_len >= 2){
		negative = whitespaces(str, &i);
		nb2 = nb_base(str[i], base);
		while (nb2 != -1){
			nb = (nb * begin_len) + nb2;
			i++;
			nb2 = nb_base(str[i], base);
		}	
		return (nb *= negative);
	}
	
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		sign = 1;
	long	res = 0;
	long	overflow = 0;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+'){
		if (*str == '-')
			sign = -1;
		str++;
	}
	
	while (ft_isdigit(*str)){
		res *= 10;
		res += (*str - 48);
		str++;
		if (overflow > res && sign == 1)
			return (-1);
		else if (overflow > res && sign == -1)
			return (0);
		overflow = res;
	}

	return ((int)res * sign);
}

int	ft_isxdigit(int c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

void	ft_bzero(void *str, size_t n)
{
	size_t	i;

	i = -1;
	while (++i < n)
		*(unsigned char *)(str + i) = 0;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = -1;
	if (!dst && !src)
		return (NULL);
	if (dst != src)
		while (++i < n)
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
	return (dst);
}

void	*ft_memset(void *str, int c, size_t n)
{
	size_t	i = -1;
	char	*cpy = str;

	while (++i < n)
		cpy[i] = c;
	return (str);
}

size_t	ft_strlen(const char *s)
{
	int	len = 0;

	if (!s)
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i = 0;

	if (dstsize != 0){
		while (i < dstsize - 1 && src[i]){
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}

	return (ft_strlen(src));
}

char	*ft_strdup(const char *s1)
{
	char	*cpy;
	size_t	len = ft_strlen(s1);

	cpy = (char *)malloc((len + 1) * sizeof(char));
	if (!cpy)
		return (NULL);
	ft_memcpy(cpy, s1, len);
	cpy[len] = '\0';
	return (cpy);
}

char	*ft_strchr(const char *str, int c)
{
	while ((char)c != *str){
		if (!*str)
			return (NULL);
		str++;
	}
	return ((char *)str);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;
	size_t	test;

	test = count * size;
	if (size != 0 && (test / size) != count)
		return (NULL);
	res = (void *)malloc(count * size);
	if (!res)
		return (NULL);
	ft_bzero(res, count * size);
	return (res);
}
