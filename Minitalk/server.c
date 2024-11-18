/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfordoxc <nfordoxc@42luxembourg.lu>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:47:13 by nfordoxc          #+#    #+#             */
/*   Updated: 2024/05/15 16:14:12 by nfordoxc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "libft/libft.h"

/*
 * <cat>signal</cat>
 *
 * <summary>
 * 	void	ft_runtime(int signal, siginfo_t *info, void *context)
 * </summary>
 *
 * <description>
 * 	ft_runtime is a runtime of the server who receive a SIGUSR1 and SIGUSR2. It 
 * 	catch the twice signal and if it's SIGUSR2 It set the bit at 1 and pass a 
 * 	the next bit.
 * 	For each char receive, server sent a signal on SIGUSR1 to inform the client 
 * 	to pass at the next char.
 * 	At the end to receive the 8 bits, it sent a signal on SIGUSR2 to inform the 
 * 	client, and print a newline.
 * </description>
 *
 * <param type="int" name="signal">id number of the signal</param>
 * <param type="siginfo *" name="info">structure info about the signal</param>
 * <param type="void *" name="context">function to execute by default</param>
 *
 * <return>
 * 	void.
 * </return>
 *
 */

static void	ft_runtime(int signal, siginfo_t *info, void *context)
{
	static int		i = 0;
	static char		c = 0;
	static pid_t	client_pid = 0;

	(void)context;
	if (!client_pid)
		client_pid = info->si_pid;
	c |= (signal == SIGUSR2);
	if (++i == 8)
	{
		i = 0;
		if (!c)
		{
			kill(client_pid, SIGUSR2);
			ft_putstr_fd("\n", 1);
			client_pid = 0;
			return ;
		}
		ft_putchar_fd(c, 1);
		c = 0;
		kill(client_pid, SIGUSR1);
	}
	else
		c <<= 1;
}

/*
 * <cat>program</cat>
 *
 * <summary>
 *	server
 * </summary>
 *
 * <description>
 *	server is a basic program using SIGUSR1 and SIGUSR2. The server is running 
 *	firstely without argument. It prints her pid and wait to receve a message on
 *	 SIGUSR1. For each octet receved, it send an acknowlege with SIGUSR2. At the
 *	 end of the message, he print it and wait for a new message to print.
 * </description>
 *
 * <param type="void" name="">main without args</param>
 *
 * <return>
 *	print her pid and all char receved on SIGUSR1.
 * </return>
 *
 */

int	main(void)
{
	struct sigaction	s_action;
	sigset_t			set;

	ft_putstr_fd("Server PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putchar_fd('\n', 1);
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	s_action.sa_sigaction = ft_runtime;
	s_action.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &s_action, NULL);
	sigaction(SIGUSR2, &s_action, NULL);
	while (1)
		pause();
	return (0);
}
