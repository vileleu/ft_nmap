

GLOSSAIRE : 
- nmap (quoi) : Nmap = Network Mapper = logiciel de cartographie de réseaux qui utilise les paquets IP pour identifier tous les périphériques connectés à ce réseau
- nmap (pourquoi) : Nmap permet d'analyser une gamme d'adresses IP de manière séquentielle et d'afficher les périphériques actifs dans ce bloc d'adresses. 
source : https://www.jedha.co/formation-cybersecurite/comment-fonctionne-nmap-guide-complet


ETAPES DE NMAP :
- (parsing) Énumération des cibles : identifie les adresses IP à scanner à partir de la cible spécifiée (IP, plage IP, nom de domaine, etc.).
----
- Resolve target
- Résolution DNS inverse (facultatif, bonus)
----
- Détecte si chaque hôte est actif ou non
-----
- Fonction principale de Nmap : Détecte les états des ports : ouvert, fermé, filtré, etc. Utilise différentes techniques de scan
source : https://www.vaadata.com/blog/fr/nmap-loutil-pour-cartographier-et-evaluer-la-securite-dun-reseau/


SUJET :
- Scan à implementer :
Chacun sert à scanner des ports sur une machine déjà détectée comme active.
1) SYN (-sS) : envoie un paquet SYN, attend SYN-ACK pour détecter un port ouvert
2) NULL Scan (-sN) : Envoie un paquet sans aucun drapeau 
3) ACK Scan	(-sA) : Envoie un paquet avec uniquement le drapeau ACK. Ne détermine pas si le port est ouvert, mais permet de détecter la présence d’un pare-feu
4) FIN Scan	(-sF) : Envoie un paquet avec le drapeau FIN. Normalement, un port fermé renvoie un RST.
5) XMAS Scan (-sX) : Envoie un paquet avec les drapeaux FIN, URG, et PSH (comme un "sapin de Noël allumé")
6) UDP (-sU) : Envoie un paquet UDP vide. Si "Port unreachable" ICMP est reçu → fermé
source : https://nmap.org/book/man-port-scanning-techniques.html

- Arguments à gérer :
1) --help
2) --ip
3) --file	Fichier avec une liste d’IP/FQDN, format du fichier : Une IP par ligne.
4) --ports	Plage ou liste de ports, permet de scanner un ou plusieurs ports spécifiques. Un seul port : 80, Une plage : 1-1000, Liste séparée par des virgules : 22,80,443
5) --speedup	Nombre de threads (0 à 250), plus le nombre est élevé, plus le scan est rapide
6) --scan	Type(s) de scan à effectuer : SYN, NULL, ACK, FIN, XMAS, UDP

VISUEL ARGUMENTS : 
Args >    [ "192.168.1.1", "example.com" ]
            ↓ (parsing)
opt->targets = char **
            ↓ (resolve_targets)
t_target[]  = IP + sockaddr_in
            ↓ (check_host_availability)
             ping / SYN / ACK test
            ↓
         Port scans


TO DO LIST :
- ✅ revoir la logique dans get_otp
- ✅ revoir la fonction resolve (/!\ ne pas oublier de l'adapter apres pour le futur --file)
- ✅ coder check_host_availability
- ✅ gerer les hostnames -> resolve_target
- ✅ decouper check_host_availability -> trop longue
- ✅ enlever resolve_target - pull
- ✅ recuperer l'argument parsing
- ✅ lire la struct opt
- ✅ faire la fonction d'initialisation
- lire de la doc sur les threads
- ✅ faire la derniere etape d'init_scan_configuration

DERNIERES MODIFICATIONS : 
-  /!\ bibliotheque sur macOS, travailler sur la VM, ne pas oublier de faire les modifs pour Linux /!\
- #define MAX_PORT 1024 car "• The ports to be scanned can be read as a range or individually. In the case no port
is specified the scan must run with the range 1-1024.
• The number of ports scanned cannot exceed 1024."


DOC fonction - check_host_availability :
- Fonction pour détecter si chaque hôte est actif/joignable ou non : ici j'utilise le ping classique ICMP
- methode : ouvre un raw socket en mode ICMP, construis un paquet ICMP Echo Request, envoies à l’IP cible, attends une réponse (ICMP Echo Reply), i reçue dans la limite timeout → hôte actif, sinon → pas actif

DOC fonction - init_scan_configuration :
- Pourquoi répartir les ports entre les threads : Quand on lance un scan de ports, l'idée est de gagner du temps en lançant plusieurs tâches en parallèle. Chaque tâche est gérée par un thread, qui va s’occuper de scanner un ou plusieurs ports. Mais pour que ce soit efficace, il faut savoir quels ports chaque thread va scanner. La répartition sert donc à : Scanner tous les ports sans en oublier, Équilibrer le travail entre les threads, Maximiser la vitesse d’exécution, Éviter la surcharge de certains threads.
   a) nb port / nb thread, exemple : 20 ports / 2 threads = 10 ports pour 2 threads
- Pourquoi allouer une structure t_thread_data : t_opt contient la config globale avec tous les ports et toutes les cibles et t_thread_data contient la partie du travail assignée à un thread précis (par exemple : ports 1-10 pour le thread 1, ports 11-20 pour le thread 2, etc.)


EXEMPLES COMMANDES
sudo ./ft_nmap --ports 20-30 --ip 1.1.1.1 --speedup 50
sudo ./ft_nmap --ip 192.168.1.1
sudo ./ft_nmap --ip 192.168.1.1 --ports 22,80,443
sudo ./ft_nmap --ip 192.168.1.1 --ports 20-25
sudo ./ft_nmap --ip 192.168.1.1 --scan SYN,XMAS
sudo ./ft_nmap --ip 192.168.1.1 --speedup 50
sudo ./ft_nmap --file targets.txt
sudo ./ft_nmap --help
