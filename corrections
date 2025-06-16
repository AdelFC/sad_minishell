test avec ulimit:

(ulimit -v 3492; ./minishell) RAM
(ulimit -n X; ./minishell) File descriptors
(ulimit -u X; ./minishell) Process count


exit:
	minishell$ exit 1a2 a a a a
	exit: too many arguments
	-> Doit quitter avec "numeric argument required"

echo:
	echo -nnnn -nnn e
	 eminishell$
	-> Doit afficher 'e' sans espace avant

CTRL+C:
	double prompt quand minishell dans minishell
	-> Fix: Desactive l'handle du sigint dans le parent 
	
minishell$ echo $HOM"E"
/home/afodil-c
-> Faux, doit afficher le contenu de $HOM suivi de 'E'

minishell$ echo $HOM''
$HOM
-> Faux, doit afficher le contenu de $HOM

minishell$ echo $HOM'E'
$HOME
-> Faux, doit afficher le contenu de $HOM suivi de 'E'

minishell$ echo $HOM'E'
$HOME
-> Faux, doit afficher le contenu de $HOM suivi de 'E'



(ulimit -u 1; valgrind --track-fds=yes ./minishell)
minishell$ echo | cat
minishell: fork failed
-> Si cat fail, le shell exit sans faire de cleanup


