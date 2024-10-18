#include "Cenario.c"
#include "Cano.c"
#include "Bird.c"
#include "Interface.c"
#include "Botao.c"

#define VELOCIDADE_PADRAO -2

#define QUANTIDADE_TOTAL_DE_NUVEM 2
#define QUANTIDADE_TOTAL_DE_PREDIO 2
#define QUANTIDADE_TOTAL_DE_ARVORE 2
#define QUANTIDADE_TOTAL_DE_CHAO 3

#define QUANTIDADE_TOTAL_DE_CANOS 4
#define DISTANCIA_ENTRE_CANOS 150

class FlappyBird
{
    private:

        int partidaEncerrada;
        int jogoPausado = 0;
        int score = 0;
        double velocidadePadrao = VELOCIDADE_PADRAO;

        char* ceu_sprite_path = "imagens/ceu.jpg";
        char* nuvem_sprite_path = "imagens/nuvens.png";
        char* predio_sprite_path = "imagens/predios.png";
        char* arvore_sprite_path = "imagens/arvores.png";
        char* chao_sprite_path = "imagens/chao.png";
        char* cano_sprite_path = "imagens/cano.png";
        char* passaro_sprite_path[3] = {"imagens/redbird-downflap.png", "imagens/redbird-midflap.png", "imagens/redbird-upflap.png"};
        char* game_over_sprite = "imagens/gameover.png";
        char* pausa_sprite = "imagens/pause.png";
        char* despausa_sprite = "imagens/despause.png";
        char* numeros_sprites[10] = {"imagens/0.png", "imagens/1.png", "imagens/2.png", "imagens/3.png", "imagens/4.png",
                                     "imagens/5.png", "imagens/6.png", "imagens/7.png", "imagens/8.png", "imagens/9.png"};
        char* logo_sprite = "imagens/Flappy_Logo.png";
        char* start_button_sprite = "imagens/start_button.png";
        char* restart_button_sprite = "imagens/restart_button_sprite";

        Bird* passaro;

        Cenario* ceu;

        Cenario* nuvem[QUANTIDADE_TOTAL_DE_NUVEM];
        Cenario* predio[QUANTIDADE_TOTAL_DE_PREDIO];
        Cenario* arvore[QUANTIDADE_TOTAL_DE_ARVORE];
        Cenario* chao[QUANTIDADE_TOTAL_DE_CHAO];

        Cano* canoInferior[QUANTIDADE_TOTAL_DE_CANOS];
        Cano* canoSuperior[QUANTIDADE_TOTAL_DE_CANOS];

        //Interface* logo;
        //Interface* menu;
        Interface* gameOver;
        Interface* menuInicar;
        Interface* pontuacao;

        Botao* start;
        Botao* restart;
        Botao* pausa;
        Botao* despausa;

        void iniciar()
        {

        }

        int verificaScore(double passaroX, double passaroLargura, double canoX, double canoLargura)
        {
            if (passaroX + passaroLargura >= canoX + canoLargura/2)
            {
                return SIM;
            }
            else
            {
                return NAO;
            }
        }

        int verificaColisaoPassaroCano(double passaroX, double passaroY, double passaroLargura, double passaroAltura,
                                       double canoX, double canoY, double canoLargura, double canoAltura)
        {
            if (passaroX + passaroLargura < canoX) // lado esquerdo
            {
                return NAO;
            }
            else if (canoX + canoLargura < passaroX) // lado direito
            {
                return NAO;
            }
            else if (passaroY + passaroAltura < canoY) // lado baixo
            {
                return NAO;
            }
            else if (canoY + canoAltura < passaroY) // lado cima
            {
                return NAO;
            }
            else
            {
                return  SIM;
            }
        }

        int verificaColisaoPassaroChao(double passaroY, double cenarioY, double chaoAltura)
        {
            if (passaroY > cenarioY + chaoAltura) // chão
            {
                return NAO;
            }
            else
            {
                return SIM;
            }
        }

        int verificaColisaoPassaroTeto(double passaroY, double passaroAltura, double tetoAltura)
        {
            if (passaroY + passaroAltura < tetoAltura) // teto
            {
                return NAO;
            }
            else
            {
                return SIM;
            }
        }

        void aplicaColisao()
        {
            if (partidaEncerrada == NAO)
            {
                double posicao_x_passaro = passaro->getPosicaoX() + 5;
                double posicao_y_passaro = passaro->getPosicaoY() + 5;
                double largura_passaro = passaro->getLarguraSprite() - 10;
                double altura_passaro = passaro->getAlturaSprite() - 10;

                int colisaoInferior = NAO;
                int colisaoSuperior = NAO;

                // Colisão pássaro x cano
                for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
                {
                    /// colisão Inferior
                    double posicao_x_canoInferior = canoInferior[i]->getPosicaoX();
                    double posicao_y_canoInferior = canoInferior[i]->getPosicaoY();
                    double largura_canoInferior = canoInferior[i]->getLarguraSprite();
                    double altura_canoInferior = canoInferior[i]->getAlturaSprite();

                    colisaoInferior = verificaColisaoPassaroCano(posicao_x_passaro, posicao_y_passaro, largura_passaro, altura_passaro,
                                                      posicao_x_canoInferior, posicao_y_canoInferior, largura_canoInferior, altura_canoInferior);

                    /// Colisão Superior
                    double posicao_x_canoSuperior = canoSuperior[i]->getPosicaoX();
                    double posicao_y_canoSuperior = canoSuperior[i]->getPosicaoY();
                    double largura_canoSuperior = canoSuperior[i]->getLarguraSprite();
                    double altura_canoSuperior = canoSuperior[i]->getAlturaSprite();

                    colisaoSuperior = verificaColisaoPassaroCano(posicao_x_passaro, posicao_y_passaro, largura_passaro, altura_passaro,
                                                      posicao_x_canoSuperior, posicao_y_canoSuperior, largura_canoSuperior, altura_canoSuperior);

                    if (colisaoInferior == SIM || colisaoSuperior == SIM)
                    {
                        encerrarPartida();
                        break;
                    }
                }

                /// Colisão pássaro x chão
                int colisaoChao = NAO;

                for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++)
                {
                    double posicao_y_cenario = chao[i]->getPosicaoY();
                    double altura_y_chao = chao[i]->getAltura();

                    colisaoChao = verificaColisaoPassaroChao(posicao_y_passaro, posicao_y_cenario, altura_y_chao);

                    if (colisaoChao == SIM)
                    {
                        encerrarPartida();
                        break;
                    }
                }

                /// Colisão pássaro x teto
                int colisaoTeto = NAO;
                double altura_y_teto = ALT_TELA;

                colisaoTeto = verificaColisaoPassaroTeto(posicao_y_passaro, altura_passaro, altura_y_teto);

                if (colisaoTeto == SIM)
                {
                    encerrarPartida();
                }
            }
        }

        int verificaPontuacao(double passaroX, double passaroLargura, double canoX, double canoLargura)
        {
            if (passaroX + passaroLargura >= canoX + canoLargura/2)
            {
                return SIM;
            }
            else
            {
                return NAO;
            }
        }

        void atualizarPontuacao()
        {
            // Percorre todos os canos
            for (int j = 0; j < QUANTIDADE_TOTAL_DE_CANOS; j++)
            {
                double posicao_x_passaro = passaro->getPosicaoX();
                double largura_passaro = passaro->getLarguraSprite();
                double posicao_x_cano = canoInferior[j]->getPosicaoX();
                double largura_cano = canoInferior[j]->getLarguraSprite();

                // Verifica se o pássaro passou do cano
                int passou_do_cano = verificaPontuacao(posicao_x_passaro, largura_passaro, posicao_x_cano, largura_cano);
                int foiContabilizado = canoInferior[j]->getContabilizado();

                if (passou_do_cano == SIM && foiContabilizado == NAO)
                {
                    canoInferior[j]->setContabilizado(SIM);
                    score++;

                    // Atualiza a pontuação quando a score é 10 ou mais
                    if (score >= 10)
                    {
                        int scoreNovo = score; // scoreNovo é igual à pontuação total
                        score = 0; // Reinicia a contagem de score

                        // Libera memória da pontuação anterior, se necessário
                        delete pontuacao;

                        // Cria nova interface de pontuação
                        pontuacao = new Interface(LARG_TELA / 2 - 15, 700, 24, 36, numeros_sprites[scoreNovo / 10]);
                        pontuacao->desenhar();
                    }

                    // Atualiza a interface de pontuação
                    delete pontuacao; // Libera a memória da pontuação anterior
                    pontuacao = new Interface(LARG_TELA / 2, 700, 24, 36, numeros_sprites[score]);
                }
            }
        }

        void atualizarInterfacePontuacao()
        {
            if (score < 10)
            {
                pontuacao = new Interface(LARG_TELA/2, 700, 24, 36, numeros_sprites[score]);
            }
            else
            {
                int temp = score;  // Variável temporária para manipular os dígitos
                int digitos[10];  // Assume no máximo 10 dígitos para números grandes
                int i = 0;

                // Armazena os dígitos em ordem inversa
                while (temp > 0)
                {
                    digitos[i] = temp % 10;
                    temp /= 10;
                    i++;
                }

                // Imprime os dígitos na ordem correta
                for (int j = i - 1; j >= 0; j--)
                {
                    pontuacao = new Interface(LARG_TELA/2, 700, 24, 36, numeros_sprites[digitos[j]]);
                }
            }
        }

        void alocarEntidades(double velocidade)
        {
            passaro = new Bird(LARG_TELA/2 - 150, ALT_TELA/2, 50, 36, 0, 0, passaro_sprite_path);

            ceu = new Cenario(0, 0, LARG_TELA, ALT_TELA, 0, 0, ceu_sprite_path);

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++) nuvem[i] = new Cenario(960 * i, 130, 960, 114, 0.40 * velocidade, 0, nuvem_sprite_path);

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++) predio[i] = new Cenario(960 * i, 145, 960, 54, 0.60 * velocidade, 0, predio_sprite_path);

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_ARVORE; i++) arvore[i] = new Cenario(960 * i, 110, 960, 52, 0.80 * velocidade, 0, arvore_sprite_path);

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++) chao[i] = new Cenario(480 * i, -250, 480, 360, velocidade, 0, chao_sprite_path);

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
            {
                int pixelAleatorio = (rand()%401) - 200; // retorna um valor de -200 até 200 inclusive

                canoInferior[i] = new Cano(1000 + (DISTANCIA_ENTRE_CANOS + 90) * i, ((ALT_TELA + 200) / 2) + pixelAleatorio, 90, 836, velocidade, 0, cano_sprite_path);
                canoSuperior[i] = new Cano(1000 + (DISTANCIA_ENTRE_CANOS + 90) * i, ((-ALT_TELA - 200) / 2) + pixelAleatorio, 90, 836, velocidade, 0, cano_sprite_path);
            }

            /// Interface
            //logo = new Interface(LARG_TELA/2 - 170, 700, 3676, 976, logo_sprite);
            //start = new Interface(LARG_TELA/2 - 170, ALT_TELA/2, 407, 150, start_button_sprite);
            gameOver = new Interface(LARG_TELA/2 - 170, ALT_TELA/2, 392, 100, game_over_sprite);
            pontuacao = new Interface(LARG_TELA / 2, 700, 24, 36, numeros_sprites[score]);

            pausa = new Botao(10, 720, 68, 68, pausa_sprite);
            despausa = new Botao(10, 720, 68, 68, despausa_sprite);
        }

    public:

        FlappyBird()
        {
            partidaEncerrada = NAO;
            alocarEntidades(velocidadePadrao);
            //pausa = new Botao(10, 720, 68, 68, pausa_sprite);
            //pausa->setBotaoStatus(NAO);
        }

        ~FlappyBird()
        {
            delete ceu;

            delete passaro;

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_NUVEM; i++) delete nuvem[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++) delete predio[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_ARVORE; i++) delete arvore[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++) delete chao[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
            {
                delete canoInferior[i];
                delete canoSuperior[i];
            }

            delete gameOver;
            delete pontuacao;
            delete start;
            delete restart;
            delete pausa;

        }

        void setVelocidade(double valor)
        {
            velocidadePadrao = valor;
        }

        double getVelocidadePadrao()
        {
            return VELOCIDADE_PADRAO;
        }

        void encerrarPartida()
        {
            partidaEncerrada = SIM;
            velocidadePadrao = 0;
        }

        void atualizar()
        {
            double posicao_y_passaro = passaro->getPosicaoY();

            if (posicao_y_passaro > 105) passaro->atualizar();

            if (partidaEncerrada == NAO)
            {
                for (int i = 0; i < QUANTIDADE_TOTAL_DE_NUVEM; i++)
                {
                    nuvem[i]->movimentar();
                    nuvem[i]->loop(QUANTIDADE_TOTAL_DE_NUVEM * 960);
                }

                for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++)
                {
                    predio[i]->movimentar();
                    predio[i]->loop(QUANTIDADE_TOTAL_DE_PREDIO * 960);
                }

                for (int i = 0; i < QUANTIDADE_TOTAL_DE_ARVORE; i++)
                {
                    arvore[i]->movimentar();
                    arvore[i]->loop(QUANTIDADE_TOTAL_DE_ARVORE * 960);
                }

                for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
                {
                    int pixelAleatorio = (rand()%401) - 200; // retorna um valor de -200 até 200 inclusive

                    canoInferior[i]->movimentarX();
                    canoInferior[i]->loop(QUANTIDADE_TOTAL_DE_CANOS * (DISTANCIA_ENTRE_CANOS + 90),
                                          ((ALT_TELA + 200) / 2) + pixelAleatorio);

                    canoSuperior[i]->movimentarX();
                    canoSuperior[i]->loop(QUANTIDADE_TOTAL_DE_CANOS * (DISTANCIA_ENTRE_CANOS + 90),
                                        ((-ALT_TELA - 200) / 2) + pixelAleatorio);
                }

                for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++)
                {
                    chao[i]->movimentar();
                    chao[i]->loop(QUANTIDADE_TOTAL_DE_CHAO * 480);
                }
                aplicaColisao();
                atualizarPontuacao();
            }
        }

        void desenhar()
        {
            ceu->desenhar();

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_NUVEM; i++) nuvem[i]->desenhar();

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++) predio[i]->desenhar();

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_ARVORE; i++) arvore[i]->desenhar();

            /// hitbox do pássaro
            //double posicao_x_passaro = passaro->getPosicaoX();
            //double posicao_y_passaro = passaro->getPosicaoY();
            //double largura_passaro = passaro->getLarguraSprite();
            //double altura_passaro = passaro->getAlturaSprite();

            //PIG_desenharRetangulo(posicao_x_passaro+5, posicao_y_passaro+5, largura_passaro - 10, altura_passaro - 10, VERMELHO, NAO, SIM);
            passaro->desenhar();

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
            {
                ///hitbox cano superior
                //double posicao_x_canoInferior = canoInferior[i]->getPosicaoX();
                //double posicao_y_canoInferior = canoInferior[i]->getPosicaoY();
                //double largura_canoInferior = canoInferior[i]->getLarguraSprite();
                //double altura_canoInferior = canoInferior[i]->getAlturaSprite();

                //PIG_desenharRetangulo(posicao_x_canoInferior, posicao_y_canoInferior, largura_canoInferior, altura_canoInferior, VERMELHO, NAO, SIM);
                canoInferior[i]->desenhar(0);

                ///hitbox cano superior cano inferior
                //double posicao_x_canoSuperior = canoSuperior[i]->getPosicaoX();
                //double posicao_y_canoSuperior = canoSuperior[i]->getPosicaoY();
                //double largura_canoSuperior = canoSuperior[i]->getLarguraSprite();
                //double altura_canoSuperior = canoSuperior[i]->getAlturaSprite();

                //PIG_desenharRetangulo(posicao_x_canoSuperior, posicao_y_canoSuperior, largura_canoSuperior, altura_canoSuperior, VERMELHO, NAO, SIM);
                canoSuperior[i]->desenhar(180);
            }

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++) chao[i]->desenhar();

            /// Interface
            if (partidaEncerrada == SIM)
            {
                gameOver->desenhar();
            }

            //desenharPausa();
            pontuacao->desenhar();
        }

        void desenharPausa()
        {
            if (jogoPausado == SIM)
            {
                printf("despausa\n");
                delete pausa;
                pausa = new Botao(100, 720, 68, 68, despausa_sprite);
                pausa->desenhar();
            }
            else
            {
                //printf("pausa\n");
                delete pausa;
                pausa = new Botao(10, 720, 68, 68, pausa_sprite);
                pausa->desenhar();
            }
        }

        void desenhaPausa()
        {
            //delete pausa;
            //pausa = new Botao(100, 720, 68, 68, despausa_sprite);
            pausa->desenhar();
        }

        void desenhaDespausa()
        {
            //delete pausa;
            //pausa = new Botao(100, 720, 68, 68, despausa_sprite);
            despausa->desenhar();
        }

        int getJogoPausado()
        {
            return jogoPausado;
        }

        void setJogoPausado(int valor)
        {
            jogoPausado = valor;
        }

        void verificarPulo()
        {
            if (PIG_tecla == TECLA_BARRAESPACO || PIG_botao == 1)
            {
                if (partidaEncerrada == NAO)
                {
                    passaro->pulo(4);
                }
            }
        }

        int getPartidaEncerradaStatus()
        {
            return partidaEncerrada;
        }

        void reiniciarPartida()
        {
            partidaEncerrada = NAO;

            /// Limpa
            delete ceu;

            delete passaro;

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_NUVEM; i++) delete nuvem[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_PREDIO; i++) delete predio[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_ARVORE; i++) delete arvore[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CHAO; i++) delete chao[i];

            for (int i = 0; i < QUANTIDADE_TOTAL_DE_CANOS; i++)
            {
                delete canoInferior[i];
                delete canoSuperior[i];
            }

            delete pausa;
            delete despausa;
            delete gameOver;
            delete pontuacao;

            /// Recria
            score = 0;
            jogoPausado = NAO;
            alocarEntidades(VELOCIDADE_PADRAO);
        }
};
