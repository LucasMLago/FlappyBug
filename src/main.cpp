#include "PIG.h"
#include "FlappyBird.c"

int main(int argc, char* args[])
{
    PIG_criarJanela("Flappy Bird", 800, 800);

    int timerGeral = PIG_criarTimer();
    int jogoPausado = NAO;

    FlappyBird* flappy = new FlappyBird();

    while (PIG_jogoRodando() == SIM)
    {
        PIG_atualizarJanela();

        if (PIG_tecla == TECLA_ENTER && jogoPausado == NAO)
        {
            flappy->reiniciarPartida();
        }

        int statusPartida = flappy->getPartidaEncerradaStatus();
        if ((PIG_tecla == TECLA_BARRAESPACO || PIG_botao == 1) && statusPartida == SIM) flappy->reiniciarPartida();

        int acabou = flappy->getPartidaEncerradaStatus();
        if (acabou == NAO)
        {
            if (PIG_tecla == TECLA_ESC && jogoPausado == NAO)
            {
                printf("entrou1");
                jogoPausado = SIM;
                flappy->setJogoPausado(SIM);
                //flappy->desenharPausa();
                flappy->desenhaDespausa();
                flappy->setVelocidade(0);
                //flappy->menu();
            }
        }

        if (jogoPausado == NAO) flappy->verificarPulo();

        if (PIG_tempoDecorrido(timerGeral) > 0.01) // A cada 0.01 segundos, atualizamos e desenhamos (100 fps)
        {
            if (jogoPausado == NAO)
            {
                //printf("entrou atualizacao\n");
                /// Atualizar

                PIG_atualizarCamera();
                flappy->atualizar();
                flappy->desenharPausa();

                /// Desenhar

                PIG_iniciarDesenho();

                flappy->desenhar();
                flappy->desenhaPausa();

                PIG_encerrarDesenho();
                PIG_reiniciarTimer(timerGeral);
            }
            else flappy->desenhaDespausa();

            if (PIG_tecla == TECLA_ESC && jogoPausado == SIM)
            {
                printf("entrou3\n");
                double retornaVelocidade = flappy->getVelocidadePadrao();
                jogoPausado = NAO;
                flappy->setJogoPausado(NAO);
                //flappy->desenharPausa();
                flappy->setVelocidade(retornaVelocidade);
            }
        }

    }

    delete flappy;
    PIG_destruirTimer(timerGeral);
    PIG_finalizarJanela();

    return 0;
}
