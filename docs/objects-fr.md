## Patchs d'aide

Une liste de patchs d'aide décrivant le fonctionnement des objets est <a href="https://github.com/Musicoll/Kiwi/releases/download/v1.0.3/documentation.zip" target='_blank_'>ici</a>.
Parfois les patchs d'aide regroupent plusieurs objets liés fonctionnellement, par exemple gate et switch ou gate~ et switch~.

## List of objects

| Object          | Alias |                       Description                       |
|-----------------|:-----:|---------------------------------------------------------|
| **line~**       |       | Génère un signal de rampe audio                         |
| **noise~**      |       | Générateur de bruit blanc                               |
| **sah~**        |       | "Sample and hold" sur un son en entrée                  |
| **hub**         |       | Envoie un message aux autres utilisateurs connectés     |
| **send**        |  *s*  | Envoie un message à l'objet receive correspondant       |
| **mtof**        |       | Convertit une note Midi en fréquence                    |
| **number~**     |       | Affiche les valeurs des samples audio                   |
| **number**      |  *f*  | Affiche un nombre                                       |
| **select**      |       | Produit un bang lorsque l'entrée est égale à la valeur  |
| **snapshot~**   |       | Capture les valeurs des samples                         |
| **random**      |       | Produit un nombre aléatoire (distribution uniforme)     |
| **unpack**      |       | Sépare les éléments d'une liste                         |
| **pack**        |       | Crée et produit une liste                               |
| **comment**     |  *c*  | Ajoute un commentaire au patch                          |
| **!=~**         |       | Opérateur signal d'inégalité                            |
| **==~**         |       | Opérateur signal d'égalité                              |
| **>=~**         |       | Opérateur signal supérieur ou égal                      |
| **>~**          |       | Opérateur signal supérieur strict                       |
| **<=~**         |       | Opérateur signal inférieur ou égal                      |
| **<~**          |       | Opérateur signal inférieur strict                       |
| **/~**          |       | Divise deux signaux                                     |
| **-~**          |       | Soustrait deux signaux                                  |
| **%**           |       | Calcule le modulo d'un nombre dans la base désirée      |
| **pow**         |       | Calcule la puissance d'un nombre                        |
| **==**          |       | Opérateur d'égalité                                     |
| **!=**          |       | Opérateur d'inégalité                                   |
| **>=**          |       | Opérateur supérieur ou égal                             |
| **>**           |       | Opérateur supérieur strict                              |
| **<=**          |       | Opérateur inférieur ou égal                             |
| **<**           |       | Opérateur inférieur strict                              |
| **/**           |       | Calcule la division de deux nombres                     |
| **-**           |       | Calcule la différence de deux nombres                   |
| **message**     |       | Produit des messages ou des listes de messages          |
| **slider**      |       | Potentiomètre graphique sortant des valeurs entre 0 et 1|
| **toggle**      |       | Bouton on/off (1/0)                                     |
| **bang**        |       | Produit un bang                                         |
| **meter~**      |       | Vu-mètre (pic d'amplitude)                              |
| **trigger**     |  *t*  | Déclenche des événements à partir des données reçues    |
| **newbox**      |       | Crée une boîte vide                                     |
| **errorbox**    |       | Boîte erreur                                            |
| **+**           |       | Ajoute deux nombres                                     |
| **\***          |       | Multiplie deux nombres                                  |
| **print**       |       | Ecrit des messages dans la console Kiwi                 |
| **receive**     | *r*   | Reçoit des messages d'un objet send du même nom         |
| **loadmess**    |       | Sort ses arguments comme message au chargement du patch |
| **delay**       |       | Retarde un bang reçu en entrée                          |
| **pipe**        |       | Retarde n'importe quel message                          |
| **metro**       |       | Produit un bang à intervalle régulier                   |
| **adc~**        |       | Reçoit les signaux audio des entrées son                |
| **dac~**        |       | Envoie des signaux audio vers les sorties son           |
| **osc~**        |       | Oscillateur cosinus                                     |
| **phasor~**     |       | Génère un signal en dents de scie                       |
| **\*~**         |       | Multiplie deux signaux                                  |
| **+~**          |       | Ajoute deux signaux                                     |
| **sig~**        |       | Convertit des nombres en signaux audio                  |
| **delaysimple~**|       | Délai variable simple avec feedback optionnel           |
| **scale**       |       | Changement d'échelle d'un nombre vers une autre échelle |
| **gate**        |       | Route un message vers la sortie sélectionnée            |
| **switch**      |       | Sort le message reçu sur l'entrée sélectionnée          |
| **gate~**       |       | Route le signal reçu vers la sortie sélectionnée        |
| **switch~**     |       | Sort le signal reçu sur l'entrée sélectionnée           |
| **clip**        |       | Ecrête un nombre entre une valeur minimum et maximum    |
| **clip~**       |       | Ecrête un signal entre une valeur minimum et maximum    |
| **float**       |       | Stocke un nombre flottant                               |
