# -*- coding: utf-8 -*-
"""
Created on Fri Aug 31 23:31:30 2012

@author: leonard
Regras e convençoes de código: http://www.python.org/dev/peps/pep-0008/
"""
#import AGfuncoes
import numpy
import random
class AG:
    """ Algoritmos genéticos """
    
    def __init__(self,pMin=1,tamPop=6,txCr=2,txMut=0.1,numGer=4):
        self.pMin=pMin              #precisao mínima (número inteiro indicando a quantidade de casas decimais requeridaa)  
        self.tamPop=tamPop          #tamanho da população
        self.txCr=txCr              #taxa de cruzamento
        self.txMut=txMut            #taxa de mutação (em percentual)
        self.numGer=numGer          #número de gerações
        self.set_Dimensao()
        self.set_Intervalo(self.dimensao)        
        self.set_Amplitude(self.dimensao,self.intervalo)
        self.set_NumeroDePontos(self.amplitude,self.pMin,self.dimensao)
        self.set_TamCromossomo(self.NP,self.dimensao)
        self.set_Cadeia(self.tamCrom,self.dimensao)
        self.set_PopulacaoInicial(self.tamPop,self.cadeia)
        
    #def f1(x):                   #funcão 1 proposta
    #    return ((x[0]-1)**2+(x[1]-1)**2-x[0]*x[1])
    #def f2(x):                  # funcao 2 proposta
    #    return (x[0]**2+3*x[1]**2-2*x[0]-2*x[1])
    
    def set_Dimensao(self):
        """ Configura dimensão de acordo com a função proposta """        
        self.dimensao=2;
    
    def get_Dimensao(self):
        """ Retorna a dimensao do problema """        
        return self.dimensao
    
    def set_Intervalo(self,dimen):
        """ Configura o intervalo de busca para as variáveis do problema (funcao). Depende da dimensao do problema (quantidade de variáveis) """  
        _vInt=[]                        #cria lista vazia
        for i in range(dimen):      #cria lista de arrays com valores nulos de tamanho dimen
            _vInt.append(numpy.zeros(dimen)) 
        #Ainda fixo para duas dimensoes
        _vInt[0][0]=-2
        _vInt[0][1]=1.5
        _vInt[1][0]=-0.5
        _vInt[1][1]=3
        self.intervalo=_vInt
    
    def get_Intervalo(self):
        return self.intervalo
    
    def Funcao(self,x):
        #x é um array de tamanho dimen ( ou cromossomo)
        self.f=((x[0]-1)**2+(x[1]-1)**2-x[0]*x[1])    

    def set_Amplitude(self,dimen,vIntB):
        """ obtem amplitude do intervalo em relação aos intervalos de busca de acordo com a dimensao da função fornecida """        
        # dimen  - dimensao do problema
        # vIntB - vetor com Intervalo de Busca
        _vAmp=[]        
        for i in range(dimen):      #cria lista de arrays com valores nulos de tamanho dimen
            _vAmp.append(sum(numpy.fabs(vIntB[i]))) 
        self.amplitude=_vAmp
        
    def get_Amplitude(self):
        """ Retorna a amplitude do intervalo em relação aos intervalos de busca de acordo com a dimensao da função fornecida """                
        return self.amplitude
    
    def set_NumeroDePontos(self,a,p,dimen,base=10):
        """ calcula o número de pontos na base (base) em funcao da amplitude (a), precisão (p) """        
        _vNP=[]
        for i in range(dimen):
            _vNP.append((a[i]*(base**p)))
        self.NP=_vNP
    
    def get_NumeroDePontos(self):        
        """ Retorna o número de pontos na base calculado em SetNumeroDePontos """                
        return self.NP
        
    def CalcBits(self,quociente):
        """ retorna a quantidade necessária de bits para representar um número decimal """        
        _qtdBit=1
        while quociente!=1:
            quociente=int(quociente)/2
            _qtdBit=_qtdBit+1
        return _qtdBit
        
    def set_TamCromossomo(self,valor,dimen):
        """ Escolhe o tamanho do cromossomo, em função do número de pontos(valor) calculado no método setNumeroDePontos """
        _vTC=[] #vetor tamanho do cromossomo em função da dimensão
        for i in range(dimen):        
            bits=self.CalcBits(valor[i])    
            _vTC.append(bits)
        self.tamCrom=max(_vTC) #retorna a maior quantidade necessária para representar o maior número
    
    def get_TamCromossomo(self):       
        """ Escolhe o tamanho do cromossomo, calculado em setTamCromossomo """        
        return self.tamCrom
       
    def set_Cadeia(self,tC,dimen):
        """  Configura a cadeia do cromossomo. Requisitos:
            - Tamanho do cromossomo (tC)
            - dimensao (dimen)
        """
        self.cadeia=tC*dimen
    
    def get_Cadeia(self):
        return self.cadeia
        
    def set_PopulacaoInicial(self,tP,tC):
        """ Inicia uma populacao inicial. Necessita das seguintes informações:
        -tamanho da população (tP)        
        -tamanho da cadeia (tC)
        """
        _vPopIni=randint(0,2,tP*tC)
        _vPopIni.shape=(tP,tC)
        self.PopIni=_vPopIni
        
    def get_PopulacaoInicial(self):
        return self.PopIni
    
    def CalcIntervalos(self,dimen,tCrom):
        """ Calcula os intervalos em que a cadeia será desmembrada. Relação com a dimensão (dimen) do problema e o tamanho do cromossomo (tCrom) """
        _vInt=[]        
        fim=-1
        for i in range(dimen):        
            ini=fim+1
            fim=ini+(tCrom-1)
            #_vAptidao=self.PopIni[]
            _vInt.append(ini)
            _vInt.append(fim)
        return _vInt
    
    def ConvBinToDec(self,num,tCrom):
        """ Converte número binário para decimal de acordo com tamanho do cromossomo(tC) 
            - o Número (num) é um array ou lista de tamanha tC, onde cada posição é armazenado somente um bit"""
        _dec=0        
        for k in range(tCrom):
            _dec=_dec+num[k]*(2**k)
        return _dec
        
    def ConverterCromossomo(self,dimen,tCrom,tC):
        """ converter cromossomo de uma base binária para uma base decimal 
            - Dimensão (dimen)
            - Tamanho do cromossomo (tCrom)
            - Tamanho da cadeia (tC)
        """
        #_vInt=self.CalcIntervalos(dimen,tCrom)
        _vAptidao=[]
        for i in range(self.tamPop):
            ini,fim=0,-1            
            for j in range(dimen):        
                ini=fim+1
                fim=ini+(tCrom-1)
                _vAptidao.append(self.ConvBinToDec(self.PopIni[i][ini:fim+1],self.tamCrom))
        _vApt=array([_vAptidao]) #convertendo em array para melhor visualização dos cromossomos
        _vApt.shape=(self.tamPop,self.dimensao)
        print _vApt
        
        #definição dos intervalos
        #nMax=2**self.tamCrom #maior número decimal obtido de acordo com o tamanho em bits do cromossomo
            
ag=AG()