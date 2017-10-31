// Barabasi with deletion the oldest edges with prob 'p'
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>

using std::vector;
using std::max;

#define _DEBUG( x )

int const M(8);//elmeleti max a kis m-re

double runif(){return rand()/double(RAND_MAX);}//0..1 folytonos (<1)
int runif(int a){return 1+rand()%a;}//1...a inc.
int runif(int a,int b){return a+rand()%(1+b-a);}//a...b inc.


struct{
   int m; // ennyi régi csúcshoz kapcsolódik az új 
   double p; // a szuletes valsege
   int lepes ; // ennyi új csúcs "keletkezik" (igazából kevesebb p<1 miatt,
   // de ez egy konzervatív becslés)
   struct{
      int* x;
      int* beg;//mutato ez elejere
      int* end;//a vegere
      void init(int* px){
         x=end=px;
         beg=end+1;//ures
      }
      void add(int v){*(++end)=v;}//hozzad a vegehez
      void del(int ennyit){//torles az elejerol
         beg+=ennyit;
         if(beg>end){beg=end+1;}
      }
      size_t size(){return size_t(end-beg+1); }
      int operator[](int i){ return *(beg+i-1);}
   }lista;
   int ncsucs;// a csucsok aktualis szama
   
   struct{ // segedlista az m csucs huzasahoz
      int x[M]; 
      int nx;
      void urit(){nx=0;}
      bool keres(int v){ // igazat ad vissza, ha sikerul berakni (meg nem volt benn)
         x[nx]=v;
         int i=0;
         while(x[i]!=v){i++;}
         if(i==nx){// uj elem
            nx++;
            return false;
         }
         return true;
      }
      int operator[](int i){return x[i];}
   } kicsilista ;
   ////////////////

   void kezdograf(){
      for(int i=m+1;i<=2*m;i++){
         for(int j=1;j<=m;j++){
            if(i!=j){
               lista.add(i);
               lista.add(j);
            }
         }
      }
      ncsucs=2*m;
   }//kezdograf()
   
   void egylepes(){
      if(runif()<p){
         szuletes();
      }else{
         kihalas();
      }
   }
   
   void szuletes(){//a születés lépése
      //m-regi kivalasztas, PA
      kicsilista.urit();
      int n=lista.size();
      if(n>0){
         for(int v=1;v<=m;){
            int jelolt=lista[runif(1,n)];
            if(false==kicsilista.keres(jelolt)){++v;}      
         }
      }else{
         n=ncsucs;
         for(int v=1;v<=m;){
            int jelolt=runif(1,n);
            if(false==kicsilista.keres(jelolt)){++v;}      
         }
      }
      
      
_DEBUG(
   for(int v=0;v<m;v++){
      printf( "%d " , kicsilista[v] ) ;
   }
   printf("\n")
) ;
      // osszekotes, listaba berakas
      ++ncsucs;
      for(int v=0;v<m;v++)
      {
         lista.add(ncsucs);
         lista.add(kicsilista[v]);
      }
      
   }//egylepes()


   void kihalas(){
//      lista.del(2*m);
      lista.del(2);
   }

   void init(){
      lista.init(new int[ m*(m+1) + 2*m*lepes + 7 ]) ; // lista[1..nlista] a csucsokat tartalmazza fokszam-szor
      // elmeletileg a max fokszam = m+lepes, ha a kezdeti graf egy csucsat minden lepesben valasztja
   }//init()

   void eloszlas(){
      vector<int>fokszam(ncsucs+3,0); // fokszam[i] az i-es csucs fokszama
      
      
      int maxfok=0;
      int n=lista.size();
      for(int i=1;i<=n;i++){
         maxfok=max(maxfok,++fokszam[lista[i]]);
      }
      for(int i=1;i<=ncsucs;i++){
         if(fokszam[i]>maxfok){maxfok=fokszam[i];}
      }
      // eloszlas nullaz

   _DEBUG( printf("maxfok:%d\n",maxfok) ) ;

      vector<int>gyakorisag(maxfok+3,0); // gyakorisag[f] az f-fokszam gyakorisaga
      // fokszamok kigyujtese
      for(int i=1;i<=ncsucs;i++){
         ++gyakorisag[fokszam[i]];
      }
      
   //return 0 ;
      // kiiras, a rekurziot is szamolja
      char outnev[64];
      sprintf(outnev,"barDel_m%d_p%.2lf_lepes%d",m,p,lepes);
      FILE*fout=fopen(outnev,"w");
      double dlepes=double(ncsucs);
      for(int d=0;d<=maxfok;d++){
         if(gyakorisag[d]>0){
            fprintf(fout,"%d %.12lf\n",d,gyakorisag[d]/dlepes);
         }
      }//for(d)
      fclose(fout);

      // kiirja az m,lepest az mkrajz-ba
      fout=fopen("mkrajzDel","w");
      fprintf(fout,"cat alapDel.gp|sed s/_m/%d/|sed s/_p/%.2lf/|sed s/_lepes/%d/|sed s/_akt/%s/>%s.gp;gnuplot %s.gp",m,p,lepes,outnev,outnev,outnev);
      fclose(fout);
   }// eloszlas()

   void mfree(){// a lefoglalt tombok felszabaditasa
      delete[] lista.x;
   }//mfree()
   

} barabasiDel;


int main(int nx,char** x){
   // hibas hivasok kezelese
   int& m(barabasiDel.m);
   double& p(barabasiDel.p);
   int& lepes(barabasiDel.lepes);
   
   if((nx<3)||(1!=sscanf(x[1],"%d", &m))||(1!=sscanf(x[2],"%lf", &p))||
   (1!=sscanf(x[3],"%d", &lepes))||(m>=M)){
      printf("hasznalat: %s m p lepes\n",x[0]);
      printf("p>0.5 m<%d\n",M);
      return 1;
   }

	srand(time(NULL));
   //itt mar van ertelmes ertek m,lepes-nek
   barabasiDel.init();

	barabasiDel.kezdograf();
	for(int l=1;l<=lepes;l++){
		barabasiDel.egylepes() ;
	}
   barabasiDel.eloszlas();

   barabasiDel.mfree();
	
	return 0 ;
}
