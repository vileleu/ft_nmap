

GLOSSAIRE : 
- nmap (quoi) : Nmap = Network Mapper = logiciel de cartographie de réseaux qui utilise les paquets IP pour identifier tous les périphériques connectés à ce réseau
- nmap (pourquoi) : Nmap permet d'analyser une gamme d'adresses IP de manière séquentielle et d'afficher les périphériques actifs dans ce bloc d'adresses. 
source : https://www.jedha.co/formation-cybersecurite/comment-fonctionne-nmap-guide-complet


ETAPES DE NMAP :
- (parsing) Énumération des cibles : identifie les adresses IP à scanner à partir de la cible spécifiée (IP, plage IP, nom de domaine, etc.).
----
- Détecte si chaque hôte est actif ou non : Utilise divers types de paquets (ICMP, TCP SYN, ACK, etc.) pour contourner les pare-feux.
-----
- Résolution DNS inverse 
-----
- Fonction principale de Nmap : Détecte les états des ports : ouvert, fermé, filtré, etc. Utilise différentes techniques de scan : TCP SYN (-sS)
source : https://www.vaadata.com/blog/fr/nmap-loutil-pour-cartographier-et-evaluer-la-securite-dun-reseau/

SUJET :
- Scan à implementer :
1) SYN (-sS) : envoie un paquet SYN, attend SYN-ACK pour détecter un port ouvert
2) NULL Scan (-sN) : Envoie un paquet sans aucun drapeau 
3) ACK Scan	(-sA) : Envoie un paquet avec uniquement le drapeau ACK. Ne détermine pas si le port est ouvert, mais permet de détecter la présence d’un pare-feu
4) FIN Scan	(-sF) : Envoie un paquet avec le drapeau FIN. Normalement, un port fermé renvoie un RST.
5) XMAS Scan (-sX) : Envoie un paquet avec les drapeaux FIN, URG, et PSH (comme un "sapin de Noël allumé")
6) UDP (-sU) : Envoie un paquet UDP vide. Si "Port unreachable" ICMP est reçu → fermé

- Arguments à gérer :
1) --help
2) --ip
3) --file	Fichier avec une liste d’IP/FQDN, format du fichier : Une IP par ligne.
4) --ports	Plage ou liste de ports, permet de scanner un ou plusieurs ports spécifiques. Un seul port : 80, Une plage : 1-1000, Liste séparée par des virgules : 22,80,443
5) --speedup	Nombre de threads (0 à 250), plus le nombre est élevé, plus le scan est rapide
6) --scan	Type(s) de scan à effectuer : SYN, NULL, ACK, FIN, XMAS, UDP

TO DO LIST :
- continuer la documentation