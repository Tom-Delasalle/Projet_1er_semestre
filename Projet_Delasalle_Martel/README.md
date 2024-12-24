# Projet 2024 Delasalle-Martel

Ici vous trouverez nos fichiers utilisés pour ce projet 2024 qui consiste à simuler le carrefour Vauban, avec ses voitures, ses bus, ses cyclistes et ses piétons.
Il a été réalisé sur Visual Studio Enterprise avec le programme CMake pour compiler et SFML pour la représentation graphique

Vous trouverez notre code finalisé du projet dans le dossier "fichier/cpp". Le code des dossiers "fichier/cp" et "fichier/cppfiles" étaient utilisés pour réaliser des tests.

Vous trouverez aussi la vidéo capsule présentant notre projet dans "fichier"

Si jamais vous rencontrez des difficultés pour compiler notre projet, vous pourrez trouver notre exécutable pour lancer la version finale du programme dans le dossier "fichier/exe1/exe2/exe3/exe4"

CAHIERS DES CHARGES :

	* Implémentés :
		- Piétons, cyclistes, véhicules légers et bus sont présents
		- Feux de circulation
		- Chaque usager à un comportement autonome, circule sur la voie qui lui est dédiée et est modélisé par un thread
		- Flux entrant est aléatoire et ne se fait que s'il y a un espace disponible
		- Flux sortant se fait sans encombre
		- Les usagers s'arrêtent aux feux de circulation
		- Les usagers se suivent sur leurs voies dédiées avec une distance de sécurité
		- Utilisation du programme graphique SFML pour représenter graphiquement les usagers, les feux et leurs états

	* Non implémentés :
		- Accélérations et décélération des voitures et bus | Raison : Trop complexe à mettre en place et manque de temps

CARACTERISTIQUES DU PROJET :

    - Voitures peuvent apparaître en haut, en bas, à gauche ou à droite de l'écran et peuvent tourner à gauche, à droite ou aller tout droit
    - Bus peuvent apparaître à gauche ou à droite et vont tout droit
    - Cyclistes peuvent apparaître en haut ou en bas et vont tout droit
    - Pour simplifier la modélisation, les piétons vont toujours tout droit, donc ils ne changent pas de trottoir et ne peuvent pas passer par plusieurs passages piétons à la fois.
    - Pour simplifier la modélisation, la forme du carrefour Vauban de ce projet n'est pas fidèle à la réalité
	- Dû à la façon dont les collisions sont implémentés, il existait des cas où deux voitures se bloquaient l'une l'autre, bloquant ainsi le reste de la circulation, il y a eu donc 
    des modifications pour que cela n'arrive plus, mais visuellement, il peut arriver que des voitures rentrent un peu dans une autre.
	- Les feux de circulation et leur fonctionnement ont été repris du programme partagé par Mr. Mosbah