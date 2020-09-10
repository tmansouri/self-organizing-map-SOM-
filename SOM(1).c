#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Noeud Noeud;
typedef struct Noeud{
int indice;
char *c;
double *vect;
Noeud *Suiv;
}Noeud;

int get_taille(char *a){
FILE *f1 = fopen(a,"r");
 if(f1 == NULL){
        printf("Fichier introuvable !");
        exit(1);
   }
     int taille = 0;
     char ligne1[256];
     if (fgets(ligne1, 255, f1)!= NULL){
     char *st = strtok( ligne1,",") ;
     while( ( st = strtok( NULL, "," ) ) != NULL )
         taille += 1 ;
     }
return taille;
}

Noeud * InsererD(Noeud * L,int x, char *ligne, int taille)
{
   
   double * vec  = (double*) malloc(sizeof(double )*taille );
   Noeud * P=(Noeud *) malloc(sizeof(Noeud));
    int i = 0;
    char * pch;
    pch = strtok (ligne,",");
    double a = atof(pch);
        vec[i]=a;
    while (pch != NULL)
    {
         i=i+1;
         pch = strtok (NULL, ",");
         if(i<taille)
            {
            double a = atof(pch);
            vec[i]=a;
            }
            else
                P->c=pch;
    }
    P->vect =vec;
    P->indice=x;
    P->Suiv=L;
  return P;
}
void normaliser(Noeud * L, int taille){
 Noeud * T=L;
 while(T!=NULL)
  {
     double Vecteur=0.0;
        for (int i=0; i< taille ;i++){
            Vecteur = Vecteur + pow(T->vect[i],2);
        }
        double absVec = sqrt(Vecteur);
        for (int i=0; i< taille ;i++){
            T->vect[i] = T->vect[i]/absVec;
        }   
     T=T->Suiv;
  }
}
void VectMoyen(double * vec , Noeud * L, int taille)
{
 Noeud * T=L;
 int i = 0;
 int len=0;
 double som[taille];
 while(T!=NULL)
  {
        len++;
        for(int i =0 ; i<taille;i++){
           som[i]=som[i]+ T->vect[i]; 
        }
     
        T=T->Suiv;
  }
   for(int i =0 ; i<taille;i++) {
        vec[i]=som[i]/len; 
        }
}
void maxValueMinVale(double * vecteur, double *min, double *max, int taille){
         *max=vecteur[0];
         *min=vecteur[0];
        for (int i= 1; i<taille; i++){
            if(vecteur[i]>*max){*max =vecteur[i];}
            if(vecteur[i]<*min){*min=vecteur[i];}
        }
        *max=*max+0.05;
        *min=*min-0.05;    
    }

 double ***genererMap2 (double mi, double ma, double ***ma60, int taille)
 {
       srand(time(NULL));
       double def = ma - mi;     
       for (int i=0; i<6; i++){
            for(int j=0; j<10 ; j++){
                for(int k=0; k<taille; k++){                     
                         double nombre = (double)rand() / (double)RAND_MAX;
                        double alea  = mi + (nombre * def);
                    if ( (alea > ma) || (alea < mi) )
                    {
                            printf("\t erreurRandom");
                    }
                    else{
                        ma60[i][j][k]=alea;

                    }
                }
            }
        }
        return ma60;
    }

double ***allocationDynamique3D(double*** ma60 , int taille )
{
   ma60=(double***) malloc(sizeof(double**) * 6); //première dimension
   if (ma60==NULL) { printf ("Erreur1\n" ); exit (-1); }
   for (int i=0;i<10;i++) {
   ma60[i] = (double**) malloc(sizeof(double*)*10 ); //deuxième dimension
   if (ma60[i]==NULL) { printf ("Erreur22\n" ); exit (-1); }
   for (int j=0;j<10;j++) {
     ma60[i][j] = (double*) malloc(sizeof(double )*taille ); // 3 dimension
     if (ma60[i][j]==NULL) { printf ("Erreur3\n" ); exit (-1); }
    }
   }
return ma60;
}

char **allocationDynamique2D(char** matFinale )
{
   matFinale = (char**) malloc(sizeof(char*)*6 ); //1ere dimension
   if (matFinale==NULL) { printf ("Erreur\n" ); exit (-1); }
   for (int j=0;j<6;j++) {
     matFinale[j] = (char*) malloc(sizeof(char)*10 ); // 2eme dimension
     if (matFinale[j]==NULL) { printf ("Erreur\n" ); exit (-1); }
    } 
return matFinale;
}
double calclueDistance(double vect1[],double vect2[], int t){
        double dist=0.0;
        double  result=0.0;
        for (int i = 0; i<t; i++){
            result=result+ pow((vect1[i]-vect2[i]), 2);
        }
        dist = sqrt(result);
        return dist;
    }
int get_indice(){

}
double ***apprentissage(Noeud * L, double*** ma60, double alpha, int nbrIT , int nbr_ligne, int taille, int Rmax )
{       
        int rang =Rmax;
        int temp = 0;
        int ligneBmu =0; int colBmu = 0;
        int tab[taille];
        for (int i = 0; i < taille; i++)
            tab[i] = 0;
        int p = nbrIT/Rmax;  
        for (int it=0; it<nbrIT;it++){
            int n = rand() % nbr_ligne;
            int test = 0, compt =0;
            while(test == 0 && compt <taille+1){
               if (tab[n] == 0)
                    {
                     test = 1; tab[n]=1;
                    } else 
                          {
                           compt++; n = rand() % nbr_ligne;
                          }
            }
            int trouve = 0;
            Noeud * T=L;
             while(T!=NULL && trouve == 0)
              {
                if (T->indice == n){
                 double bmu = ma60[0][0][0];
                 for(int l =0; l<6;l++){
                    for(int c =0; c<10; c++){
                        double dist = calclueDistance(T->vect,ma60[l][c], taille);
                        if(dist<bmu){
                           bmu = dist;
                           ligneBmu =l; colBmu = c;
                        }

                    }
                 }                                    
                  int def = it/p;
                   if (def> temp)
                   {
                       rang = rang -1; temp = def;
                   }              
                     for (int ll=ligneBmu-rang;ll<(ligneBmu+rang+1); ll++){
                         for(int co=(colBmu-rang); co<(colBmu+rang+1);co++){
                            for (int d=0;d<taille;d++){
                              if(ll>-1 && ll<6 && co>-1 && co <10){
                                 double ancienneVal = ma60[ll][co][d];
                                 ma60[ll][co][d] = ancienneVal + alpha * (T->vect[d] - ancienneVal);
                                }
                            }
                        }
                    }
                 trouve =1;
                 }
                T=T->Suiv;
                }
        alpha = alpha * (1-((it+1)/nbrIT));
      }
 return ma60;
}

char** matriceFinale2(Noeud * L, double*** ma60, char** matFinale, int nbrL, int taille)
{      
             
            char c;
            for (int i = 0; i < 6; ++i)
            {
               for (int j = 0; j < 10; ++j)
               {
                int indicebmu =0; c = 'A';
                Noeud * T=L;
                double bmu = calclueDistance(T->vect,ma60[i][j], taille);
                while(T!=NULL)
                    {
                                    
                          double dist = calclueDistance(T->vect,ma60[i][j], taille);
                          if(dist<bmu){         
                                bmu = dist; 
                                if (T->indice< (nbrL/3)){c = 'A';}else{if(T->indice>= (nbrL/3) && T->indice< ((nbrL*2)/3)){c = 'B';}else{c = 'C';}}                           
                            }           
                        T=T->Suiv;
                    }
                matFinale[i][j] = c;
               }
            }
            
 return matFinale;
}
void afficher (char **matFinale)
 {
       for (int i=0; i<6; i++){

            for(int j=0; j<10 ; j++){
                if(matFinale[i][j] != NULL)
                    printf("%c   ",matFinale[i][j]); 
			else
                            printf("#   ");
                }
        printf("\n");
        }
    }

int main()
{
    Noeud * L = NULL;
    int x = 0;
    int taille =0;
    taille = (get_taille("iris.data")); 
    FILE *f = fopen("iris.data","r");
    if(f == NULL){
        printf("Fichier introuvable !");
        exit(1);
    }
    char ligne[256];
    while(fgets(ligne, 255, f)!= NULL){
    L=InsererD(L,x,ligne,taille);
    x++;
    }
    double*** ma60;
    ma60 = allocationDynamique3D(ma60,taille);
    char** matFinale;
    matFinale = allocationDynamique2D(matFinale);
    double *vectMoy; double *maxValue; double *minValue;
    vectMoy = (double*)malloc((taille) * sizeof(double));
    minValue = (double*)malloc(sizeof(double));
    maxValue = (double*)malloc(sizeof(double));
    normaliser(L, taille);
    VectMoyen(vectMoy,L, taille);
    maxValueMinVale(vectMoy,minValue,maxValue,taille);
    ma60 = genererMap2(*minValue, *maxValue,ma60, taille);
    ma60 = apprentissage(L,ma60,0.4,300,x,taille, 3);
    ma60 = apprentissage(L,ma60,0.2,1500,x,taille, 3);
    matFinale = matriceFinale2(L,ma60,matFinale,x, taille);
    printf("--------------------------------------\n");
    afficher(matFinale);
    printf("--------------------------------------\n");
    return 0;
}
