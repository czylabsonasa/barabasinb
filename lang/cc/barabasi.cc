#include <cstdio>
#include <cstdlib>
#include <ctime>
#define _DEBUG( x )

int const M(8);//elmeleti max a kis m-re

struct{
   int m; // ennyi régi csúcshoz kapcsolódik az új 
   int lepes ; // ennyi új csűcs "keletkezik"
   int* lista=nullptr; // aka NULL,0
   int* fokszam=nullptr;
   int* gyakorisag=nullptr;
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
         fokszam[i]=m;
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
_DEBUG(
   for(int v=0;v<m;v++){
      printf( "%d " , kicsilista[v] ) ;
   }
   printf("\n")
) ;
      // osszekotes, listaba berakas
      fokszam[++ncsucs]=m;
      for(int v=0;v<m;v++)
      {
         lista[++nlista]=ncsucs;
         lista[++nlista]=kicsilista[v];
         ++fokszam[kicsilista[v]];
      }
      
   }//egylepes()

   void init(){
      lista = new int[ m*(m+1) + 2*m*lepes + 7 ] ; // lista[1..nlista] a csucsokat tartalmazza fokszam-szor
      fokszam = new int[ m + 1 + lepes + 7 ] ; // fokszam[i] az i-es csucs fokszama
      gyakorisag = new int[ m + 1 + lepes + 7 ] ; // gyakorisag[f] az f-fokszam gyakorisaga
      // elmeletileg a max fokszam = m+lepes, ha a kezdeti graf egy csucsat minden lepesben valasztja
   }//init()

   void eloszlas(){
      int maxfok=0;
      for(int i=1;i<=ncsucs;i++){
         if(fokszam[i]>maxfok){maxfok=fokszam[i];}
      }
      // eloszlas nullaz

fprintf(stderr,"ncsucs:%d\n",ncsucs) ;
fprintf(stderr,"maxfok:%d\n",maxfok) ;

      for(int f=m;f<=maxfok;f++){
         gyakorisag[f]=0;
      }
      // fokszamok kigyujtese
      for(int i=1;i<=ncsucs;i++){
         ++gyakorisag[fokszam[i]];
      }
      
   //return 0 ;
      // kiiras, a rekurziot is szamolja
      char outnev[64];
      sprintf(outnev,"bar_m%d_lepes%d",m,lepes);
      FILE*fout=fopen(outnev,"w");
int sum=0;
      double delta=0;
      double dlepes=d0+lepes*2*m;
      double cd=1.0/(m*(2.0+m));
      for(int d=m;d<=maxfok;d++){
         double rgy=gyakorisag[d]/double(dlepes);//ncsucs?
         delta+=(rgy-cd)*(rgy-cd);
         if(gyakorisag[d]>0){
            fprintf(fout,"%d %.12lf %.12lf\n",d,rgy,cd);
sum+=gyakorisag[d];
//fprintf(stderr,"%lf\n",gyakorisag[d]/double(ncsucs));
         }
         cd=d*cd/double(d+3);
      }//for(d)
      fclose(fout);
fprintf(stderr,"sum=%d\n",sum);
fprintf(stderr,"delta2=%.15lf\n",delta);

      // kiirja az m,lepest az mkrajz-ba
      fout=fopen("mkrajz","w");
      fprintf(fout,"cat alap.gp|sed s/_m/%d/|sed s/_lepes/%d/|sed s/_akt/%s/>%s.gp;gnuplot %s.gp",m,lepes,outnev,outnev,outnev);
      fclose(fout);

   }// eloszlas()

   void mfree(){// a lefoglalt tombok felszabaditasa
      delete[] lista;
      delete[] fokszam;
      delete[] gyakorisag;
   }//mfree()
   

} barabasi;


int main(){
   int& m(barabasi.m);
   int& n_step(barabasi.lepes);
	int max_step=1000000;
	int rep=3;


	srand(time(NULL));
	while(n_step<max_step){
	   barabasi.init();

		barabasi.kezdograf();
		for(int l=1;l<=n_step;l++){
			barabasi.egylepes() ;
		}

   barabasi.mfree();
	
	return 0 ;
}
