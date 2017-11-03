#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

using namespace chrono;
time_point<steady_clock> start;
double tik(int f=0){
   if(0==f)
   {
      start=steady_clock::now();
      return (-1.0);
   }
   else
   {
      duration<double> diff=steady_clock::now()-start;
      return diff.count();
   }
}
double tak(){
   return tik(1);
}


int const M(8);//elmeleti max a kis m-re

struct{
   int m; // ennyi régi csúcshoz kapcsolódik az új 
   int max_step ; // ennyi új csűcs "keletkezik"
   vector<int> lista;
   int nlista; // a lista aktualis merete
   int ncsucs;
   int d0;//kezdograf fokszama
   
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
            return true;
         }
         return false;
      }
      int operator[](int i){return x[i];}
   } kicsilista ;
   ////////////////

   int huz(){// 1...nlista-bol egy random indexet ad
      return lista[1+(rand()%nlista)];
   }//huz()
   
   void kezdograf(){//kezdo graf: egy teljes m+1 graf + nullazas
      nlista=0;
      for(int i=1;i<=m+1;i++){
         for(int j=1;j<=m;j++){
            lista[++nlista]=i;
         }
      }
      ncsucs=m+1;
      d0=m*(m+1);
   }//kezdograf()
   
   void egylepes(){
      //m-regi kivalasztas, PA
      kicsilista.urit();
      for(int v=1;v<=m;){
         int jelolt=huz();
         if(true==kicsilista.keres(jelolt)){++v;}      
      }

      // osszekotes, listaba berakas
      for(int v=0;v<m;v++){
         lista[++nlista]=ncsucs;
         lista[++nlista]=kicsilista[v];
      }
      
   }//egylepes()

   void init(int _m,int _max_step){
      m=_m;
      max_step=_max_step;
      lista=vector<int>(m*(m+1) + 2*m*max_step + 7);
   }//init()

   void gen(int n_step){
      kezdograf();
      for(int s=1;s<=n_step;s++){
         egylepes() ;
      }
   }
} barabasi;


int main(){
	srand(time(NULL));

   int m=3;
   int n_step=10;
   int max_step=10000000;
   double q=1.5;
   int n_rep=3;
   FILE* f=fopen("result.dat","w");  

   while(n_step<max_step){
      barabasi.init(m,n_step);
      double best=0;
      for(int i=0;i<n_rep;i++){
         tik();
         barabasi.gen(n_step);
         double st=tak();
         if(i==0){
            best=st;
         }else{
            best=std::max(best,st);
         }
      }
      fprintf(f,"%d %d %lf cc\n",m,n_step,best);
      n_step=int(q*n_step);
   }
   fclose(f);
	return 0 ;
}
