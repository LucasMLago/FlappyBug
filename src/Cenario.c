class Cenario
{
    private:

        int spriteID;
        double posicaoX, posicaoY;
        double velocidadeX, velocidadeY;
        double larguraSprite, alturaSprite;

    public:

        Cenario(double px, double py, double largura, double altura,
                double vx, double vy, char* sprite)
        {
            posicaoX        = px;
            posicaoY        = py;
            velocidadeX     = vx;
            velocidadeY     = vy;
            larguraSprite   = largura;
            alturaSprite    = altura;

            spriteID        = PIG_criarSprite(sprite, 0);
        }

        ~Cenario()
        {
            PIG_destruirSprite(spriteID);
        }

        void movimentar()
        {
            posicaoX = posicaoX + velocidadeX;
        }

        void loop(double posicaoFinal)
        {
            if (posicaoX <= -larguraSprite)
            {
                posicaoX = posicaoX + posicaoFinal;
            }
        }

        void setVelocidade(double valor)
        {
            velocidadeX = valor;
        }

        void desenhar()
        {
            PIG_desenharSprite(spriteID, posicaoX, posicaoY, larguraSprite, alturaSprite, 0, NAO, BRANCO, 255, SIM);
        }

        double getPosicaoY()
        {
            return posicaoY;
        }

        double getAltura()
        {
            return alturaSprite;
        }
};
