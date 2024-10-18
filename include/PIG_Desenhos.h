/********************************
Seção de elementos geométricos
********************************/

void PIG_desenharLinha(double x1, double y1, double x2, double y2, PIG_Cor cor, double grossuraLinha, int referencialVirtual)
{
    if(referencialVirtual == SIM)
    {
        x1              = XVirtualParaReal(x1);
        y1              = YVirtualParaReal(y1);
        x2              = XVirtualParaReal(x2);
        y2              = YVirtualParaReal(y2);
        grossuraLinha   = tamanhoVirtualParaReal(grossuraLinha);
    }

    if(grossuraLinha > 1)
    {
        for(int i=-grossuraLinha/2; i<round(grossuraLinha/2); i++)
        {
            for(int j=-grossuraLinha/2; j<round(grossuraLinha/2); j++)
            {
                jogo->DesenhaLinhaSimples(x1+i,y1+j,
                                          x2+i,y2+j,
                                          cor);
            }
        }
    }
    else
    {
        if(grossuraLinha <= 1)
        {
            jogo->DesenhaLinhaSimples(x1,y1,x2,y2,cor);
        }
    }
}

void PIG_desenharLinhasConectadas(double* pontosX, double* pontosY, int quantidadePontos, PIG_Cor cor, int grossuraLinha, int referencialVirtual)
{
    SDL_Point* pontosNovos = (SDL_Point*)malloc(quantidadePontos*sizeof(SDL_Point));
    for(int k=-grossuraLinha/2; k<round(grossuraLinha/2); k++)
    {
        for(int j=-grossuraLinha/2; j<round(grossuraLinha/2); j++)
        {
            for(int i=0; i<quantidadePontos; i++)
            {
                if(referencialVirtual == SIM)
                {
                    pontosNovos[i].x = XVirtualParaReal(pontosX[i]) + k;
                    pontosNovos[i].y = ALT_TELA - (YVirtualParaReal(pontosY[i]) + j);
                }
                else
                {
                    pontosNovos[i].x = pontosX[i] + k;
                    pontosNovos[i].y = ALT_TELA - (pontosY[i] + j);
                }
            }
            jogo->DesenhaLinhas(pontosNovos, quantidadePontos, cor);
        }
    }
    free(pontosNovos);
}

void PIG_desenharPonto(double x, double y, PIG_Cor cor, double tamanho, int referencialVirtual)
{
    if(referencialVirtual == SIM)
    {
        x       = XVirtualParaReal(x);
        y       = YVirtualParaReal(y);
        tamanho = tamanhoVirtualParaReal(tamanho);
    }
    jogo->DesenharPonto(x,y,cor,tamanho);
}

void PIG_desenharPontos(double* pontosX, double* pontosY, int quantidadePontos, PIG_Cor cor, int referencialVirtual)
{
    SDL_Point* pontosNovos = (SDL_Point*)malloc(quantidadePontos*sizeof(SDL_Point));
    for(int i=0; i<quantidadePontos; i++)
    {
        if(referencialVirtual == SIM)
        {
            pontosNovos[i].x = XVirtualParaReal(pontosX[i]);
            pontosNovos[i].y = ALT_TELA - YVirtualParaReal(pontosY[i]);
        }
        else
        {
            pontosNovos[i].x = pontosX[i];
            pontosNovos[i].y = ALT_TELA - pontosY[i];
        }
    }
    jogo->DesenharPontos(pontosNovos, quantidadePontos, cor);
    free(pontosNovos);
}

void PIG_desenharCirculoVazado(double centroX, double centroY, double raio, int grossuraLinha, PIG_Cor cor, int referencialVirtual)
{
    double pontosX[1000];
    double pontosY[1000];
    double angulo = 0;
    double qtd = 360;
    double X,Y;

    for(int i=0; i<(int)qtd; i++)
    {
        pontosX[i] = centroX + raio*cos(DEGTORAD*angulo);
        pontosY[i] = centroY + raio*sin(DEGTORAD*angulo);

        angulo = angulo + 360.0/(qtd-1);
    }

    PIG_desenharLinhasConectadas(pontosX, pontosY, qtd, cor, grossuraLinha, referencialVirtual);
}

void PIG_desenharRetangulo(double x, double y, double largura, double altura, PIG_Cor cor, int alinhadoCentro, int referencialVirtual)
{
    if(alinhadoCentro == SIM)
    {
        x = x - largura/2.0;
        y = y - altura/2.0;
    }

    if(referencialVirtual == SIM)
    {
        x       = XVirtualParaReal(x);
        y       = YVirtualParaReal(y);
        largura = tamanhoVirtualParaReal(largura);
        altura  = tamanhoVirtualParaReal(altura);
    }

    jogo->DesenhaRetangulo(x,y,altura,largura,cor);
}

void PIG_desenharRetanguloVazado(double x, double y, double largura, double altura, PIG_Cor cor, int alinhadoCentro, int referencialVirtual)
{
    if(alinhadoCentro == SIM)
    {
        x = x - largura/2.0;
        y = y - altura/2.0;
    }

    if(referencialVirtual == SIM)
    {
        x       = XVirtualParaReal(x);
        y       = YVirtualParaReal(y);
        largura = tamanhoVirtualParaReal(largura);
        altura  = tamanhoVirtualParaReal(altura);
    }

    jogo->DesenhaRetanguloVazado(x, y, altura, largura, cor);
}

void PIG_desenharGrade( double X, double Y, double Largura, double Altura,
                        int qtdColunas, int qtdLinhas, PIG_Cor cor,
                        int referencialVirtual)
{
    double tamanhoHorizontal = Largura/(double)qtdColunas;
    double tamanhoVertical = Altura/(double)qtdLinhas;

    for(int i=0; i<=qtdColunas; i++)
    {
        PIG_desenharLinha(  X+(i*tamanhoHorizontal),
                            Y,
                            X+(i*tamanhoHorizontal),
                            Y+Altura,
                            cor,1,referencialVirtual);
    }
    for(int i=0; i<=qtdLinhas; i++)
    {
        PIG_desenharLinha(  X,
                            Y+(i*tamanhoVertical),
                            X+Largura,
                            Y+(i*tamanhoVertical),
                            cor,1,referencialVirtual);
    }
}


/********************************
A função desenharSprite() é responsável por desenhar um sprite (que já esteja criado) na tela
********************************/

void PIG_desenharSprite(int id_sprite, double x, double y, double largura, double altura, double angulo, int alinhadoCentro, PIG_Cor cor, int opacidade, int referencialVirtual)
{
    if(alinhadoCentro == SIM)
    {
        x = x - largura/2.0;
        y = y - altura/2.0;
    }

    if(referencialVirtual == SIM)
    {
        x       = XVirtualParaReal(x);
        y       = YVirtualParaReal(y);
        largura = tamanhoVirtualParaReal(largura);
        altura  = tamanhoVirtualParaReal(altura);
    }

    cor.a = opacidade;
    CGerenciadorObjetos::DesenhaSprite(id_sprite,x,y,largura,altura,angulo,cor);
}

