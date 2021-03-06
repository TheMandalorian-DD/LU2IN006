
#include "Reseau.h"
#include "SVGwriter.h"
#include <stdlib.h> 

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){

    for(CellNoeud* L_noeud = R -> noeuds; L_noeud; L_noeud = L_noeud -> suiv){
        Noeud* noeud = L_noeud -> nd;
        if (noeud -> x == x && noeud -> y == y) return noeud; 
    }
    CellNoeud* new_noeud = malloc(sizeof(CellNoeud));
    Noeud* n = malloc(sizeof(Noeud));
    n -> num = (R -> nbNoeuds) + 1;
    n -> x = x;
    n -> y = y;
    n -> voisins = NULL;
    new_noeud -> nd = n;
    new_noeud -> suiv = R -> noeuds;
    R -> noeuds = new_noeud;
    R -> nbNoeuds += 1;
    return n;
}

Reseau *reconstitueReseauListe(Chaines *C) {
  Reseau *R = malloc(sizeof(Reseau));
  R->nbNoeuds = 0;
  R->gamma = C->gamma;
  R->noeuds = NULL;
  R->commodites = NULL;
  Noeud *node, *node_pre,*node_first;
  CellChaine *list_chaine = C->chaines;
  while (list_chaine != NULL) {
    CellPoint *list_point = list_chaine->points;
    node_pre = NULL;
    node_first = rechercheCreeNoeudListe(R,list_point->x,list_point->y);
    while (list_point != NULL) {
      node = rechercheCreeNoeudListe(R, list_point->x, list_point->y);
      if (node_pre != NULL) {
        ajoute_voisins(node_pre, node);
      }
      node_pre = node;
      list_point = list_point->suiv;
    }
    ajoute_commodites(R, node_first, node);
    list_chaine = list_chaine->suiv;
  }
  return R;
}

void ajoute_voisins(Noeud *node1, Noeud *node2) {
  CellNoeud *list_node1 = malloc(sizeof(CellNoeud));
  CellNoeud *voisins = node1->voisins;
  while(voisins != NULL){
    if (voisins->nd == node2) return;
    voisins = voisins->suiv;
  }
  list_node1->nd = node2;
  list_node1->suiv = node1->voisins;
  CellNoeud *list_node2 = malloc(sizeof(CellNoeud));
  list_node2->nd = node1;
  list_node2->suiv = node2->voisins;
  node1->voisins = list_node1;
  node2->voisins = list_node2;
}

void ajoute_commodites(Reseau *R, Noeud *node1, Noeud *node2) {
  CellCommodite *commodite = malloc(sizeof(CellCommodite));
  commodite->extrA = node1;
  commodite->extrB = node2;
  commodite->suiv = R->commodites;
  R->commodites = commodite;
}

int nbLiaisons(Reseau *R){
    int nbL = 0;
    for(CellNoeud* cell_noeud = R -> noeuds; cell_noeud; cell_noeud = cell_noeud -> suiv){
        Noeud* n = cell_noeud -> nd;
        for(CellNoeud* v = n -> voisins; v; v = v -> suiv){
          if (v->nd->num < cell_noeud->nd->num) nbL++;
        }
    }
    return nbL;
}

int nbCommodites(Reseau *R){ 
    int nbCom = 0;
    for(CellCommodite* cell_com = R -> commodites; cell_com; cell_com = cell_com -> suiv) nbCom++;
    return nbCom;
}

void ecrireReseau(Reseau *R, FILE *f){
    fprintf(f,"nbNoeud = %d\n", R -> nbNoeuds);
    fprintf(f,"nbLiaison = %d\n",nbLiaisons(R));
    fprintf(f,"nbCom = %d\n",nbCommodites(R));
    fprintf(f,"Gamma = %d\n\n", R -> gamma);
    for(CellNoeud* cell_noeud = R -> noeuds; cell_noeud; cell_noeud = cell_noeud -> suiv) fprintf(f,"v %d %.2f %.2f\n",cell_noeud -> nd -> num, cell_noeud -> nd -> x, cell_noeud -> nd -> y);
    fprintf(f,"\n");
    for(CellNoeud* cell_noeud = R -> noeuds; cell_noeud; cell_noeud = cell_noeud -> suiv){
      for(CellNoeud* v = cell_noeud -> nd -> voisins; v; v = v -> suiv){
        if (v -> nd -> num < cell_noeud -> nd -> num) fprintf(f,"l %d %d\n", v -> nd -> num, cell_noeud -> nd -> num);
      }
    } 
    fprintf(f,"\n");
    for(CellCommodite* cell_com = R -> commodites; cell_com; cell_com = cell_com -> suiv) fprintf(f,"k %d %d\n", cell_com -> extrA -> num, cell_com -> extrB -> num);
}



void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv; 
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds; 
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

void liberer_noeud(CellNoeud* n){
  while(n){
    CellNoeud* tmp = n->suiv;
    CellNoeud* voisins = n->nd->voisins;
    while (voisins)
    {
      CellNoeud* next = voisins->suiv;
      free(voisins);
      voisins=next;
    }
    
    free(n->nd);
    free(n);
    n=tmp;
  }
}

void liberer_commodite(CellCommodite* c){
  while(c){
    CellCommodite* tmp = c->suiv;
    free(c);
    c=tmp;
  }
}

void liberer_reseau(Reseau* R){
  liberer_noeud(R->noeuds);
  liberer_commodite(R->commodites);
  free(R);
}

