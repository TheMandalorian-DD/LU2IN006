# **Problèmes rencontrés et leurs résolutions**

Après avoir tester la fonction ```functionClef``` permettant de récupérer la clef de hachage d'un point qui été calculé à partir de ses coordonnées, pour de très grandes valeurs de coordonnées, la clef retournait parfois une valeur négative car selon le type (int,double,long...), chaque type a sa propre garantie de données, et donc sa propre plage de valeurs.

On décide donc de prendre le type primitif ```long long``` pour laquelle il y a une garantie de données de 64 bits.

Après avoir fixer ce problème, on remarque finalement que la reconstitution du réseau par table de hachage est plus performante que l'arbre quaternaire, ce qui est théoriquement exact car les recherches et insertions sont en O(1), tandis que les arbres sont en O(log n).

# **Modifications codes / SDD**

## **Table de Hachage**

Pour la SDD de table de hachage, nous avons décider de créer une sous-structure ```NodeHachage``` pour avoir une composition uniforme. 

La fonction ```functionClef``` et ```functionHachage``` sont désormais des macros. La clef du point est maintenant récupérée dans une variable de type ```long long clef``` dans ```rechercheCreeNoeudHachage```.

## **Arbre quaternaire**

Pour la recherche des coordonnées minimales et maximales dans les arbres quaternaire, nous utilisons deux macros ```max``` et ```min```.

## **Graphe**

Nous nous sommes permis de modifier la SDD du Graphe, dans la structure ```cellule_arete```, finalement elle pointe vers deux ```cellule_arete``` : ```usuiv``` et ```vsuiv```, pour avoir un tri plus précis et rapide.

