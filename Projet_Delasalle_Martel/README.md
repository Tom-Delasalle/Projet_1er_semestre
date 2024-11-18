# Projet 2024 Delasalle-Martel

Ici nous allons lister les comportements des diff�rents choses pr�sentes dans notre programme 

<b> - Les feux tricolores : </b>

    * Il y en aura 4, un � droite de chaque route pr�s du carrefour.
    * Ils alterneront entre vert, orange et rouge. 

<b> - Les voitures : </b>
    
    * Elles arriveronts de l'extr�mit� d'une route dans la zone r�serv�e aux voitures
    � droite.
    * Elles appara�teront avec l'id�e de tourner � gauche, � droite ou de continuer 
    tout droit au carrefour
    * Elles devront ralentir et s'arr�ter s'il y a un obstacle devant elles (par exemple
    une autre voiture) ou si le feu est rouge ou orange. Si l'obstacle est trop proche
    de la voiture pour lui laisser le temps de ralentir, elle pourra s'arr�ter imm�diatement. 

<b> - Les bus : </b>
    
    * Ils arriveronts de l'extr�mit� d'une route dans la zone r�serv�e aux bus
    � droite.
    * Ils appara�teront avec l'id�e de continuer tout droit car il n'y a qu'une seule route
    avec une voie r�serv�e aux bus.
    * Ils devront ralentir et s'arr�ter s'il y a un obstacle devant eux (par exemple
    une autre voiture) ou si le feu est rouge ou orange. Si l'obstacle est trop proche
    de la voiture pour lui laisser le temps de ralentir, il pourra s'arr�ter imm�diatement.

<b> - Les cyclistes : </b>
    
    * Ils arriveronts de l'extr�mit� d'une route dans la zone r�serv�e aux cyclistes
    � droite.
    * Ils appara�teront avec l'id�e de continuer tout droit car il n'y a qu'une seule route
    avec une voie r�serv�e aux cyclistes.
    * Ils devront ralentir et s'arr�ter s'il y a un obstacle devant eux (par exemple
    une autre voiture) ou si le feu est rouge ou orange. Si l'obstacle est trop proche
    de la voiture pour lui laisser le temps de ralentir, il pourra s'arr�ter imm�diatement.

<b> - Les pi�tons : </b>
    
    * Ils arriveronts de l'extr�mit� d'un trottoir.
    * Ils appara�teront avec l'id�e de traverser la route et continuer tout 
    droit (- pour l'instant -).
    * Ils devront s'arr�ter (pas besoin de ralentir) quand ils arrivent devant le passage
    pi�ton si le feu est vert. Une fois le feu rouge, ils pourront traverser et continueront
    de traverser m�me si le feu passe au orange puis au vert