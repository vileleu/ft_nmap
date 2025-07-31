#include "ft_nmap.h"

void init_scan_configuration(t_opt *opt)
{
    // 1. Vérifier si des ports ont été spécifiés
    // Si aucun port n'est fourni (ni plage, ni liste)
    //     → Initialiser la plage par défaut : min = 1, max = 1024

    // 2. Calculer le nombre total de ports à scanner
    // Si une plage a été spécifiée
    //     → nb_ports = max - min + 1
    // Sinon si une liste de ports a été fournie
    //     → nb_ports = compter les éléments de la liste chaînée

    // 3. Vérifier que le nombre total de ports ne dépasse pas 1024
    // Si nb_ports > 1024
    //     → Afficher une erreur et quitter le programme

    // 4. Vérifier les types de scans demandés
    // Si aucun des opt->scan[i] n'est activé (tous à 0)
    //     → Activer tous les types : SYN, NULL, ACK, FIN, XMAS, UDP

    // 5. Vérifier le nombre de threads
    // Si opt->thread == 0
    //     → Définir une valeur par défaut (ex : 10)
    // Sinon si opt->thread > 250
    //     → Afficher une erreur et quitter

    // 6. Préparer la répartition des ports entre les threads
    // → ports_par_thread = nb_ports / opt->thread
    // → gérer le reste : nb_ports % opt->thread
    //     (ex : ajouter 1 port de plus à certains threads pour équilibrer)

    // 7. Initialiser des structures pour chaque thread
    // → Pour chaque thread :
    //     - Plage de ports à scanner
    //     - Adresse IP cible
    //     - Types de scans à effectuer
    //     - (Facultatif : résultat du scan, chrono, etc.)
}
