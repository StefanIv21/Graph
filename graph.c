#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//strcutura pentru graf
typedef struct graf{
    int noduri;
    double **mat;
}graf;

//strcutura care functioneaza ca o stiva 
typedef struct nod{
   double elem;
   struct nod *next;
}Nod, *List;
//functie care pune elemntul de stiva
void push(List *stiva,double x){
    List nou;
    nou = (List)malloc(sizeof(Nod));
    nou->elem = x;
    nou->next = *stiva;
    *stiva = nou;
}
//functie care scoate elementul de pe stiva
List pop(List stiva){
  List p = stiva;
  stiva = stiva->next;
  free(p);
  return stiva;
}
//primul element de pe stiva
int top(List stiva)
{
    return stiva->elem;
}
//primul element de pe stiva2(pentru cost-uri)
double top2(List stiva)
{
    return stiva->elem;
}
//verific daca stiva e goala
int is_empty(List head)
{
    if(head == NULL)
        return 0;
   return 1;
}
//functie care printeaza calea la task 1
void printcale(int n, int *P[n], int i, int j,int ok)
{
    if(P[i][j] == -1)
	{
        if(ok == 0)
        {
		printf("%d",j);
        }
		return;
	}
	printcale(n,P,i,P[i][j],ok);
	printf(" %d",j);
}
//adauga muchia la matricea graf 
void add_edge(graf *g,int i,int j,double cost,int *path[]){
    g->mat[i][j] = cost;
    path[i][j] = i;//adaug elementul de plecare
}
//functie care sterge legaturile  unui nod
void del_edge(graf *g,int i){
    for(int j = 0; j<g->noduri; j++)
    {
        g->mat[i][j] = 0;
        g->mat[j][i] = 0;
    }
}
//fucntie care parcurge graful si pune intr un vector nodurile prin care a trecut
void parcurgere_normala(int x,int *plus,graf *g)
{
    plus[x] = 1;
    for(int i=0 ; i < g->noduri ; i ++)
        if(plus[i] == 0 && g->mat[x][i] == 1)
            parcurgere_normala(i,plus,g);
}
//asemena ca functia de mai sus,parcurge invers graful
void parcurgere_inversa(int x,int *minus,graf *g)
{
    minus[x] = 1;
    for(int i=0 ; i < g->noduri ; i ++)
        if(minus[i] == 0 && g->mat[i][x] == 1)
            parcurgere_inversa(i,minus,g);
}
//functie care calculeaza algoritmul floyd_warshall
void floyd_warshall(graf *g,int *path[],double **m){
   
    for(int i = 0; i < g->noduri;i++)
        for(int j = 0;j < g->noduri ;j++){
            if(g->mat[i][j] != 0 ||  i == j)
                m[i][j] = g->mat[i][j];
            else{
                m[i][j] = 9999;
                path[i][j] = -1; 
            }
            path[i][i] = -1; 
        }
    for( int k = 0; k < g->noduri; k++)
        for(int i = 0; i < g->noduri; i++)
            for(int j = 0; j < g->noduri; j++)
            {
                if(m[i][j] > m[i][k] + m[k][j] && m[i][k] + m[k][j] > 0)
                {
                    m[i][j] = m[i][k] + m[k][j];
                    path[i][j] = path[k][j];//retin nodul prin care se face trecerea
                }
            }
}
//functie care calculeaza drumul minim intre 2 puncte
void min(int nod_current,graf *g,int *vizitate,double *cost,int nr_doi,double *valori,int *indice_valori,int noduri_viz[g->noduri][g->noduri],int copie_nod_viz[g->noduri][g->noduri],List stiva,int *linie_nod_viz,List stiva2)
{
    int i = 0,nod_urmator;
    //cresc indicele la nodul vizitat
    vizitate[nod_current]++;
    //pun nodul in matricea de noduri vizitate
    noduri_viz[*linie_nod_viz][nod_current] = nod_current;
    //daca un nod a fost de 2 ori vizitat ,ma opresc
    if(vizitate[nod_current] > 1)
    {
        valori[*indice_valori] += *cost;
        (*indice_valori)++;
        return;
    }
    //variabila in care verific daca un nod este ultimul nod
    int ok = 0;
    //variabila in care pun nr de drumuri ale unui nod
    int nr = 0;
    //calculez numarul de drumuri
    for(i = 0; i <g->noduri; i++)
    {
        if(((g->mat[nod_current][i]!=0)&&(vizitate[i] == 0)))nr++;
    }
    if(nr > 1)
    {
        //pun obtiuniile pe stiva
       for(i = 0; i <g->noduri; i++)
            if(((g->mat[nod_current][i] != 0)&&(vizitate[i] == 0)))
                push(&stiva,i);
        //cat timp stiva nu este goala
        while(is_empty(stiva))
        {
            for(int i = 0; i<g->noduri; i++)
            copie_nod_viz[*linie_nod_viz][i] = noduri_viz[*linie_nod_viz][i];
            (*linie_nod_viz)++;
            //pun costul initial pe stiva
            push(&stiva2,*cost);
            //adaug la cost muchiile pentru o optiune
             *cost += g->mat[nod_current][top(stiva)];
            //apelez din nou funcita(calculez drumul ce porneste de la optiunea data)
            min(top(stiva),g,vizitate,cost,nr_doi,valori,indice_valori,noduri_viz,copie_nod_viz,stiva,linie_nod_viz,stiva2);
            //sterg indexul nodurile vizitate in timpul apelarii functiei
            for(int i = 0; i<g->noduri; i++)
            {
                if(noduri_viz[*linie_nod_viz][i] == i && noduri_viz[*linie_nod_viz][i] != copie_nod_viz[(*linie_nod_viz)-1][i])
                    vizitate[i] = 0;
            }
            //iau cost ul initial
            *cost = top2(stiva2);
            stiva = pop(stiva);
             stiva2 = pop(stiva2);
            //sterg linia cu nodurile vizitate in timpul apelarii functiei
            for(int i = 0; i<g->noduri; i++)
                noduri_viz[*linie_nod_viz][i] = 0;
        }
        (*linie_nod_viz)--;
       return;
    }
    //daca nu se bifurca de la un nod
    if(nr<2)
    {
        //iterez prin fiecare nod
        for(i = 0; i<g->noduri; i++)
        {
            //daca exista o muchie care porneste de la nodul intial si nodul la care ajunge nu a fost vizitat,marhcez nodul final din muchie si adaug muchia la cost
            if(((g->mat[nod_current][i] != 0)&&(vizitate[i] == 0)))
            {
                ok = 1;
                *cost += g->mat[nod_current][i];
                nod_urmator = i;
            }
            //daca nu mai am muchi catre alte noduri nevizitate si i ul este nodul de intoarcere ,i devine nodul urmator,fiind si ultimul nod(funtia si opreste dupa)
            if((vizitate[i]==1)&&(i==nr_doi)&&(g->mat[nod_current][i]!=0))
            {
                nod_urmator = i;
                //pun la cost muchia
                *cost += g->mat[nod_current][i];
                ok = 0;
            }   
        }
        if(ok == 0)
        {
            valori[*indice_valori] += *cost;
            (*indice_valori)++;
            return;
        }
    }
    min(nod_urmator,g,vizitate,cost,nr_doi,valori,indice_valori,noduri_viz,copie_nod_viz,stiva,linie_nod_viz,stiva2);
}

int main(){

    int noduri = 0,muchi = 0,nr_dep = 0;
    scanf("%d%d%d",&noduri,&muchi,&nr_dep);
    //vector in care retin depoztile curente din task ul cerut
    int depozit_curent[noduri];
    //vector in care retin depozitele initiale
    int depozite[noduri];
    //variabila in care retin costul unui drum
    double cost = 0;
    double max1 = -1;
    //vector in care retin task ul ce trebuie rezolvat
    char nume[2];
    //variabila unde retin numarul de cerinte ce trebuie rezolvate
    int repetare_task =  0;
    //aloc memorie pentru o variabila de tipul graf
    graf *g = malloc(sizeof(*g));
    g->noduri = noduri;
    //aloc memorie matricei
    g->mat = malloc(sizeof(double*) * noduri);
	for(int i = 0; i < noduri; i++)	
		g->mat[i] = malloc(sizeof(double) * noduri); 
    for(int i = 0; i < noduri; i++)
        for(int j = 0; j < noduri; j++)
            g->mat[i][j]=0;
    //aloc memorie pentru o matrice in care o sa retinem caile catre drumuri
    int **path = malloc(sizeof(int*) * g->noduri);
	for(int i = 0; i < g->noduri; i++)	
		path[i] = malloc(sizeof(int) * g->noduri);
    //citesc datale pentru graful dat
    for(int i = 0; i < muchi; i++){
        int nod_plecare, nod_sosire;
        scanf("%d%d%lf", &nod_plecare, &nod_sosire, &cost);
        add_edge(g,nod_plecare, nod_sosire, cost, path);
    }
    //citesc fiecare depozit si il pun in vector
    for(int i=0;i<nr_dep;i++)
        scanf("%d",&depozite[i]);
    //citesc numarul de cerinte ce trebuiesc rezolvate
    scanf("%d",&repetare_task);
    //matrice in care pun magazinele pentru fiecare task
    int magazine[repetare_task][noduri];
    for (int i = 0; i < repetare_task; i++)
        for(int j = 0; j < noduri; j++)
            magazine[i][j]=0;
    //vector in care retin numarul de magazine pentru fiecare task
    int nr_mag[noduri];
    int i = 0;
    int copie_repetare_task = repetare_task;
    while(repetare_task>0)
    {
        scanf("%s",nume);
        nume[2]='\0';
        if(!strstr(nume,"e1"))
            break;
        scanf("%d",&depozit_curent[i]);
        scanf("%d",&nr_mag[i]);
        for(int j=0;j<nr_mag[i];j++)
            scanf("%d",&magazine[i][j]);
        repetare_task--;
        i++;
    }
    if(strstr(nume,"e1"))
    { 
        //matrice in care pun costul drumurilor dupa aplicarea algoritmului Floyd–Warshall 
        double **floyd;
        floyd = calloc((g->noduri),sizeof(*floyd));
        for(int i = 0; i< g->noduri; i++)
            floyd[i] = calloc((g->noduri),sizeof(**floyd));
        floyd_warshall(g,path,floyd);

        int ok = 0;//variabila pentru a modifica printarea 
        int j = 0;
        while(copie_repetare_task>0)
        {
            //variabila in care calculez suma totala
            double suma = 0;
            for(int i = 0; i < nr_mag[j]; i++)
            {
                printf("%d\n",magazine[j][i]);
                int c = magazine[j][i];
                printf("%0.1lf %0.1lf",floyd[depozit_curent[j]][c],floyd[c][depozit_curent[j]]);
                printf("\n");
                printcale(noduri,path,depozit_curent[j],c,ok);
                ok = 1;
                printcale(noduri,path,c,depozit_curent[j],ok);
                ok=  0;
                printf("\n");
                suma = suma + floyd[depozit_curent[j]][c] + floyd[c][depozit_curent[j]];
            }
            if(copie_repetare_task != 1)
                printf("%0.1lf\n",suma);
            else
                printf("%0.1lf",suma);
            j++;
            copie_repetare_task--;
        }
        //eliberez memoria ocupata
        for(int i = 0; i < g->noduri; i++)
		    free(floyd[i]);
		free(floyd);
    }
    if(strstr(nume,"e2"))
    {
        //vectori pentru marcarea nodurilor vizitate in timpul parcurgerilor
        int plus[noduri],minus[noduri];
        //vector pentru a retine nodurile care au intrat intr o componenta tare conexa
        int ctc[g->noduri];
        //variabila in care retin numarul de componente tare conexe
        int nrc=0;
        //matrice in care pun nodurile care fac parte din fiecare componenta tare conexa
        int noduri_conex[noduri][noduri];
        for(int i = 0; i < noduri; i++)
            for(int j = 0; j < noduri; j++)
                noduri_conex[i][j] = 0;
        //inlocuiesc costul drumului cu 1
        for(int i = 0;i < g->noduri; i++)
            for(int j = 0; j< g->noduri; j++)
            {
                if(g->mat[i][j] != 0)
                g->mat[i][j] = 1;
            }
        //elimin legatura nodurilor cu depozitele 
        for(int i = 0;i < nr_dep; i++)
            del_edge(g,depozite[i]);
        int ok = 0;
        int linie = 0;
        int coloana = 0;
        for(int i = 0; i<noduri; i++)
            ctc[i]=0;
        for(int i = 0 ; i<g->noduri ; ++i)
        //daca ndoul nu face parte dintr o componenta tare conexa,aplic algoritumul plus-minus
            if(ctc[i] == 0)
            {
                for(int j = 0; j < g->noduri ; ++j)
                    minus[j] = plus[j] = 0;
                //daca nodul curent e depozit , ma opresc
                for(int j = 0;j < nr_dep; j++)
                {
                    if(i == depozite[j])
                    {
                        ok = 1;
                        break;
                    }
                }
                if(ok == 0)
                {
                    //parcurg dfs prin graf normal si in sens invers
                    parcurgere_normala(i,minus,g);
                    parcurgere_inversa(i,plus,g);
                    nrc++;
                }
                //daca nodurile se gasesc in amblele tablouri ,inseamna ca sunt componente tare conexe
                for(int j = 0; j < g->noduri ; ++j)
                    if(minus[j] == 1 && plus[j] == 1)
                    {
                        noduri_conex[linie][coloana]=j;
                        coloana++;
                        ctc[j]++;
                    }
                linie++;
                coloana = 0;
                ok = 0;
            }
        //afisez numarul de componente tare conexe
        printf("%d\n",nrc);
        //variabila pentru a retine linia matricei
        int schimba =-1;
        while(nrc > 0)
        {
            //parcurg fiecare linie a matricei
            for(int i = 0; i<noduri; i++)
            {
                //daca linia incepe cu un element nenul inseamna ca avem noduri din componenta tare conexa
                if(noduri_conex[i][0] != 0)
                {
                    //retin lini
                    schimba = i;
                    int j = 0;
                    while(noduri_conex[i][j]!=0)
                    {
                        if(j == 0)
                            printf("%d",noduri_conex[i][j]);
                        else
                            printf(" %d",noduri_conex[i][j]);
                        j++;
                    }
                    break;
                }
            }
        //linia cu primul elemnt devine 0 ca sa nu mai trec prin el
        noduri_conex[schimba][0]=0;
        nrc--;
        if(nrc != 0)
            printf("\n");
        }   
    }
    if(strstr(nume,"e3"))
    {
        //vector in care retin nodurile vizitate
        int vizitate[noduri];
        //vector in care pun costul drumurilor
        double valori[noduri];
        for(int j = 0; j<noduri; j++)
        {
            vizitate[j]=0;
            valori[j]=0;
        }
        int indice_valori = 0;
        cost = 0;
        int linie_nod_vizitat = 0;
        //matrice in care retin nodurile viiztate
        int noduri_viz[noduri][noduri];
        //copie la matricea de mai sus
        int copie_noduri_viz[noduri][noduri];
        for(int i = 0; i<noduri; i++)
            for(int j = 0; j<noduri; j++)
            {
                copie_noduri_viz[i][j] = 0;
                noduri_viz[i][j]=0;
            }
        //stiva pentru a retine bifurcatiile unui nod
        List stiva = NULL;
        //stiva pentru a retine costul
        List stiva2 = NULL;
        //variabila in care retin numarul de zone
        int zone;
        int copie_zone;
        scanf("%d",&zone);
        copie_zone = zone;
        //variabila in care retin nodul de plecare
        int primul_nr;
        //variabila in care retin nodul de intoarcere
        int nr_doi;
        //copie la matricea grafului
        double copie[noduri][noduri];
        for(int  i= 0; i<noduri; i++)
            for(int j = 0 ;j<noduri; j++)
                copie[i][j] = g->mat[i][j];
        while(zone > 0)
        {
            //variabila in care retin numarul de noduri
            int nrnod=0;
            int copie_nrnod=0,copie2_nrnod=0;
            scanf("%d",&nrnod);
            copie_nrnod = nrnod;
            copie2_nrnod = copie_nrnod;
            //vector in care pun nodurile din fiecare componenta tare  conexa
            int noduri_cur[nrnod];
            for(int i = 0;i <nrnod; i++)
                noduri_cur[i] = 0;
            while (nrnod>0)
            {
                scanf("%d",&noduri_cur[nrnod-1]);
                nrnod--;
            }
            //aflu nodul de plecare si nodul de intoarcere
            for(int k = 0; k<nr_dep; k++)
            {
                for(int i = 0; i<copie2_nrnod; i++)
            {
            if(g->mat[depozite[k]][noduri_cur[i]] != 0)
                primul_nr = noduri_cur[i];
            if(g->mat[noduri_cur[i]][depozite[k]] != 0)  
                nr_doi = noduri_cur[i];
            }
            copie_nrnod = copie2_nrnod;
            //adaug la cost muchia de plecare si muchia de intoarcere
            cost += g->mat[depozite[k]][primul_nr] + g->mat[nr_doi][depozite[k]];
            int i = 0;
            copie_nrnod = copie_nrnod-1;
            //sterg nodurile care nu se afla in componenta tare conexa
            for( i = 0; i<g->noduri; i++)
            {
                if(i!=noduri_cur[copie_nrnod])
                    del_edge(g,i);
                else
                {
                    if(copie_nrnod != 0)
                    copie_nrnod = copie_nrnod - 1;
                }
            }
            //daca un singur nod se afla in componenta tare conexa,nu mai intram in functie pentru a calcula costul
            if(copie2_nrnod != 1)
                min(primul_nr,g,vizitate,&cost,nr_doi,valori,&indice_valori,copie_noduri_viz,noduri_viz,stiva,&linie_nod_vizitat,stiva2);
            if(copie2_nrnod==1)
            {
                valori[indice_valori]=cost;
                indice_valori++;
            }
            //reiau datele initiale pentru urmatorul nod depozit
            cost = 0;
            for(int  j = 0; j< noduri; j++)
                vizitate[j] = 0;
            linie_nod_vizitat = 0;
            for(int i = 0; i<noduri; i++)
                for(int j = 0; j<noduri; j++)
                {
                copie_noduri_viz[i][j] = 0;
                noduri_viz[i][j] = 0;
                g->mat[i][j] = copie[i][j];
                }
            }
            zone--;
            cost = 0;
             for(int  j = 0; j< noduri; j++)
                vizitate[j] = 0;
            linie_nod_vizitat = 0;
            for(int i = 0; i<noduri; i++)
                for(int j = 0; j<noduri; j++)
                {
                copie_noduri_viz[i][j] = 0;
                noduri_viz[i][j] = 0;
                g->mat[i][j] = copie[i][j];
                }
        }
        //daca numarul de zone este mai mare ca 1 ,trebuie afisat costul pentru fiecare zona in ordine desc
        if(copie_zone > 1)
        {
            //pentru fiecare zona,iau maximul din vectorul de costuri si afisez
            while(copie_zone > 0)
            {   
                max1 = -1;
                for(int i = 0; i<noduri; i++)
                {
                    if(valori[i] > max1 && valori[i] != 0)
                    {
                        max1 = valori[i];
                        //linia devine 0 pentru a nu o mai lua in calcul
                        valori[i] = 0;
                    }
                }
            if(copie_zone != 1)
                printf("%0.1lf\n",max1);
            else
                printf("%0.1lf",max1);
            copie_zone--;
            }
        }
        //daca numarul de zone este 1 ,trebuie afisat costul minim
        else
        {
            while(copie_zone>0)
            {   
                double min1 = 999;
                for(int i = 0; i <noduri; i++)
                {
                    if(valori[i] < min1 && valori[i] != 0)
                    {
                        min1 = valori[i];
                        valori[i] = 0;
                    }
                }
                printf("%0.1lf",min1);
                copie_zone--;
            }
        }
    }
    //eliberez memoria
    for(int z = 0; z <g->noduri; z++)
		free(g->mat[z]);
	free(g->mat);
    for(int z = 0; z <g->noduri; z++)
			free(path[z]);
	free(path);
    free(g); 
    return 0;
}