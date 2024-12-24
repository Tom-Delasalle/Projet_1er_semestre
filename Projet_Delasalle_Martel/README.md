# Projet 2024 Delasalle-Martel

Ici vous trouverez nos fichiers utilis�s pour ce projet 2024 qui consiste � simuler le carrefour Vauban, avec ses voitures, ses bus, ses cyclistes et ses pi�tons.
Il a �t� r�alis� sur Visual Studio Enterprise avec le programme CMake pour compiler et SFML pour la repr�sentation graphique

Vous trouverez notre code finalis� du projet dans le dossier "fichier/cpp". Le code des dossiers "fichier/cp" et "fichier/cppfiles" �taient utilis�s pour r�aliser des tests.

Vous trouverez aussi la vid�o capsule pr�sentant notre projet dans "fichier"

Si jamais vous rencontrez des difficult�s pour compiler notre projet, vous pourrez trouver notre ex�cutable pour lancer la version finale du programme dans le dossier "fichier/exe1/exe2/exe3/exe4"

CAHIERS DES CHARGES :

	* Impl�ment�s :
		- Pi�tons, cyclistes, v�hicules l�gers et bus sont pr�sents
		- Feux de circulation
		- Chaque usager � un comportement autonome, circule sur la voie qui lui est d�di�e et est mod�lis� par un thread
		- Flux entrant est al�atoire et ne se fait que s'il y a un espace disponible
		- Flux sortant se fait sans encombre
		- Les usagers s'arr�tent aux feux de circulation
		- Les usagers se suivent sur leurs voies d�di�es avec une distance de s�curit�
		- Utilisation du programme graphique SFML pour repr�senter graphiquement les usagers, les feux et leurs �tats

	* Non impl�ment�s :
		- Acc�l�rations et d�c�l�ration des voitures et bus | Raison : Trop complexe � mettre en place et manque de temps

CARACTERISTIQUES DU PROJET :

    - Voitures peuvent appara�tre en haut, en bas, � gauche ou � droite de l'�cran et peuvent tourner � gauche, � droite ou aller tout droit
    - Bus peuvent appara�tre � gauche ou � droite et vont tout droit
    - Cyclistes peuvent appara�tre en haut ou en bas et vont tout droit
    - Pour simplifier la mod�lisation, les pi�tons vont toujours tout droit, donc ils ne changent pas de trottoir et ne peuvent pas passer par plusieurs passages pi�tons � la fois.
    - Pour simplifier la mod�lisation, la forme du carrefour Vauban de ce projet n'est pas fid�le � la r�alit�
	- D� � la fa�on dont les collisions sont impl�ment�s, il existait des cas o� deux voitures se bloquaient l'une l'autre, bloquant ainsi le reste de la circulation, il y a eu donc 
    des modifications pour que cela n'arrive plus, mais visuellement, il peut arriver que des voitures rentrent un peu dans une autre.
	- Les feux de circulation et leur fonctionnement ont �t� repris du programme partag� par Mr. Mosbah