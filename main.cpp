#include <allegro.h>
#include "hPrincipal.h"
#include <iostream>
#include <time.h>

void Mostra_Abertura(BITMAP *);
void Mostra_Ajuda(BITMAP *);
void DesabilitaDado(strDados [], int, int);
void RecolocaDados();
void DesenhaPote(BITMAP *, strPote);
void Mostra_Msg(BITMAP *);
void Reseta_Vitorias();

void PassaVez();
void Controles();
int ContaPontos();
int ContaDados();

bool VerificaDados();

void cont_fps()
{
	fps++;
	conta_fps++;
}
END_OF_FUNCTION(cont_fps);

void tempo()
{
	contador++;
}
END_OF_FUNCTION(tempo);

int main()
{
	allegro_init();
	install_keyboard();
	LOCK_VARIABLE(fps);
   	LOCK_FUNCTION(cont_fps);
   	install_int_ex(cont_fps, BPS_TO_TIMER(60));
   	LOCK_VARIABLE(contador);
   	LOCK_FUNCTION(tempo);
   	install_int(tempo, 100);
   	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);

	imgDados = NULL;
	imgDados = load_datafile("Dados.dat");
	
	imgFontes = NULL;
	imgFontes = load_datafile("Fontes.dat");

	BITMAP *Buffer = NULL;
	Buffer = create_bitmap(SCREEN_W,SCREEN_H);
	clear_bitmap(Buffer);

	Inicia_Estruturas();
	Mostra_Abertura(Buffer);
	
	srand( time(NULL) );

	MexeDados();

	int i = 0;
	int iTotal = 0;
	int iCPURnd = 0;
	int Buffer_CPU = 0;

	RecuperaArquivo();
	
	while(!GameOver)
	{
		while (fps > 0)
		{
			clear_bitmap(Buffer);
			blit((BITMAP *)imgDados[7].dat,Buffer,0,0,0,0,SCREEN_W,SCREEN_H);
			MostraJogadores(Buffer);

			textprintf_ex(Buffer,font,570,530,makecol(176,200,255),-1,"ESC - Sai do Jogo");
			textprintf_ex(Buffer,font,570,540,makecol(176,200,255),-1,"F1  - Aprenda a jogar");
			textprintf_ex(Buffer,font,570,550,makecol(176,200,255),-1,"F2  - Recomece o jogo");
			textprintf_ex(Buffer,font,570,560,makecol(176,200,255),-1,"F12 - Reseta Vitorias");
			textprintf_ex(Buffer,font,570,570,makecol(176,200,255),-1,"E   - Mexe o Pote");
			textprintf_ex(Buffer,font,570,580,makecol(176,200,255),-1,"R   - Joga o pote na mesa");
			textprintf_ex(Buffer,font,570,590,makecol(176,200,255),-1,"T   - Marca o Ponto");
 
			DesenhaPote(Buffer,Pote);
			textout_right_ex(Buffer, (FONT *)imgFontes[2].dat, "Jogador:", 70, 450, makecol(176,226,255),-1);
			textout_right_ex(Buffer, (FONT *)imgFontes[2].dat, "Cromos:", 70, 465, makecol(176,226,255),-1);
			textout_right_ex(Buffer, (FONT *)imgFontes[2].dat, "Necros:", 70, 480, makecol(176,226,255),-1);
			textout_right_ex(Buffer, (FONT *)imgFontes[2].dat, "Morgan:", 70, 495, makecol(176,226,255),-1);
			textout_right_ex(Buffer, (FONT *)imgFontes[2].dat, "Parplo:", 70, 510, makecol(176,226,255),-1);
			textprintf_ex(Buffer,(FONT *)imgFontes[1].dat,74,450,makecol(176,200,255),-1,cJogador);
			textprintf_ex(Buffer,(FONT *)imgFontes[1].dat,74,465,makecol(238,180,34),-1,Cromos);
			textprintf_ex(Buffer,(FONT *)imgFontes[1].dat,74,480,makecol(238,180,34),-1,Necros);
			textprintf_ex(Buffer,(FONT *)imgFontes[1].dat,74,495,makecol(238,180,34),-1,Morgan);
			textprintf_ex(Buffer,(FONT *)imgFontes[1].dat,74,510,makecol(238,180,34),-1,Parplo);

			if (!bFimPartida)
			{
   				if (bJogador)
				{//15
					textprintf_ex(Buffer,(FONT *)imgFontes[0].dat,0,560,makecol(176,226,255),-1,"Sua vez de jogar");
					rect(Buffer, 75, 119, 75+70, 119+71, makecol(255,255,0));

					if (bJogouDados)
					{//14
						// se o jogador não estiver na lista faça
						if (!Jogador.bLista)
						{//1
							iTotal = ContaPontos(); // conta pontos
							if (iTotal > 0) // se marcou pontos
							{//2
								iPontoVisual = iTotal;
								Jogador.iPontos += iTotal;
								iTotal = 0;
								bJogouDados=false;
								if (VerificaDados())
								{//3
									RecolocaDados();
								}//3
							}//2
    	  					else
							{//4
								PassaVez();
							}//4
							if (Jogador.iPontos >= 750)
							{//5
								bMarcador = true;
								strcpy (cJogador,"Acumule pontos apertando T ou arrisque TUDO.");
							}//5
       						else if (Jogador.iPontos > 0)
             				{
             					strcpy (cJogador,"Continue tentando 750 pontos jogando  outra vez.");
             				}
	 
						}//1
    	 				else // se já estiver na lista faça
     					{//13
     						iTotal = ContaPontos(); // conta pontos
							if (iTotal > 0) // se marcou pontos
							{//11
								if (Jogador.iAcumulado < iMaxPontos)
								{//10
       								strcpy (cJogador,"Acumule pontos apertando T ou arrisque TUDO.");
       								bMarcador = true;
									iPontoVisual = iTotal;
									Jogador.iPontos += iTotal;
									iTotal = 0;
									bJogouDados=false;
									if (VerificaDados())
									{//6
										RecolocaDados();
									}//6

									if (Jogador.iAcumulado + Jogador.iPontos == iMaxPontos)
									{//7
										Jogador.iAcumulado += Jogador.iPontos;
										Jogador.iPontos = 0;
										bFimPartida = true;
										Jogador.iVitoria++;
										strcpy (cJogador,"Parabens, Voce e o REI dos PUTO.");

										if (Jogador.iNivel < 8) 
          								{
											if (Vencedor_Anterior == 0)
											{
												iAuxVencedor++;
												if (iAuxVencedor >= 2)
												{
													iAuxVencedor = 0;
													Jogador.iNivel++;
													Mostra_Msg(Buffer);
												}
											}
											else
											{
												Vencedor_Anterior = 0;
												iAuxVencedor++;
											}
										}
										
									}//7
									else
									{//8
										if (Jogador.iAcumulado + Jogador.iPontos > iMaxPontos)
										{//9
											strcpy (cJogador,"Marcou mais do que devia.");
											PassaVez();
										}//9
									}//8
								}//10
							}//11
							else
							{//12
								PassaVez();
							}//12
	     				}//13
					}//14
				}//15
				else // CPU CPU CPU CPU CPU CPU CPU CPU CPU CPU
				{
					textprintf_ex(Buffer,(FONT *)imgFontes[0].dat,0,560,makecol(193,205,205),-1,"Vez da Maquina");
					rect(Buffer, 76+(CPU[iCPU].id*140), 119, 76+(CPU[iCPU].id*140)+70, 119+71, makecol(255,255,0));

   					if (Buffer_CPU == 0 && !bFimPartida)
   					{
   						Buffer_CPU=iVelCPU;

						iCPURnd = 1+rand() %10;
		
						for (i=1;i<iCPURnd;i++)
						{//1
							MexeDados();
						}//1
			
						bJogouDados = true;
			
						/* fazer cpu jogar dados*/
						if (bJogouDados)
						{//7
							// se o jogador não estiver na lista faça
							if (!CPU[iCPU].bLista)
							{
								iTotal = ContaPontos(); // conta pontos
								if (iTotal > 0) // se marcou pontos
								{//5
									iPontoVisual = iTotal;
									CPU[iCPU].iPontos += iTotal;
									iTotal = 0;
									bJogouDados=false;

									if (VerificaDados())
									{//2
										RecolocaDados();
									}//2
									if (CPU[iCPU].iPontos >= 750 && !CPU[iCPU].bLista)
									{//3
										CPU[iCPU].bLista = true;
										CPU[iCPU].iAcumulado += CPU[iCPU].iPontos;
										if (CPU[iCPU].id == 1) strcpy (Cromos,"Entrou na partida.");
										if (CPU[iCPU].id == 2) strcpy (Necros,"Entrou na partida.");
										if (CPU[iCPU].id == 3) strcpy (Morgan,"Entrou na partida.");
										if (CPU[iCPU].id == 4) strcpy (Parplo,"Entrou na partida.");
								
										PassaVez();
									}//3
									else
									{//4
										if (CPU[iCPU].id == 1) strcpy (Cromos,"Precisa fazer 750 pontos ou mais para entrar na partida.");
										if (CPU[iCPU].id == 2) strcpy (Necros,"Precisa fazer 750 pontos ou mais para entrar na partida.");
										if (CPU[iCPU].id == 3) strcpy (Morgan,"Precisa fazer 750 pontos ou mais para entrar na partida.");
										if (CPU[iCPU].id == 4) strcpy (Parplo,"Precisa fazer 750 pontos ou mais para entrar na partida.");
									}//4
								}//5
        						else
								{//6
									if (CPU[iCPU].id == 1) strcpy (Cromos,"Nao entrou na partida.");
									if (CPU[iCPU].id == 2) strcpy (Necros,"Nao entrou na partida.");
									if (CPU[iCPU].id == 3) strcpy (Morgan,"Nao entrou na partida.");
									if (CPU[iCPU].id == 4) strcpy (Parplo,"Nao entrou na partida.");
									PassaVez();
       							}//6
							}//7
							else // Se o CPU já estiver na lista faça
							{
								iTotal = ContaPontos(); // conta pontos
								if (iTotal > 0) // se marcou pontos
								{
									if (CPU[iCPU].iAcumulado < iMaxPontos)
									{
										iPontoVisual = iTotal;
										CPU[iCPU].iPontos += iTotal;
										iTotal = 0;
										bJogouDados=false;
										if (VerificaDados()) // verifica se todos os dados foram jogados
										{//
      										RecolocaDados();
      									}//

										if (CPU[iCPU].iAcumulado + CPU[iCPU].iPontos < iMaxPontos)
										{//
											if (ContaDados() < 3) // verifica quantos dados tenho disponiveis para jogar
											{//
							 					if (CPU[iCPU].iPontos >= 150)
												{
													if (CPU[iCPU].id == 1) strcpy (Cromos,"Marcou ponto.");
													if (CPU[iCPU].id == 2) strcpy (Necros,"Marcou ponto.");
													if (CPU[iCPU].id == 3) strcpy (Morgan,"Marcou ponto.");
													if (CPU[iCPU].id == 4) strcpy (Parplo,"Marcou ponto.");
													CPU[iCPU].iAcumulado += CPU[iCPU].iPontos;
													PassaVez();
												}
											}else if (CPU[iCPU].iPontos >= 200)
											{
												if (CPU[iCPU].id == 1) strcpy (Cromos,"Marcou ponto.");
												if (CPU[iCPU].id == 2) strcpy (Necros,"Marcou ponto.");
												if (CPU[iCPU].id == 3) strcpy (Morgan,"Marcou ponto.");
												if (CPU[iCPU].id == 4) strcpy (Parplo,"Marcou ponto.");
												CPU[iCPU].iAcumulado += CPU[iCPU].iPontos;
												PassaVez();
											}
										}else
         								if (CPU[iCPU].iAcumulado + CPU[iCPU].iPontos == iMaxPontos) // Marca pontos pequenos para CPU
										{//
											CPU[iCPU].iAcumulado += CPU[iCPU].iPontos;
											CPU[iCPU].iPontos = 0;
											if (CPU[iCPU].id == 1) strcpy (Cromos,"Sou o REI dos PUTO.");
											if (CPU[iCPU].id == 2) strcpy (Necros,"Sou o REI dos PUTO.");
											if (CPU[iCPU].id == 3) strcpy (Morgan,"Sou o REI dos PUTO.");
											if (CPU[iCPU].id == 4) strcpy (Parplo,"Sou o REI dos PUTO.");
											CPU[iCPU].iVitoria++;
											if (CPU[iCPU].iNivel < 8)
											{
												if (Vencedor_Anterior == CPU[iCPU].id)
												{
													iAuxVencedor++;
													if (iAuxVencedor >= 2)
													{
														iAuxVencedor = 0;
														CPU[iCPU].iNivel++;
													}
												}
												else
												{
													Vencedor_Anterior = CPU[iCPU].id;
													iAuxVencedor++;
												}
											}
			 								bFimPartida = true;
										}//
          			
										if (CPU[iCPU].iAcumulado + CPU[iCPU].iPontos > iMaxPontos)
										{//
											if (CPU[iCPU].id == 1) strcpy (Cromos,"Marcou mais do que devia.");
											if (CPU[iCPU].id == 2) strcpy (Necros,"Marcou mais do que devia.");
											if (CPU[iCPU].id == 3) strcpy (Morgan,"Marcou mais do que devia.");
											if (CPU[iCPU].id == 4) strcpy (Parplo,"Marcou mais do que devia.");
											PassaVez();
										}//
        
										
	
									}//
								}//
								else
								{
									if (CPU[iCPU].id == 1) strcpy (Cromos,"Nao Marcou pontos.");
									if (CPU[iCPU].id == 2) strcpy (Necros,"Nao Marcou pontos.");
									if (CPU[iCPU].id == 3) strcpy (Morgan,"Nao Marcou pontos.");
									if (CPU[iCPU].id == 4) strcpy (Parplo,"Nao Marcou pontos.");
									PassaVez();
								}
							}
     					} 
        			}	else Buffer_CPU--;
				}
			}
			
			if (bFimPartida)
			{
				if (CPU[iCPU].iAcumulado == iMaxPontos) draw_sprite(Buffer,(BITMAP *)imgDados[10].dat,76+(CPU[iCPU].id*140),10);
				else
				if (CPU[iCPU].iAcumulado == iMaxPontos) draw_sprite(Buffer,(BITMAP *)imgDados[10].dat,76+(CPU[iCPU].id*140),10);
				else
				if (CPU[iCPU].iAcumulado == iMaxPontos) draw_sprite(Buffer,(BITMAP *)imgDados[10].dat,76+(CPU[iCPU].id*140),10);
				else
				if (CPU[iCPU].iAcumulado == iMaxPontos) draw_sprite(Buffer,(BITMAP *)imgDados[10].dat,76+(CPU[iCPU].id*140),10);
				else
				draw_sprite(Buffer,(BITMAP *)imgDados[10].dat,75,10);
			}

			JogaDados(Buffer);
			textprintf_centre_ex(Buffer, (FONT *)imgFontes[0].dat, 400,325,makecol(144, 238, 144), -1,"%d",iPontoVisual);
			Controles();
			fps--;
		} 
		blit(Buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
	}
	Pote.imgPote = NULL;
	Buffer = NULL;
	delete Pote.imgPote;
	delete Buffer;
	unload_datafile(imgDados);

	Reinicia_Jogo();
	GravaArquivo();

	allegro_exit();
	return 0;     
}

void Mostra_Abertura(BITMAP *Buffer)
{
	static BITMAP *imgAbertura=NULL;
	imgAbertura = create_bitmap(SCREEN_W,SCREEN_H);
	imgAbertura = (BITMAP*)imgDados[0].dat;

	while (!key[KEY_ENTER])
	{
		clear_bitmap(Buffer);
		blit(imgAbertura,Buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(Buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
		fps=0;
	}
}

void Mostra_Msg(BITMAP *Buffer)
{
	static BITMAP *imgMsg=NULL;
	imgMsg = create_bitmap(250,50);
	imgMsg = (BITMAP*)imgDados[15].dat;

	while (!key[KEY_ENTER])
	{
		blit(imgMsg,Buffer,0,0,252,302,250,50);
		blit(Buffer,screen,0,0,0,0,800,600);
		fps=0;
	}
}

void Mostra_Ajuda()
{
	static BITMAP *imgAjuda=NULL;
	imgAjuda = create_bitmap(SCREEN_W,SCREEN_H);
	imgAjuda = (BITMAP*)imgDados[11].dat;

	BITMAP *Buffer;
	Buffer = create_bitmap(SCREEN_W,SCREEN_H);

	while (!key[KEY_ESC])
	{
		clear_bitmap(Buffer);
		blit(imgAjuda,Buffer,0,0,0,0,SCREEN_W,SCREEN_H);
		blit(Buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
		fps=0;
	}
}

void Controles()
{
	static int Buffer_teclado = 50;

	if (Buffer_teclado == 0)
	{
		if (key[KEY_ESC])
		{
			GameOver=true;
			Buffer_teclado = 10;
		}
		if (key[KEY_F1])
		{
			Mostra_Ajuda();
			Buffer_teclado = 10;
		}
		if (key[KEY_F2] && bFimPartida)
		{
			Reinicia_Jogo();
			Buffer_teclado = 10;
		}
		if (key[KEY_F12])
		{
			Reseta_Vitorias();
			Buffer_teclado = 10;
		}
		if (bJogador && !bFimPartida)
  		{
			if (key[KEY_E])
			{
				if (Pote.iAngulo == 0 || Pote.iAngulo == 130)
				{
					Pote.iAngulo = 10;
				}else
				{
					if (Pote.iAngulo == 10)
					{
						Pote.iAngulo = -10;
					}
  					else
					if (Pote.iAngulo == -10)
					{
						Pote.iAngulo = 0;
					}
				}
				MexeDados();
				Buffer_teclado = 5;
			}
			if (key[KEY_R]) // vira o copo
			{
				Pote.iAngulo = 130;
		 		MexeDados();
	 			bJogouDados = true;
				Buffer_teclado = 10;
			}
			if (key[KEY_T])
			{
				if (bMarcador)
				{
					bMarcador = false;
					Jogador.bLista = true;
					Jogador.iAcumulado += Jogador.iPontos;
					bJogador = false; // passa a vez para a cpu
					bJogouDados=false;
					Jogador.iPontos = 0;
					strcpy (cJogador,"Marcou o ponto.");
					Buffer_teclado = 10;
					RecolocaDados();
				}
			}
		}

		if(key[KEY_SPACE])
		{
			RecolocaDados();
			Buffer_teclado = 10;
		}
	} else Buffer_teclado--;
}

int ContaPontos()
{
	int iTotal=0;
	int i=0;
	int iTabela[7];
	
	// Zera Tabela
	for (i=0;i<7;i++)
	{
		iTabela[i] = 0;
	}

	//Contabiliza Dados
	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			iTabela[Dados[i].iNumero] += 1;
		}
	}

	// Calcula Pontos
	for (i=1;i<7;i++)
	{
		if (iTabela[i]>=3)
		{
			if (i == 1)
			{
				iTotal += 1000;

			}else
			{
				iTotal += i*100;
			}
			DesabilitaDado(Dados,i,3);
			iTabela[i] = iTabela[i]-3;
		}
		
		if (iTabela[i] == 2)
		{
			if (i == 1)
			{
				iTotal += iTabela[i]*100;
				iTabela[i] = iTabela[i]-2;
				DesabilitaDado(Dados,i,2);
			}
			if (i == 5)
			{
				iTotal += iTabela[i]*50;
				iTabela[i] = iTabela[i]-2;
				DesabilitaDado(Dados,i,2);
			}
		}
		
		if (iTabela[i] == 1)
		{
			if (i == 1)
			{
				iTotal += iTabela[i]*100;
				iTabela[i] = iTabela[i]-1;
				DesabilitaDado(Dados,i,1);
			}
			if (i == 5)
			{
				iTotal += iTabela[i]*50;
				iTabela[i] = iTabela[i]-1;
				DesabilitaDado(Dados,i,1);
			}
		}
	}

	return iTotal;
}

void DesabilitaDado(strDados Dados[], int iValorDado, int iQtde)
{
	int i=0;
	int iControle=0;
	
	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			if (iValorDado == Dados[i].iNumero)
			{
				Dados[i].bDisponivel = false;
				iControle++;
				if (iControle == iQtde)
				{
					break;
				}
			}
		}
	}
}

void RecolocaDados()
{
	int i=0;
	iPontoVisual = 0;
	for (i=0;i<5;i++)
	{
		Dados[i].bDisponivel = true;
	}
	bJogouDados=false;
}

void DesenhaPote(BITMAP *Buffer, strPote Pote)
{
	rotate_sprite(Buffer, Pote.imgPote, Pote.iPosx, Pote.iPosy, itofix(Pote.iAngulo));
}

bool VerificaDados()
{
	bool bTodos = true;
	int i=0;
	
	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			bTodos = false;
			break;
		}
	}
	return bTodos;
}

int ContaDados() // conta quantos dados tem disponiveis
{
	int i=0;
	int iQtde;
	for (i=0;i<5;i++)
	{
		if (Dados[i].bDisponivel)
		{
			iQtde++;
		}
	}
	return iQtde;
}

void PassaVez()
{
	RecolocaDados();

	if (!bJogador)
	{
		if (iCPU == 4)
		{
			CPU[iCPU].iPontos = 0;
			iCPU = 1;
		 	bJogador = true;
			bMarcador = false;
			bJogouDados = false;
		}
    	else
     	{
     		CPU[iCPU].iPontos = 0;
      		iCPU++;
     	}
	}
 	else 
 	{
 		Jogador.iPontos = 0;
 		bMarcador = false;
  		bJogador = false;
  		bJogouDados = false;
	}
}

void Reseta_Vitorias()
{
	Vencedor_Anterior=-0;
	iAuxVencedor=0;

	Jogador.iVitoria=0;
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
}
END_OF_MAIN();
