#define TOTAL_DE_FRAMES_DO_PASSARO 3

class Bird
{
    private:

        int spriteID[TOTAL_DE_FRAMES_DO_PASSARO];
        double posicaoX, posicaoY;
        double larguraSprite, alturaSprite;
        double velocidadeY;
        double anguloSprite;

        int contadorAnimacao;
        int indiceFrameAtual;

        void atualizarAnimacao()
        {
            contadorAnimacao += 1;

            if (contadorAnimacao > 20)
            {
                contadorAnimacao = 0;

                indiceFrameAtual += 1;

                if (indiceFrameAtual >= TOTAL_DE_FRAMES_DO_PASSARO)
                {
                    indiceFrameAtual = 0;
                }
            }
        }

        void gravidade()
        {
            double aceleracaoGravidade = -0.32;

            velocidadeY += aceleracaoGravidade;
            posicaoY += velocidadeY;
        }

        void atualizarAngulo()
        {
            if (anguloSprite >= -90)
            {
                anguloSprite -= 0.8;
            }
        }

    public:

        Bird(double px, double py, double largura, double altura, double vy, double angulo, char* listaSprite[TOTAL_DE_FRAMES_DO_PASSARO])
        {
            posicaoX        = px;
            posicaoY        = py;
            larguraSprite   = largura;
            alturaSprite    = altura;
            velocidadeY     = vy;
            anguloSprite    = angulo;

            contadorAnimacao = 0;
            indiceFrameAtual = 0;

            for (int i = 0; i < TOTAL_DE_FRAMES_DO_PASSARO;  i++)
            {
                spriteID[i] = PIG_criarSprite(listaSprite[i], 0);
            }
        }

        ~Bird()
        {
            for (int i = 0; i < TOTAL_DE_FRAMES_DO_PASSARO; i++)
            {
                PIG_destruirSprite(spriteID[i]);
            }
        }

        void atualizar()
        {
            atualizarAnimacao();
            atualizarAngulo();
            gravidade();
        }

        void pulo(double deslocamento)
        {
            anguloSprite = 20;

            velocidadeY = 2.5;
            velocidadeY += deslocamento;
        }

        void desenhar()
        {
            PIG_desenharSprite(spriteID[indiceFrameAtual], posicaoX, posicaoY, larguraSprite, alturaSprite, anguloSprite, NAO, BRANCO, 255, SIM);
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
