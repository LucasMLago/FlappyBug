class Interface
{
    private:

        int spriteID;
        double posicaoX, posicaoY;
        double larguraSprite, alturaSprite;

    public:

    Interface(double px, double py, double largura, double altura, char* sprite)
    {
        posicaoX        = px;
        posicaoY        = py;
        larguraSprite   = largura;
        alturaSprite    = altura;

        spriteID        = PIG_criarSprite(sprite, 0);
    }

    ~Interface()
    {
        PIG_destruirSprite(spriteID);
    }

    void desenhar()
    {
        PIG_desenharSprite(spriteID, posicaoX, posicaoY, larguraSprite, alturaSprite, 0, NAO, BRANCO, 255, SIM);
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
