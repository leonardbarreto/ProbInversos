/****************************************************PROBLEMAS INVERSOS EM ENGENHARIA 2012-02***************************************************/
/* Professores: Antonio Jose da Silva Neto                                                                                                     */
/*              Wagner Rambaldi Telles                                                                                                         */
/* Implementacao do metodo Algoritmo Genetico                                                                                                  */
/* Data: 30 de agosto de 2012                                                                                                                  */
/***********************************************************************************************************************************************/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Vari�veis Globais
#define QTDVAR 2     //Numero de Vari�veis do problema
#define TAMPOP 24     //Tamanho da Popula��o
#define TAMCROM 16    //Tamanho do Cromossomo
#define TAMCADEIA 32 //Tamanho da Cadeia de Cromossomos
#define NUMEVOL 151   //N�mero de Evolu��es
#define TXCO 0.8     //Taxa de Crossover
#define TXMT 0.3    //Taxa de Muta��o
#define TOTCOR 1    //Total de corridas a ser realizada no programa

//Fun��es Utilizadas
double Fitness(double, double);                                                    //Fun��o Objetivo
void PopulacaoReal(double [][TAMCADEIA], double [][QTDVAR], double [], double []); //Fun��o Cromossomo Real
void AptAdm(double []);                                                            //Fun��o de Aptid�o
int Roleta(double []);                                                             //Fun��o Roleta
double Mutacao(double);                                                            //Fun��o Muta��o
int Best(double []);                                                               //Funcao Best

//======================================================Programa Principal=====================================================================//
int main(){
   int ciclo, i, j, n, aux, exp, selecao, inicio, comp, fim, MT, melhor, melhor1, corrida;
   double Populacao[TAMPOP][TAMCADEIA], ProxGer[TAMPOP][TAMCADEIA], PopReal[TAMPOP][QTDVAR];
   double min[QTDVAR], max[QTDVAR], Apt[TAMPOP], f1[TAMCADEIA], f2[TAMCADEIA], solucao[TAMCADEIA];
   double soma, aleatorio, x1, x2;

//Gerando n�meros aleat�rios
//Utiliza o rel�gio do computador
//   srand((unsigned)time(NULL));

//Especifica qual semente ser� utilizada para gerar os n�meros aleat�rios
   unsigned semente = 1;
   srand(semente);

//Permite ao usi�rio digitar uma semente para gerar os n�meros aleat�rios
//   unsigned semente;
//   printf("Entre com a semente: ");
//   scanf ("%u", &semente);
//   srand(semente);

//--------------------------------------------------------Intervalo de busca das fun��es-------------------------------------------------------//
   min[0] = -2.; //Valor minimo do intervalo de busca de x1.
   max[0] =  2.; //Valor maximo do intervalo de busca de x1.
   min[1] = -2.; //Valor minimo do intervalo de busca de x2.
   max[1] =  2.; //Valor maximo do intervalo de busca de x1.
//---------------------------------------------------------------------Fim---------------------------------------------------------------------//

   FILE *arquivo;
   arquivo=fopen("GA.txt","w"); //Abrindo o arquivo para gravar os resultados

   clock_t tInicio, tFim, tDecorrido; //Calculando o tempo de execu��o do programa
   tInicio=clock();

   for(corrida = 1; corrida <= TOTCOR; corrida++){ //Numero de corridas realizadas pelo programa

//---------------------------------------------------------Gerando a populacao inicial---------------------------------------------------------//
//   printf("Populacao Inicial:\n");
   for(i = 0; i < TAMPOP; i++){ //Gerando os elementos da matriz de cromossomos.
      for(j = 0; j < TAMCADEIA; j++){
         Populacao[i][j] = (double)(0 + rand() % 2); //N�meros aleat�rios entre [0, 1]
//         printf("%.f ", Populacao[i][j]);
      }
//      printf("\n");
   } //Fim (Gerando os elementos da matriz de cromossomos).
//--------------------------------------------------------------------Fim----------------------------------------------------------------------//

//----------------------------------------------------Transformando a populacao binaria em real------------------------------------------------//
   PopulacaoReal(Populacao, PopReal, min, max); //Gerando a nova matriz com numeros reais.
//Imprimindo a populacao na base decimal.
//   printf("\nPopulacao Real:\n");
//   for(i = 0; i < TAMPOP; i++){
//      for(j = 0; j < QTDVAR; j++){
//         printf("%7.4f ", PopReal[i][j]);
//      }
//      printf("\n");
//   } //Fim (Gerando os elementos da matriz de cromossomos).
//--------------------------------------------------------------------Fim----------------------------------------------------------------------//

//----------------------------------------------Calculando a aptidao de cada cadeia de cromossomo----------------------------------------------//
   for(i = 0; i < TAMPOP; i++){ //Calcula a aptidao de cada cadeia de cromossomos.
      Apt[i] = Fitness(PopReal[i][0], PopReal[i][1]);
   }
   AptAdm(Apt); //Admencionaliza as Aptidoes.
//----------------------------------------------------------------Fim--------------------------------------------------------------------------//

//-----------------------------------------------------Calculando o melhor cromossomo----------------------------------------------------------//
   melhor = Best(Apt);
   melhor1 = melhor;
   for(i = 0; i < TAMCADEIA; i++){
      solucao[i]=Populacao[melhor1][i]; //Solu��o na forma bin�ria.
   }
   x1 = PopReal[melhor1][0]; //Guarda a melhor solu��o do primeiro cromossomo em formato real.
   x2 = PopReal[melhor1][1]; //Guarda a melhor solu��o do segundo cromossomo em formato real.
   fprintf(arquivo,"Melhor da Populacao Inicial:\n");
   fprintf(arquivo,"%10.6f %10.6f %10.6f\n", x1, x2, Fitness(x1, x2)); //Imprime a melhor solu��o at� o momento.
//   printf("\nMelhor solucao da Populacao Inicial:\n");
//   printf("%7.4f %7.4f %7.4f\n", x1, x2, Fitness(x1, x2)); //Imprime a melhor solu��o at� o momento.

//----------------------------------------------------------------Fim--------------------------------------------------------------------------//

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Evolucoes+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Ciclo de evolu��o da t�cnica: evolu��o, crossover e muta��o
   fprintf(arquivo,"\nEvolucoes:\n");
//   printf("\nCromossomos selecionados:\n");
   ciclo = 1;
   while(ciclo <= NUMEVOL){
      for(j = 0; j < TAMCADEIA; j++){
         ProxGer[0][j] = solucao[j]; //Pego minha melhor solu��o da Popula��o
//         printf("%.f ", ProxGer[0][j]);
      }
//      printf("\n");
      for(i = 1; i < TAMPOP; i++){
         selecao = Roleta(Apt);
         for(j = 0; j < TAMCADEIA; j++){
            ProxGer[i][j] = Populacao[selecao][j]; //Cromossomo selecionado na Popula��o.
//            printf("%.f ", ProxGer[i][j]);
         }
//         printf("\n");
      }

//----------------------------------------------------------------Reproduzir-------------------------------------------------------------------//
    for(i = 0; i < TAMPOP; i += 2){ //Realiza o crossover (crusamento) dos cromossomos selecionados 2 a 2.
       aleatorio = ((double)rand() / (double)RAND_MAX); //Numero aleatorio entre [0, 1].
       if(aleatorio <= TXCO){ //Inicio do crossover.
          inicio = rand() % (TAMCADEIA - 1); //Ponto de inicio do crossover.
          comp = ((TAMCADEIA - 1) - inicio); //Comprimento do crossover.
          fim = (rand() % (comp)) + inicio; //Ponto de termino do crossover.
          for(j = 0; j < TAMCADEIA; j++){ //Inicializando os vetores dos cromossomos filhos.
             f1[j] = -1.0; //Filho 1.
             f2[j] = -1.0; //Filho 2.
          } //Fim (Inicializando os vetores dos cromossomos filhos).
          for(j = inicio; j <= fim; j++){ //Gerando da parte interna dos cromossomos filhos.
             f1[j] = ProxGer[i][j]; //Filho 1 recebe os genes do Pai 1.
             f2[j] = ProxGer[i + 1][j]; //Filho 2 recebe os genes do Pai 2.
          } //Fim (Gerando da parte interna dos cromossomos filhos).
          for(j = 0; j < TAMCADEIA; j++){ //Gerando os extremos dos cromossomos filhos.
             if((f1[j] == -1) && (f2[j] == -1)){
                f1[j] = ProxGer[i + 1][j]; //Filho 1 recebe os genes do Pai 2.
                f2[j] = ProxGer[i][j]; //Filho 2 recebe os genes do Pai 1.
             }
          } //Fim (Gerando os extremos dos cromossomos filhos).
          MT = rand() % (TAMCADEIA); //Definindo o elemento do cromossomo f1 a sofrer mutacao.
          f1[MT] = Mutacao(f1[MT]); //Realizando a mutacao no filho 1.
          MT = rand() % (TAMCADEIA); //Definindo o elemento do cromossomo f1 a sofrer mutacao.
          f2[MT] = Mutacao(f2[MT]); //Realizando a mutacao  no filho 2.
          for(j = 0; j < TAMCADEIA; j++){
             Populacao[i][j] = f1[j]; //A nova populacao recebe o filho 1.
             Populacao[i + 1][j] = f2[j]; //A nova populacao recebe o filho 2.
          }
       } //Fim (Inicio do crossover).
       else{ //Definindo a nova Populacao de cromossomos caso nao ocorra o crossover.
          for(j = 0; j < TAMCADEIA; j++){ //A matriz Populacao recebe a matriz ProxGeracao.
             Populacao[i][j] = ProxGer[i][j];
             Populacao[i + 1][j] = ProxGer[i + 1][j];
          } //Fim (A matriz Populacao recebe a matriz ProxGeracao).
       } //Fim (Definindo a nova Populacao de cromossomos caso nao ocorra o crossover).
    } //Fim (Realiza o crossover dos cromossomos selecionados 2 a 2).
//-------------------------------------------------------------Fim da reproducao---------------------------------------------------------------//

//---------------------------------------------------Transformando a populacao binaria em real-------------------------------------------------//
   PopulacaoReal(Populacao, PopReal, min, max); //Gerando a nova matriz com numeros reais.
//-------------------------------------------------------------------Fim-----------------------------------------------------------------------//

//-----------------------------------------------------------Calculando a aptidao--------------------------------------------------------------//
   for(i = 0; i < TAMPOP; i++){ //Calcula a aptidao de cada cadeia de cromossomos.
      Apt[i] = Fitness(PopReal[i][0], PopReal[i][1]);
   }
   AptAdm(Apt); //Admencionaliza as Aptidoes.
//-------------------------------------------------------------------Fim-----------------------------------------------------------------------//

//---------------------------------------------------------Calculando o melhor cromossomo------------------------------------------------------//
    melhor = Best(Apt);
    if(Fitness(PopReal[melhor][0], PopReal[melhor][1]) < Fitness(x1, x2)){ //Avalia se a melhor solu��o da gera��o � melhor
       melhor1 = melhor;                                                   //que a guardadae a atualiza caso isso ocorra.
       for(i = 0; i < TAMCADEIA; i++){
          solucao[i] = Populacao[melhor1][i];
       }
       x1 = PopReal[melhor1][0];
       x2 = PopReal[melhor1][1];
    }
    fprintf(arquivo,"%10.6f %10.6f %10.6f\n", x1, x2, Fitness(x1, x2)); //Imprime a melhor solu��o at� o momento.
//    fprintf(arquivo,"%10.6f\n", Fitness(x1, x2)); //Imprime a melhor solu��o at� o momento.
//-------------------------------------------------------------------Fim-----------------------------------------------------------------------//

    ciclo++;
    }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Fim das Evolucoes++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//Imprimindo o resultado final.
    fprintf(arquivo,"\nResultado Final:\n");
    fprintf(arquivo,"%10.6f %10.6f %10.6f\n", x1, x2, Fitness(x1, x2));
}//-----------------------------------------------------Fim do Numero de corridas--------------------------------------------------------------//

tFim = clock(); //Finalizando e imprimindo o tempo de gasto para executar o programa
fprintf(arquivo,"\nTempo de execucao: %ld milisegundos.\n",tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC/1000)));

fclose(arquivo); //Fechando o arquivo onde os dados foram gravados
//getchar();
return (0);
}
//=======================================================Fim do Programa Principal=============================================================//

//============================================================Funcao Objetivo==================================================================//
//Defindo a Funcao Objetivo a ser usada no metodo.
double Fitness(double x1, double x2){
   double f;
   //f = pow(x1, 2) + pow(x2, 2); //Exemplo
   //f = -(sum(fabs(x))*exp(-sum(x**2)))
   //f = -((fabs(x1)+fabs(x2))*exp(-(pow(x1,2)+pow(x2,2))));
   //f = ((x1-1)*(x1-1))+((x2-1)*(x2-1))-x1*x2;  //Fun��o 1 do trabalho
   f = (x1*x1)+(3*(x2*x2))-2*x1+2*x2; //Fun��o 2 do trabalho
   return (f);
}
//=============================================================================================================================================//

//===========================================================Funcao CromoReal==================================================================//
//Funcao que converte a populacao "binaria" em pupolacao "real".
void PopulacaoReal(double Populacao[][TAMCADEIA], double PopReal [][QTDVAR], double min [], double max []){
   int i, j, n, aux, exp;
   double real, soma;

   for(i = 0; i < TAMPOP; i++){ //Convertendo os cromossomos binarios da populacao em cromossomos reais.
      aux = 0;
      for(n = 0; n < TAMCADEIA; n += TAMCROM){ //Transformando as vari�veis binarias em reais.
         exp = TAMCROM;
         soma = 0;
         for(j = n; j < (n + TAMCROM); j++){
            exp--;                             //Transformando cada gene do cromossomo em um
            soma += Populacao[i][j] * pow(2, exp); //numero real de acordo com sua posicao.
         }
         PopReal[i][aux] = min[aux] + ((max[aux] - min[aux]) / (pow(2, TAMCROM) - 1)) * soma; //Gerando a nova matriz com numeros reais.
         aux++;
      } //Fim (Transformando as vari�veis binarias em reais).
   } //Fim (Convertendo os cromossomos binarios da populacao em cromossomos reais).
}
//=============================================================================================================================================//

//============================================================Funcao Aptidao===================================================================//
//Funcao que calcula a aptidao de cada cromossomo.
void AptAdm(double Apt[]){
   int i;
   double somainvertperc=0, somaperc=0, somaapt=0, menor, maior, comp, somainvert=0;

/*
//Aptid�o calculada por meio do inverso do valor da fun��o objetivo (1 / f(x1, x2));
   for(i = 0; i < TAMPOP; i++){ //Calculando as aptid�es invertidas.
      Apt[i] = 1.0 / Apt[i];
      somaapt += Apt[i];
   }

   for(i = 0; i < TAMPOP; i++){
      Apt[i] = (Apt[i] / somaapt) * 100;
   }
*/

//Aptid�o calculada levando-se em conta o menor e o maior valor da fun��o objetivo.
   menor = -Apt[0];
   maior = -Apt[0];
   for(i = 1; i < TAMPOP; i++){ //Define a menor e a maior aptidao.
      if(-Apt[i] < menor){
         menor = -Apt[i]; //Armazena o valor da menor aptidao.
      }
      if(-Apt[i] > maior){
         maior = -Apt[i]; //Armazena o valor da maior aptidao.
      }
   }

   comp = maior - menor;
   for(i = 0; i < TAMPOP; i++){ //Admensionaliza as aptidoes.
      Apt[i] = (-Apt[i] - menor) / comp;
         somaapt+=Apt[i];
   }

   for(i = 0; i < TAMPOP; i++){
      Apt[i] = (Apt[i] / somaapt) * 100;
   }

}
//=============================================================================================================================================//

//============================================================Funcao Roleta====================================================================//
//Funcao que define os cromossomos selecionados para o crossover (reproducao).
int Roleta(double Apt[]){
   int sort = 0;
   double aux = 0, aleatorio = 0;

   aleatorio = ((double)rand() / (double)RAND_MAX) * 100; //Numero aleatorio entre [0, 1].
   aux = Apt[0];
   while(aux < aleatorio){ //Escolhe aleatoriamente um cromossomo para realizar o crossover.
      sort++;
      aux += Apt[sort];
   }
   return(sort); //Retorna a posicao do cromossomo sorteado.
}
//=============================================================================================================================================//

//===========================================================Funcao Mutacao====================================================================//
//Funcao que realiza a mutacao em um gene do cromossomo de acordo com a taxa de mutacao.
double Mutacao(double bit){
   double aleatorio = 0;

   aleatorio = ((double)rand() / (double)RAND_MAX); //Numero aleatorio entre [0, 1].
   if(aleatorio <= TXMT){ //Realiza a mutacao se o numero aleatorio gerado for menor ou igual a taxa de mutacao.
      if(bit == 1){
         return(0); //Retorna "0" se o gene for "1".
      }
      else{
         return(1); //Retorna "1" se o gene for "0".
      }
   }
   else{
      return(bit); //Retorna o valor de "gene" caso nao haja mutacao.
   }
}
//=============================================================================================================================================//

//============================================================Funcao Best======================================================================//
//Funcao que retorna o indice do melhor cromossomo.
int Best(double Apt[]){
    int i = 0, best = 0;
    double best_apt;

    best_apt = Apt[0];
    for(i = 1; i < TAMPOP; i++){ //Define a melhor cadeia de cromossomos (solucao).
       if(Apt[i] > best_apt){
          best_apt = Apt[i]; //Armazena o valor do melhor minimo da Funcao Fitness.
          best = i; //Posicao (na populacao) do melhor cromossomo.
       }
    } //Fim (Define a melhor cadeia de cromossomos (solucao)).

    return(best);
}
//=============================================================================================================================================//
