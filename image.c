#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nmax 10
struct maillon_ligne
{
	int pix;
	struct maillon_ligne *suiv;
};
typedef struct maillon_ligne *ptr_ligne;

typedef ptr_ligne T_image[nmax];
struct couple
{
	int val, nb;
};

struct maillon_ligne_compressee
{
	struct couple pix;
	struct maillon_ligne_compressee *suiv;
};
typedef struct maillon_ligne_compressee *ptr_ligne_compressee;

typedef ptr_ligne_compressee T_image_compressee[nmax];

T_image im, ima;
int nbligne;
T_image_compressee imc;
ptr_ligne_compressee ligne;

void ajout(int e, ptr_ligne *L)
{
	ptr_ligne p;
	p = malloc(sizeof(struct maillon_ligne));
	p->pix = e;
	p->suiv = *L;
	*L = p;
}

void ajouter(struct couple c, ptr_ligne_compressee *L)
{
	ptr_ligne_compressee p;
	p = malloc(sizeof(struct maillon_ligne_compressee));
	p->pix = c;
	p->suiv = *L;
	*L = p;
}

void afficheLigne(ptr_ligne L)
{
	while (L != NULL)
	{
		printf("%d ", L->pix);
		L = L->suiv;
	}
	printf("\n");
}

void afficheLigneCompressee(ptr_ligne_compressee L)
{
	while (L != NULL)
	{
		printf("(%d,%d) ", (L->pix).val, (L->pix).nb);
		L = L->suiv;
	}
	printf("\n");
}

void creer_image(T_image *im, int *nbligne)
{
	FILE *fp;
	int x, i;
	if ((fp = fopen("image.txt", "r")) == NULL)
	{
		fprintf(stderr, "Cannot open image.txt\n");
		exit(1);
	}
	fscanf(fp, "%d", &(*nbligne));
	for (i = 0; i < *nbligne; i++)
		(*im)[i] = NULL;
	i = 0;
	while (i != *nbligne)
	{
		do
		{
			fscanf(fp, "%i ", &x);
			if (x != -1)
				ajout(x, &(*im)[i]);
		} while (x != -1);
		i++;
	}
	fclose(fp);
}

void affiche(T_image im, int nbligne)
{
	ptr_ligne p;
	int i;
	for (i = 0; i < nbligne; i++)
	{
		p = im[i];
		while (p != NULL)
		{
			printf("%d ", p->pix);
			p = p->suiv;
		}
		printf("\n");
	}
}

void affichecompressee(T_image_compressee im, int nbligne)
{
	ptr_ligne_compressee p;
	int i;
	for (i = 0; i < nbligne; i++)
	{
		p = im[i];
		while (p != NULL)
		{
			printf("(%d,%d)", p->pix.val, p->pix.nb);
			p = p->suiv;
		}
		printf("\n");
		;
	}
}

void compresse_ligne(ptr_ligne A, ptr_ligne_compressee *B)
{



	struct couple e;      // on Définit un couple e
	e.val = A->pix;      // on assigne à la valeur de e, le premier pixel de la ligne
	e.nb = 1;            // il a tout de suite une première occurence d'où son nombre d'occurence devient 1

//on parcourt la liste chainée qui représente la ligne. à chaque fois qu'on rencontre un meme pixel que le précédent, on incrémente son nombre d'occurence

	while (A->suiv != NULL)
	{
              //on travaille maintenant sur la suite de la ligne. tant qu'un pixel correspond à la valeur de e, on incrémente
			  // son nombre d'occurence. c'est ce que je traite dans le "if"

			  //si on ne rencontre plus le meme pixel , on se rend compte que c'est la fin de la série de pixel et on ajoute le couple (valeur, occurence) à la liste chainée
			  // qui constitue la ligne compressée	
			  // Après ça ,on prend assigne à la valeur du couple, le nouveau pixel trouvé et on met son nombre d'occurence à 1.c'est ce que je traite dans le "else"	
		if (A->suiv->pix == e.val)
		{
			e.nb = e.nb + 1;
		}
		else
		{
			ajouter(e, &(*B));

			e.val = A->suiv->pix;
			e.nb = 1;
		}
		A->suiv = A->suiv->suiv;
	}
	ajouter(e, &(*B));

}

void compresse_image(T_image im, int nbligne, T_image_compressee *imc)
{
 //on fait une boucle allant de 1 au nombre de ligne de l'image dans laquelle on parcourt chaque ligne de l'image. Ensuite on compresse la ligne 

	int l;
	for (l = 1; l < nbligne; ++l)
	{
		compresse_ligne(im[l], &(*imc)[l]);
	}
}

void decompresse_ligne(ptr_ligne_compressee A,ptr_ligne *B)
{
  int col =0;
	while (A!=NULL)
	{

		//on prend le premier élément de la ligne compressé et on fait une boucle allant de 1 
		// à son nombre d'occurence. et on ajoute la valeur de ce pixel à la liste chainée représentant la ligne non compressé
		struct couple e;
	    e=A->pix;
        int i;
		
		for(i=1;i<=e.nb;++i)
		{
            ajout(e.val,&(B)[col]);
		}
		A=A->suiv;
	}
	
}


void decompresse_image(T_image_compressee imc,int nbligne,T_image *ima)
{
int l;

/*
Dans une boucle allant de 1 au nombre de ligne de l'image, on décompresse chaque ligne compressé 
du tableau d'image compressé.
*/

	for (l = 1; l <= nbligne; ++l)
	{
		decompresse_ligne(imc[l],&(*ima)[l]);
	}

}



int main()
{

	creer_image(&im, &nbligne);
	printf("Image saisie\n");
	affiche(im, nbligne);

	compresse_ligne(im[0], &imc[0]);
	afficheLigneCompressee(imc[0]);

	compresse_image(im, nbligne, &imc);
	printf("\nImage compressee\n");
	affichecompressee(imc, nbligne);
	decompresse_ligne(imc[0], &(ima[0]));
	printf("\nLigne 1 decompressee\n");
	affiche(ima,1);
	decompresse_image(imc,nbligne,&ima);
	printf("\nImage decompressé : \n");
	affiche(ima,nbligne);
	return 0;
}
 