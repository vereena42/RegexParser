/*
	Copyright (c) 2015, Dominika Salawa <vereena42@gmail.com>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

		* Redistributions of source code must retain the above copyright notice,
		  this list of conditions and the following disclaimer.

		* Redistributions in binary form must reproduce the above copyright notice,
		  this list of conditions and the following disclaimer in the documentation
		  and/or other materials provided with the distribution.

		* Neither the name of the <organization> nor the names of its
		  contributors may be used to endorse or promote products derived from this
		  software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT,
	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include<cstdio>
#include<vector>
#include<stack>
#include<map>
#include<string>
#include<sstream>
#include<algorithm>
#include<set>
#include<queue>
#include<array>

using namespace std;

struct para{
	int pocz;
	int kon;
};
struct przejscia{
	int tab[26];
};
przejscia puste;
int najwiekszy;

vector<przejscia>V;
vector< vector<int> >Epsi;

vector<przejscia>bez_Epsi;
vector<bool>koncowy;

vector<bool>odwiedzony;

map< set<int>,int > mapa;

vector<int>koncowe;
stack<int>pozostale;

vector<przejscia>pierwsze;
vector<przejscia>drugie;

vector<bool>pierwsze_kon;
vector<bool>drugie_kon;
char litera(int i)
{
	return i+49 +'0';
}
int liczba(char i)
{
	return i-'0'-49;
}
int najwiekszy_bez_epsi;
bool pierwszy;


void pozbadz_sie_epsi(int S,int F,int n)
{
	stack<int>do_czyszczenia;
	//robimy tablicę 27 setów - przejścia po kolejnych literach
	set<int>temp[27];
	
	//zerujemy odwiedzone
	for(int i=0;i<=najwiekszy;i++)
		odwiedzony.push_back(false);
	
	set<int>SS;	
	//wrzucamy S na set
	SS.insert(S);
	//inicjalizujemy najwiekszy_bez_epsi =-1
	najwiekszy_bez_epsi=-1;
	//mapujemy set z S-em na najwiekszy_bez_epsi+1
	mapa[SS]=najwiekszy_bez_epsi+1;
	najwiekszy_bez_epsi++;

	//dodajemy puste przejscia do bez_Epsi
	bez_Epsi.push_back(puste);
	//dodajemy false do koncowy
	koncowy.push_back(false);
	//wrzucamy nasz set na kolejkę
	queue< set<int> >Q;
	queue<int>kolejka;
	Q.push(SS);
	int x;
	int v;
	while(!Q.empty())
	{
		//sciagamy set ze stosu
		SS=Q.front();
		Q.pop();
		v=mapa[SS];
		for(set<int>::iterator i=SS.begin();i!=SS.end();++i)
		{
			//wrzucamy na kolejkę wierzcholki kolejnego wierzcholka z bez_Epsi
			//jezeli ktorys byl koncowy, to wierzcholek z bez_Epsi też jest koncowy
			if(*i==F)
				koncowy[mapa[SS]]=true;
			odwiedzony[*i]=true;
			kolejka.push(*i);
			do_czyszczenia.push(*i);
		}
		while(!kolejka.empty())
		{
			x=kolejka.front();
			kolejka.pop();
			if(Epsi[x].empty())
			{
				//lecimy po literach z x i dodajemy do temp jak znajdziemy jakas
				for(int j=0;j<n;j++)
				{
					if(V[x].tab[j]!=-1)
					{
						temp[j].insert(V[x].tab[j]);
						break;
					}
				}
			}
			//jezeli ma epsi przejscia, to wrzucamy to gdzie mozna dojsc na kolejke
			for(int k=0;k<Epsi[x].size();k++)
			{
				if(odwiedzony[Epsi[x][k]]==false)
				{
					if(Epsi[x][k]==F)
						koncowy[mapa[SS]]=true;
					odwiedzony[Epsi[x][k]]=true;
					kolejka.push(Epsi[x][k]);
					do_czyszczenia.push(Epsi[x][k]);
				}
			}
		}
		//najpierw wyczyscmy tablice odwiedzonych
		while(!do_czyszczenia.empty())
		{
			odwiedzony[do_czyszczenia.top()]=false;
			do_czyszczenia.pop();
		}
		//teraz w kolejnych 27 komorkach temp mamy wierzcholki do ktorych mozna przejsc z SS po kolejnych literach
		//przejdzmy po nich
		for(int g=0;g<n;g++)
		{
			if(!temp[g].empty())
			{
				if(mapa.find(temp[g]) == mapa.end())
				{
					//mapujemy set na najwiekszy_bez_epsi+1
					mapa[temp[g]]=najwiekszy_bez_epsi+1;
					najwiekszy_bez_epsi++;

					//dodajemy puste przejscia do bez_Epsi
					bez_Epsi.push_back(puste);
					
					//dodajemy false do koncowy
					koncowy.push_back(false);
					
					//wrzucamy nasz set na kolejkę
					Q.push(temp[g]);
				}
				//ustawiamy bez_Epsi[SS].tab[g] na numer tego setu w mapie
				bez_Epsi[v].tab[g]=mapa[temp[g]];
				temp[g].clear();
			}
		}
	}
}

void minimalizuj(int n)
{
	vector< array < vector<int>, 27> > bez_Epsi_transp(najwiekszy_bez_epsi+2);
		
	//robimy graf transponowany
	for(int i=0;i<=najwiekszy_bez_epsi;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(bez_Epsi[i].tab[j]!=-1)
				bez_Epsi_transp[bez_Epsi[i].tab[j]][j].push_back(i);
			else
				bez_Epsi_transp[najwiekszy_bez_epsi+1][j].push_back(i);
		}
	}
	for(int j=0;j<n;j++)
		bez_Epsi_transp[najwiekszy_bez_epsi+1][j].push_back(najwiekszy_bez_epsi+1);
	//dynamicznie alokujemy potrzebna tablice stanów
	bool **T=new bool*[najwiekszy_bez_epsi+2];
	for(int i=0;i<=najwiekszy_bez_epsi+1;i++)
		T[i]=new bool[najwiekszy_bez_epsi+2];
	//zerujemy tablice
	for(int i=0;i<=najwiekszy_bez_epsi+1;i++)
	{
		for(int j=0;j<=najwiekszy_bez_epsi+1;j++)
		{
			T[i][j]=false;
			T[j][i]=false;
		}
	}
	for(int i=0;i<=najwiekszy_bez_epsi;i++)
	{
		if(koncowy[i]==true)
			koncowe.push_back(i);
		else
			pozostale.push(i);
	}
	pozostale.push(najwiekszy_bez_epsi+1);
	para x;
	int temp;
	queue<para>Q;
	//laczymy w pary poczatkowe i koncowe i wrzucamy na kolejke
	while(!pozostale.empty())
	{
		temp=pozostale.top();
		pozostale.pop();
		x.pocz=temp;
		for(int i=0;i<koncowe.size();i++)
		{
			x.kon=koncowe[i];
			T[x.kon][x.pocz]=true;
			T[x.pocz][x.kon]=true;
			Q.push(x);
		}
	}
	para temp2;
	//dla kazdej pary w kolejce
	while(!Q.empty())
	{
		x=Q.front();
		Q.pop();
		//dla kazdej litery sprawdzamy czy mozemy sie nie cofnac do jakiejs pary
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<bez_Epsi_transp[x.pocz][i].size();j++)
			{
				for(int k=0;k<bez_Epsi_transp[x.kon][i].size();k++)
				{
					if(bez_Epsi_transp[x.pocz][i][j]!=bez_Epsi_transp[x.kon][i][k])
					{
						temp2.pocz=bez_Epsi_transp[x.pocz][i][j];
						temp2.kon=bez_Epsi_transp[x.kon][i][k];
						if(T[temp2.pocz][temp2.kon]==false)
						{
							T[temp2.kon][temp2.pocz]=true;
							T[temp2.pocz][temp2.kon]=true;
							Q.push(temp2);
						}
					}
				}
			}
		}
	}
	int *nowy_numer=new int[najwiekszy_bez_epsi+2];
	for(int i=0;i<=najwiekszy_bez_epsi+1;i++)
	{
		nowy_numer[i]=-1;
	}
	int kolejny_numer=0;
	//liczymy dla 0
	for(int j=0;j<=najwiekszy_bez_epsi+1;j++)
	{
		if(T[0][j]==false)
			nowy_numer[j]=kolejny_numer;
	}
	//liczymy smietnik
	przejscia smietnik;
	for(int i=0;i<n;i++)
		smietnik.tab[i]=-1;
	kolejny_numer++;
	if(pierwszy)
	{
		pierwsze_kon.push_back(koncowy[0]);		
		pierwsze.push_back(bez_Epsi[0]);
		for(int i=0;i<pierwsze.size();i++)
		{
			for(int j=0;j<n;j++)
			{
				if(pierwsze[i].tab[j]==-1)
				{
					if(nowy_numer[najwiekszy_bez_epsi+1]==-1)
					{
						for(int g=0;g<=najwiekszy_bez_epsi+1;g++)
						{
							if(T[najwiekszy_bez_epsi+1][g]==false)
								nowy_numer[g]=kolejny_numer;
						}
						kolejny_numer++;
						pierwsze.push_back(smietnik);
						pierwsze_kon.push_back(false);
					}
					pierwsze[i].tab[j]=nowy_numer[najwiekszy_bez_epsi+1];	
				}
				else if(nowy_numer[pierwsze[i].tab[j]]==-1)
				{
					for(int g=0;g<=najwiekszy_bez_epsi+1;g++)
					{
						if(T[pierwsze[i].tab[j]][g]==false)
							nowy_numer[g]=kolejny_numer;
					}
					kolejny_numer++;
					pierwsze_kon.push_back(koncowy[pierwsze[i].tab[j]]);
					pierwsze.push_back(bez_Epsi[pierwsze[i].tab[j]]);
					pierwsze[i].tab[j]=nowy_numer[pierwsze[i].tab[j]];
				}
				else
					pierwsze[i].tab[j]=nowy_numer[pierwsze[i].tab[j]];
			}
		}
	}
	else
	{
		drugie_kon.push_back(koncowy[0]);
		drugie.push_back(bez_Epsi[0]);
		for(int i=0;i<drugie.size();i++)
		{
			for(int j=0;j<n;j++)
			{
				if(drugie[i].tab[j]==-1)
				{
					if(nowy_numer[najwiekszy_bez_epsi+1]==-1)
					{
						for(int g=0;g<=najwiekszy_bez_epsi+1;g++)
						{
							if(T[najwiekszy_bez_epsi+1][g]==false)
								nowy_numer[g]=kolejny_numer;
						}
						kolejny_numer++;
						drugie_kon.push_back(false);
						drugie.push_back(smietnik);
					}
					drugie[i].tab[j]=nowy_numer[najwiekszy_bez_epsi+1];	
				}
				else if(nowy_numer[drugie[i].tab[j]]==-1)
				{
					for(int g=0;g<=najwiekszy_bez_epsi+1;g++)
					{
						if(T[drugie[i].tab[j]][g]==false)
							nowy_numer[g]=kolejny_numer;
					}
					kolejny_numer++;
					drugie_kon.push_back(koncowy[drugie[i].tab[j]]);
					drugie.push_back(bez_Epsi[drugie[i].tab[j]]);
					drugie[i].tab[j]=nowy_numer[drugie[i].tab[j]];
				}
				else
					drugie[i].tab[j]=nowy_numer[drugie[i].tab[j]];
				if(drugie.size()<=pierwsze.size())
				{
					if(drugie[i].tab[j]!=pierwsze[i].tab[j])
					{
						printf("NIE\n");
						goto uwolnij_pamiec;
					}
					else if(pierwsze_kon[i]!=drugie_kon[i])
					{
						printf("NIE\n");
						goto uwolnij_pamiec;
					}
				}
				else
				{
					printf("NIE\n");
					goto uwolnij_pamiec;
				}
			}
		}
		printf("TAK\n");
	}
	uwolnij_pamiec:
	koncowe.clear();
	//usuwamy zaalokowana pamiec
	for(int i=0;i<=najwiekszy_bez_epsi+1;i++)
		delete [] T[i];
	delete [] T;
	delete [] nowy_numer;
	T=NULL;
}

int main()
{
	for(int i=0;i<26;i++)
			puste.tab[i]=-1;
	int n;
	scanf("%d",&n);
	int z;
	scanf("%d",&z);
	while(z--)
	{
		pierwszy=true;
		vector<int>nic;
		stack<para>Q;
		bez_Epsi.clear();
		koncowy.clear();
		odwiedzony.clear();
		mapa.clear();
		V.clear();
		Epsi.clear();
		najwiekszy=-1;
		char c;
		scanf("%c",&c);
		while(c==' ' || c=='\n')
		{
			scanf("%c",&c);
		}
		para a,b,x;
		while(c!='\n' && c!=' ')
		{
			if(c=='.')
			{
				a=Q.top();
				Q.pop();
				b=Q.top();
				Q.pop();
				x.pocz=b.pocz;
				Epsi[b.kon].push_back(a.pocz);
				x.kon=a.kon;
				Q.push(x);
			}
			else if(c=='+')
			{
				a=Q.top();
				Q.pop();
				b=Q.top();
				Q.pop();
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(b.pocz);
				Epsi[najwiekszy+1].push_back(a.pocz);
				Epsi[b.kon].push_back(najwiekszy+2);
				Epsi[a.kon].push_back(najwiekszy+2);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else if(c=='*')
			{
				a=Q.top();
				Q.pop();
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+1;
				
				Epsi.push_back(nic);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(a.pocz);
				Epsi[a.kon].push_back(najwiekszy+1);
				
				najwiekszy++;
				Q.push(x);
			}
			else if(c=='0')
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else if(c=='1')
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(najwiekszy+2);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				V[najwiekszy+1].tab[c-'0'-49]=najwiekszy+2;
				
				najwiekszy+=2;
				Q.push(x);
			}
			scanf("%c",&c);
		}
		pozbadz_sie_epsi(Q.top().pocz,Q.top().kon,n);	
		
		
		minimalizuj(n);
			
		while(!Q.empty())
			Q.pop();
		
		pierwszy=false;
		bez_Epsi.clear();
		koncowy.clear();
		odwiedzony.clear();
		mapa.clear();
		V.clear();
		Epsi.clear();
		najwiekszy=-1;
		while(c==' ' || c=='\n')
		{
			scanf("%c",&c);
		}
		
		
		while(c!='\n' && c!=' ')
		{
			if(c=='.')
			{
				a=Q.top();
				Q.pop();
				b=Q.top();
				Q.pop();
				x.pocz=b.pocz;
				Epsi[b.kon].push_back(a.pocz);
				x.kon=a.kon;
				Q.push(x);
			}
			else if(c=='+')
			{
				a=Q.top();
				Q.pop();
				b=Q.top();
				Q.pop();
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(b.pocz);
				Epsi[najwiekszy+1].push_back(a.pocz);
				Epsi[b.kon].push_back(najwiekszy+2);
				Epsi[a.kon].push_back(najwiekszy+2);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else if(c=='*')
			{
				a=Q.top();
				Q.pop();
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+1;
				
				Epsi.push_back(nic);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(a.pocz);
				Epsi[a.kon].push_back(najwiekszy+1);
				
				najwiekszy++;
				Q.push(x);
			}
			else if(c=='0')
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else if(c=='1')
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				Epsi[najwiekszy+1].push_back(najwiekszy+2);
				
				najwiekszy+=2;
				Q.push(x);
			}
			else
			{
				x.pocz=najwiekszy+1;
				x.kon=najwiekszy+2;
				
				Epsi.push_back(nic);
				Epsi.push_back(nic);
				V.push_back(puste);
				V.push_back(puste);
				
				V[najwiekszy+1].tab[c-'0'-49]=najwiekszy+2;
				
				najwiekszy+=2;
				Q.push(x);
			}
			scanf("%c",&c);
		}
		pozbadz_sie_epsi(Q.top().pocz,Q.top().kon,n);
		
		minimalizuj(n);
		pierwsze.clear();
		drugie.clear();
		pierwsze_kon.clear();
		drugie_kon.clear();
	}
	return 0;
}
