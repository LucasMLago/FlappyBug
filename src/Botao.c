class Botao
{
    private:

        int spriteID;
        double posicaoX, posicaoY;
        double larguraSprite, alturaSprite;

        int botaoStatus;
        int botaoPausaStatus;
        int botaoStartStatus;
        int botaoRestartStatus;

    public:

    Botao(double px, double py, double largura, double altura, char* sprite)
    {
        posicaoX        = px;
        posicaoY        = py;
        larguraSprite   = largura;
        alturaSprite    = altura;

        spriteID        = PIG_criarSprite(sprite, 0);
    }

    ~Botao()
    {
        PIG_destruirSprite(spriteID);
    }

    void desenhar()
    {
        PIG_desenharSprite(spriteID, posicaoX, posicaoY, larguraSprite, alturaSprite, 0, NAO, BRANCO, 255, SIM);
    }

    void setBotaoStatus(double valor)
    {
        botaoStatus = valor;
    }

    int getBotaoStatus()
    {
        return botaoStatus;
    }

    //void setBotaoStartStatus(double valor)
    //{
    //    botaoStartStatus = valor;
    //}

    //void setBotaoRestartStatus(double valor)
    //{
      //  botaoRestartStatus = valor;
    //}

    int mauseClique(double xMause, double yMause)
    {
        if (xMause >= posicaoX && xMause <= posicaoX + larguraSprite &&
            yMause >= posicaoY && yMause >= posicaoY + alturaSprite)
        {
            return SIM;
        }
        else
        {
            return NAO;
        }
    }

    double getPosicaoX()
    {
        return posicaoX;
    }

    double getPosicaoY()
    {
        return posicaoY;
    }

    double getLarguraSprite()
    {
        return larguraSprite;
    }

    double getAlturaSprite()
    {
        return alturaSprite;
    }
};
