/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfordoxc <nfordoxc@42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:47:11 by nfordoxc          #+#    #+#             */
/*   Updated: 2024/05/15 16:14:13 by nfordoxc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

/*
 * <cat>signal</cat>
 *
 * <summary>
 * 	void	ft_runtime(int signal)
 * </summary>
 *
 * <description>
 * 	ft_runtime is a runtime about SIGUSR1 and SIGUSR2.
 * 	SIGUSR1 is using to acknowlege the reception by the server.
 * 	SIGUSR2 is using to close the connection between the server and the client. 
 * </description>
 *
 * <param type="int" name="signal">id number of the signal</param>
 *
 * <return>
 * 	void.
 * </return>
 *
 */

static void	ft_runtime(int signal)
{
	static int	received = 0;

	if (signal == SIGUSR1)
		++received;
	else
	{
		ft_putnbr_fd(received, 1);
		ft_putchar_fd('\n', 1);
		exit(0);
	}
}

/*
 * <cat>signal</cat>
 *
 * <summary>
 * 	oid	ft_kill(int pid, char *str)
 * </summary>
 *
 * <description>
 * 	ft_kill send the second argument to the server using SIGUSR1 and SIGUSR2.
 * 	It sends all char, bit by bit. SIGUSR1 if bit  == 0, SIGUSR2 if bit == 1.
 * 	To close the connection it sent 8 bit to 0 by SIGUSR1.
 * </description>
 *
 * <param type="int" name="pid">pid of the server</param>
 * <param type="char *" name="str">string to send to the server</param>
 *
 * <return>
 * 	void.
 * </return>
 *
 */

static void	ft_kill(int pid, char *str)
{
	int		i;
	char	c;

	while (*str)
	{
		i = 8;
		c = *str++;
		while (i--)
		{
			if (c >> i & 1)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
			usleep(400);
		}
	}
	i = 8;
	while (i--)
	{
		kill(pid, SIGUSR1);
		usleep(400);
	}
}

/*
 * <cat>program</cat>
 *
 * <summary>
 *	client
 * </summary>
 *
 * <description>
 *	client is a basic program using SIGUSR1 and SIGUSR2 to sent a string to a 
 *	server. it need two arguments, the pid of the server and the string to send.
 *	It print how many octet is sending and how namy octet is receive by the 
 *	server.
 * </description>
 *
 * <param type="" name="">main args</param>
 *
 * <return>
 *	0 if no error or 1 if error.
 * </return>
 *
 */

int	main(int argc, char **argv)
{
	if (argc != 3 || !ft_strlen(argv[2]))
		return (1);
	ft_putstr_fd("Sent    : ", 1);
	ft_putnbr_fd(ft_strlen(argv[2]), 1);
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("Received: ", 1);
	signal(SIGUSR1, ft_runtime);
	signal(SIGUSR2, ft_runtime);
	ft_kill(ft_atoi(argv[1]), argv[2]);
	while (1)
		pause();
	return (0);
}
