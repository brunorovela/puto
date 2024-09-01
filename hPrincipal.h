#include <allegro.h>
#include <fstream>
#include <iostream>

using namespace std;

static volatile long int fps = 0;
static volatile long int contador = 0;
static int mostra_fps = 0;
static int conta_fps = 0;

void Inicia_Estruturas();
void MexeDados();
void JogaDados(BITMAP *);
void MostraJogadores(BITMAP *);
void Reinicia_Jogo();
void GravaArquivo();
void RecuperaArquivo();

static bool GameOver = false;
static bool bJogador = true;
static bool bJogouDados = false;
static bool bMarcador = false;
static bool bFimPartida = false;
static int iPontoVisual = 0;
static int iCPU = 1;
static int iMaxPontos = 5000;
static int Vencedor_Anterior =- 0;
static int iAuxVencedor = 0;

//Mensagens
static char cJogador[100];
static char Cromos[100];
static char Necros[100];
static char Morgan[100];
static char Parplo[100];

static DATAFILE *imgDados = NULL;
static DATAFILE *imgFontes = NULL;

static int iVelCPU = 15;

//	Essa struct armazena o valor de 5 dados. 
struct strDados
{
	int iNumero;
	int bDisponivel;
} Dados[5];

struct strJocador
{
	bool bLista;
	int iPontos;
	int iAcumulado;
	int iVitoria;
	int iNivel;
} Jogador;

struct strCPU
{
	bool bLista;
	int id;
	int iPontos;
	int iAcumulado;
	int iVitoria;
	int iNivel;
} CPU[5];

struct strPote
{
	BITMAP *imgPote;
	int iPosx;
	int iPosy;
	int iAngulo;
} Pote;

void Inicia_Estruturas()
{
	Vencedor_Anterior=-1;
	iAuxVencedor=0;

	bJogador = true;
	bJogouDados = false;
	bMarcador = false;
	bFimPartida = false;
	iCPU = 1;
	iPontoVisual = 0;

	Dados[0].iNumero = 0;
	Dados[1].iNumero = 0;
	Dados[2].iNumero = 0;
	Dados[3].iNumero = 0;
	Dados[4].iNumero = 0;
	Dados[0].bDisponivel = true;
	Dados[1].bDisponivel = true;
	Dados[2].bDisponivel = true;
	Dados[3].bDisponivel = true;
	Dados[4].bDisponivel = true;

	Jogador.iPontos = 0;
	Jogador.iAcumulado = 0;
	Jogador.bLista = false;
	Jogador.iVitoria= 0;
	Jogador.iNivel = 0;

	CPU[0].iNivel = 0;
	CPU[1].iNivel = 0;
	CPU[2].iNivel = 0;
	CPU[3].iNivel = 0;
	CPU[4].iNivel = 0;
	CPU[0].iVitoria = 0;
	CPU[1].iVitoria = 0;
	CPU[2].iVitoria = 0;
	CPU[3].iVitoria = 0;
	CPU[4].iVitoria = 0;
	CPU[0].id = 0;
	CPU[1].id = 1; // Cromo
	CPU[2].id = 2; // Necros
	CPU[3].id = 3; // Morgan
	CPU[4].id = 4; // Parblo
	CPU[0].bLista = false;
	CPU[1].bLista = false;
	CPU[2].bLista = false;
	CPU[3].bLista = false;
	CPU[4].bLista = false;
	CPU[0].iPontos = 0;
	CPU[1].iPontos = 0;
	CPU[2].iPontos = 0;
	CPU[3].iPontos = 0;
	CPU[4].iPontos = 0;
	CPU[0].iAcumulado = 0;
	CPU[1].iAcumulado = 0;
	CPU[2].iAcumulado = 0;
	CPU[3].iAcumulado = 0;
	CPU[4].iAcumulado = 0;
	
	Pote.imgPote = (BITMAP *)imgDados[8].dat;
	Pote.iPosx = 600;
	Pote.iPosy = 350;
	Pote.iAngulo = 0;
}

void MexeDados()
{
	int i=0;

	bJogouDados = false;

	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			Dados[i].iNumero = 1+(rand() % 6);
		}			
	}
}

void JogaDados(BITMAP *Buffer)
{
	int i = 0;
	
	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			draw_sprite(Buffer,(BITMAP *)imgDados[Dados[i].iNumero].dat,300+40*i,365);	
		} else textprintf_ex(Buffer,font,308+40*i,378,makecol(255,0,255),-1,"OK");
		
		if (!Dados[i].bDisponivel)
		{
			draw_sprite(Buffer,(BITMAP *)imgDados[Dados[i].iNumero].dat,300+40*i,401);	
		}
	}
}

void MostraJogadores(BITMAP *Buffer)
{
	int i = 0;
	int iCorte = 0;

	textprintf_centre_ex(Buffer, font,  80+32, 110,makecol(176,226,255), -1,"Jogador");
	textprintf_centre_ex(Buffer, font,  220+32, 110,makecol(193,205,205), -1,"Cromos");
	textprintf_centre_ex(Buffer, font,  360+32, 110,makecol(193,205,205), -1,"Necros");
	textprintf_centre_ex(Buffer, font,  500+32, 110,makecol(193,205,205), -1,"Morgan");
	textprintf_centre_ex(Buffer, font,  640+32, 110,makecol(193,205,205), -1,"Parplo");

	for (i=0; i<5; i++)
	{
		masked_blit((BITMAP *)imgDados[9].dat,Buffer,(i*64),0,80+(i*140),120,64,64);
		
		if ( i>0 )
		{
			textprintf_ex(Buffer, font,78+(i*140), 194,makecol(0, 255, 255), -1,"Acumulado");
			textprintf_ex(Buffer, font,78+(i*140), 204,makecol(255, 255, 0), -1,"%d",CPU[i].iAcumulado);
			textprintf_ex(Buffer, font,78+(i*140), 214,makecol(0, 255, 255), -1,"Pontos");
			textprintf_ex(Buffer, font,78+(i*140), 224,makecol(255, 255, 0), -1,"%d",CPU[i].iPontos);
			textprintf_ex(Buffer, font,78+(i*140), 234,makecol(0, 255, 255), -1,"Vitorias");
			textprintf_ex(Buffer, font,78+(i*140), 244,makecol(255, 255, 0), -1,"%d / %d",CPU[i].iVitoria,CPU[i].iNivel);
			
			//Medalhas
			if (CPU[i].iNivel > 0)
			{
				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,(78+(i*140))+iCorte*22,254,22,28); // MEDALHAS
				iCorte++;
			}
			if (CPU[i].iNivel > 1)
			{
				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,(78+(i*140))+iCorte*22,254,22,28); // MEDALHA
				iCorte++;
			}
			if (CPU[i].iNivel > 2)
			{
   				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,(78+(i*140))+iCorte*22,254,22,28); // MEDALHAS
   			}
			iCorte=0;
			
			//Ouro
			if (CPU[i].iNivel > 3)
			{
				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,(83+(i*140))+iCorte*18,282,18,17); // OURO
				iCorte++;
			}
			if (CPU[i].iNivel > 4)
			{
				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,(83+(i*140))+iCorte*18,282,18,17); // OURO
				iCorte++;
			}
			if (CPU[i].iNivel > 5)
			{
   				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,(83+(i*140))+iCorte*18,282,18,17); // OURO
   			}
			iCorte=0;

			//Trofeu
			if (CPU[i].iNivel > 6)
				masked_blit((BITMAP *)imgDados[14].dat,Buffer,0,0,(101+(i*140)),300,18,20); // Trofeu
		}
	}

		//Medalhas
			if (Jogador.iNivel > 0)
			{
				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,75+(iCorte*22),254,22,28); // MEDALHAS
				iCorte++;
			}
			if (Jogador.iNivel > 1)
			{
				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,75+(iCorte*22),254,22,28); // MEDALHA
				iCorte++;
			}
			if (Jogador.iNivel > 2)
			{
   				masked_blit((BITMAP*)imgDados[12].dat,Buffer,iCorte*22,0,75+(iCorte*22),254,22,28); // MEDALHAS
   			}
			iCorte=0;
			
			//Ouro
			if (Jogador.iNivel > 3)
			{
				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,80+(iCorte*18),282,18,17); // OURO
				iCorte++;
			}
			if (Jogador.iNivel > 4)
			{
				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,80+(iCorte*18),282,18,17); // OURO
				iCorte++;
			}
			if (Jogador.iNivel > 5)
			{
   				masked_blit((BITMAP *)imgDados[13].dat,Buffer,iCorte*18,0,80+(iCorte*18),282,18,17); // OURO
   			}
			iCorte=0;

			//Trofeu
			if (Jogador.iNivel > 6)
				masked_blit((BITMAP *)imgDados[14].dat,Buffer,0,0,98,300,18,20); // Trofeu
				
		textprintf_ex(Buffer, font,75,194,makecol(0, 255, 255), -1,"Acumulado");
		textprintf_ex(Buffer, font,75,204,makecol(255, 255, 0), -1,"%d",Jogador.iAcumulado);
		textprintf_ex(Buffer, font,75,214,makecol(0, 255, 255), -1,"Pontos");
		textprintf_ex(Buffer, font,75,224,makecol(255, 255, 0), -1,"%d",Jogador.iPontos);
		textprintf_ex(Buffer, font,75,234,makecol(0, 255, 255), -1,"Vitorias");
		textprintf_ex(Buffer, font,75,244,makecol(255, 255, 0), -1,"%d / %d",Jogador.iVitoria,Jogador.iNivel);
}

void Reinicia_Jogo()
{
	bJogador = true;
	bJogouDados = false;
	bMarcador = false;
	bFimPartida = false;
	iCPU = 1;
	iPontoVisual=0;

	strcpy (Cromos,"");
	strcpy (Necros,"");
	strcpy (Morgan,"");
	strcpy (Parplo,"");
	strcpy (cJogador,"");
	Dados[0].bDisponivel = true;
	Dados[1].bDisponivel = true;
	Dados[2].bDisponivel = true;
	Dados[3].bDisponivel = true;
	Dados[4].bDisponivel = true;

	Jogador.iPontos = 0;
	Jogador.iAcumulado = 0;
	Jogador.bLista = false;

	CPU[0].id = 0;
	CPU[1].id = 1; // Cromo
	CPU[2].id = 2; // Necros
	CPU[3].id = 3; // Morgan
	CPU[4].id = 4; // Parblo
	CPU[0].bLista = false;
	CPU[1].bLista = false;
	CPU[2].bLista = false;
	CPU[3].bLista = false;
	CPU[4].bLista = false;
	CPU[0].iPontos = 0;
	CPU[1].iPontos = 0;
	CPU[2].iPontos = 0;
	CPU[3].iPontos = 0;
	CPU[4].iPontos = 0;
	CPU[0].iAcumulado = 0;
	CPU[1].iAcumulado = 0;
	CPU[2].iAcumulado = 0;
	CPU[3].iAcumulado = 0;
	CPU[4].iAcumulado = 0;
	
	Pote.iPosx = 600;
	Pote.iPosy = 350;
	Pote.iAngulo = 0;
}

void GravaArquivo()
{
	ofstream AbreArquivo("Jogo.dat", ios::out | ios::binary);	 

	AbreArquivo.write((char *) &Jogador, sizeof Jogador); 
	AbreArquivo.write((char *) &CPU, sizeof CPU); 

	AbreArquivo.close(); 
}

void RecuperaArquivo()
{
	ifstream LendoArquivo("Jogo.dat", ios::in | ios::binary);
	if (LendoArquivo)
	{
		LendoArquivo.read((char *) &Jogador, sizeof Jogador); 
		LendoArquivo.read((char *) &CPU, sizeof CPU);
	
		LendoArquivo.close();
	}
}

