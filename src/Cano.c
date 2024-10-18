class Cano
{
    private:

        int spriteID;
        double posicaoX, posicaoY;
        double velocidadeX, velocidadeY;
        double larguraSprite, alturaSprite;

        int contabilizado;

    public:

        Cano(double px, double py, double largura, double altura,
                double vx, double vy, char* sprite)
        {
            posicaoX        = px;
            posicaoY        = py;
            velocidadeX     = vx;
            velocidadeY     = vy;
            larguraSprite   = largura;
            alturaSprite    = altura;

            spriteID        = PIG_criarSprite(sprite, 0);
            contabilizado   = NAO;
        }

        ~Cano()
        {
            PIG_destruirSprite(spriteID);
        }

        void movimentarX()
        {
            posicaoX += velocidadeX;
        }

        void movimentarY()
        {
            posicaoY += velocidadeY;
        }

        void loop(double posicaoFinal, double novaAbertura)
        {
            if (posicaoX <= -larguraSprite)
            {
                posicaoX = posicaoX + posicaoFinal;
                posicaoY = novaAbertura;
                contabilizado = NAO;
            }
        }

        void setVelocidade(double valor)
        {
            posicaoX = valor;
        }

        void setContabilizado(int valor)
        {
            contabilizado = valor;
        }

        int getContabilizado()
        {
            return contabilizado;
        }

        void desenhar(double angulo)
        {
            PIG_desenharSprite(spriteID, posicaoX, posicaoY, larguraSprite, alturaSprite, angulo, NAO, BRANCO, 255, SIM);
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
