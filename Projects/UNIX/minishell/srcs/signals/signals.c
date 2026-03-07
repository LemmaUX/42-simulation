/* ************************************************************************** */
/*                                                                            */
/*   signals.c - Manejo de senales                                            */
/*                                                                            */
/*   El comportamiento de las senales cambia segun el contexto:               */
/*                                                                            */
/*   EN EL PROMPT (esperando input):                                          */
/*     SIGINT  (ctrl-C) -> nueva linea, mostrar nuevo prompt                  */
/*     SIGQUIT (ctrl-\) -> ignorar                                            */
/*                                                                            */
/*   DURANTE EJECUCION (proceso hijo corriendo):                              */
/*     SIGINT  (ctrl-C) -> el padre ignora, el hijo muere                     */
/*     SIGQUIT (ctrl-\) -> el padre ignora, el hijo muere                     */
/*                                                                            */
/*   EN EL HIJO:                                                              */
/*     Comportamiento por defecto (SIG_DFL) para ambas senales.               */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** sigint_handler_prompt: Cuando ctrl-C se presiona en el prompt,
** imprime una nueva linea y vuelve a mostrar el prompt.
*/
static void	sigint_handler_prompt(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, PROMPT, ft_strlen(PROMPT));
}

/*
** setup_signals_prompt: Configura senales para cuando estamos
** en el prompt esperando input del usuario.
*/
void	setup_signals_prompt(void)
{
	signal(SIGINT, sigint_handler_prompt);
	signal(SIGQUIT, SIG_IGN);
}

/*
** setup_signals_exec: Configura senales para cuando un comando
** se esta ejecutando. El padre ignora ambas senales y deja que
** el hijo las maneje.
*/
void	setup_signals_exec(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*
** setup_signals_child: En el proceso hijo, restaurar el
** comportamiento por defecto para todas las senales.
*/
void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
