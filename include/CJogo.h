class CJogo{

private:

    SDL_Texture *text_fundo;
    PIG_Cor corFundo;
    PIG_Evento ultimoEvento;
    PIG_Teclado teclado;
    PIG_Mouse mouse;
    int contFPS;
    Timer timerFPS;
    MapaCaracteres fontes[MAX_FONTES];

    int estado;
    int rodando;

public:

    SDL_Window* window;
    SDL_Renderer* renderer;
    OffscreenRenderer offRenderer;
    int cursorPadrao;

    CJogo(char *nome, int cursor=0)
    {
        rodando = true;
        teclado = SDL_GetKeyboardState(NULL);
        mouse = (int*)calloc(4*sizeof(int), 0);
        estado = 0;
        contFPS = 0;
        timerFPS = new CTimer();
        offRenderer = NULL;
        cursorPadrao = cursor;

		text_fundo = NULL;
        if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        }else{
            window = SDL_CreateWindow( nome, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARG_TELA, ALT_TELA, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL );
            if( window == NULL ){
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            }else{
                renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
                SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
            }
        }

        TTF_Init();

        //inicializa o vetor contendo as fontes a serem usadas no jogo
        for (int i=0;i<MAX_FONTES;i++)
            fontes[i] = NULL;

        //cria��o da fonte padr�o da PIG (fonte com identificador 0)
        for(int i=0; i<MAX_FONTES; i++)
        {
            fontes[i] = NULL;
        }
        fontes[0] = new CMapaCaracteres("..//fontes//arial.ttf",renderer,36,ESTILO_NORMAL,BRANCO,0,BRANCO);

        corFundo = PRETO;
    }

    ~CJogo()
    {
        delete timerFPS;
        if (offRenderer) delete offRenderer;

        for (int i=0;i<MAX_FONTES;i++)
            if (fontes[i]) delete fontes[i];
        TTF_Quit();

        if (text_fundo) SDL_DestroyTexture(text_fundo);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);

        free(mouse);

        SDL_Quit();
    }

    int getProximoIndiceFonte()
    {
        for(int i=0; i<MAX_FONTES; i++)
        {
            if(fontes[i] == NULL)
            {
                return i;
            }
        }
        return -1;
    }

    void DefineFundo(char *nomeArquivo)
    {
        SDL_Surface* bitmap = SDL_LoadBMP(nomeArquivo);
        if (text_fundo) SDL_DestroyTexture(text_fundo);
        text_fundo = SDL_CreateTextureFromSurface(renderer,bitmap);
        SDL_FreeSurface(bitmap);
    }

    PIG_Evento PegaEvento()
    {

        SDL_Event event;
        if(SDL_PollEvent((&event)))
        {
            if(event.type == SDL_QUIT)
            {
                rodando = false;
            }

            switch(event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    ultimoEvento.tipoEvento = EVENTO_MOUSE;
                    ultimoEvento.mouse.acao = MOUSE_PRESSIONADO;
                    ultimoEvento.mouse.botao= event.button.button;
                    ultimoEvento.mouse.posX = event.button.x;
                    ultimoEvento.mouse.posY = ALT_TELA - event.button.y;
                    mouse[ultimoEvento.mouse.botao] = 1;

                    break;
                case SDL_MOUSEBUTTONUP:
                    ultimoEvento.tipoEvento = EVENTO_MOUSE;
                    ultimoEvento.mouse.acao = MOUSE_LIBERADO;
                    ultimoEvento.mouse.botao= event.button.button;
                    ultimoEvento.mouse.posX = event.button.x;
                    ultimoEvento.mouse.posY = ALT_TELA - event.button.y;
                    mouse[ultimoEvento.mouse.botao] = 0;

                    ultimoEvento.mouse.botao = 0;

                    break;
                case SDL_MOUSEMOTION:
                    ultimoEvento.tipoEvento = EVENTO_MOUSE;
                    ultimoEvento.mouse.acao = MOUSE_MOVIDO;
                    ultimoEvento.mouse.posX = event.motion.x;
                    ultimoEvento.mouse.posY = ALT_TELA - event.motion.y;
                    ultimoEvento.mouse.relX = event.motion.xrel;
                    ultimoEvento.mouse.relY = -event.motion.yrel;
                    if (cursorPadrao == 0)
                        CMouse::Move(ultimoEvento.mouse.posX,ALT_TELA-ultimoEvento.mouse.posY);
                    break;
                case SDL_MOUSEWHEEL:
                    ultimoEvento.tipoEvento = EVENTO_MOUSE;
                    ultimoEvento.mouse.acao = MOUSE_RODINHA;
                    ultimoEvento.mouse.relX = event.wheel.x;
                    ultimoEvento.mouse.relY = event.wheel.y;
                    break;
                case SDL_KEYDOWN:
                    ultimoEvento.tipoEvento = EVENTO_TECLADO;
                    ultimoEvento.teclado.acao = TECLA_PRESSIONADA;
                    ultimoEvento.teclado.tecla = event.key.keysym.scancode;
                    ultimoEvento.teclado.repeticao = event.key.repeat;
                    break;
                case SDL_KEYUP:
                    ultimoEvento.tipoEvento = EVENTO_TECLADO;
                    ultimoEvento.teclado.acao = TECLA_LIBERADA;
                    ultimoEvento.teclado.tecla = event.key.keysym.scancode;
                    ultimoEvento.teclado.repeticao = event.key.repeat;
                    break;
                case SDL_TEXTEDITING:
                    ultimoEvento.tipoEvento = EVENTO_TECLADO;
                    ultimoEvento.teclado.acao = TECLA_EDICAO;
                    ultimoEvento.teclado.inicio = event.edit.start;
                    ultimoEvento.teclado.tamanhoSelecao = event.edit.length;
                    strcpy(ultimoEvento.teclado.texto,event.edit.text);
                    break;
                case SDL_TEXTINPUT:
                    ultimoEvento.tipoEvento = EVENTO_TECLADO;
                    ultimoEvento.teclado.acao = TECLA_INPUT;
                    strcpy(ultimoEvento.teclado.texto,event.edit.text);
                    break;
                case SDL_WINDOWEVENT:
                    ultimoEvento.tipoEvento = EVENTO_JANELA;
                    ultimoEvento.janela.acao = event.window.event;
                    ultimoEvento.janela.dado1 = event.window.data1;
                    ultimoEvento.janela.dado2 = event.window.data2;
                    break;
            }
        }
        else
        {
            ultimoEvento.tipoEvento = EVENTO_NULO;
        }
        return ultimoEvento;
    }

    inline PIG_Teclado PegaTeclado()
    {
        return teclado;
    }

    inline PIG_Mouse PegaMouse()
    {
        return mouse;
    }

    void IniciaInputTextoTela(){
        SDL_StartTextInput();
    }

    void EncerraInputTextoTela(){
        SDL_StopTextInput();
    }

    void IniciaDesenho(){
        if (text_fundo==NULL) {
            SDL_SetRenderDrawColor(renderer,corFundo.r,corFundo.g,corFundo.b,255);
            SDL_RenderClear( renderer );
        }
        else SDL_RenderCopy(renderer,text_fundo,NULL,NULL);
    }

    void EncerraDesenho()
    {
        if(0)
        {
            register int i;
            register int j;
            SDL_Surface* saveSurface = NULL;
            SDL_Surface* infoSurface = NULL;
            infoSurface = SDL_GetWindowSurface(window);
            if (infoSurface != NULL)
            {
                int pitch = infoSurface->format->BytesPerPixel;

                unsigned char * pixels = new unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
                if (pixels != 0)
                {
                    if(SDL_RenderReadPixels(renderer,
                                            &infoSurface->clip_rect,
                                            infoSurface->format->format,
                                            pixels,
                                            infoSurface->w * infoSurface->format->BytesPerPixel) == 0)
                    {
                        for(i=0; i<infoSurface->h; i++)
                        {
                            for(j=0; j<infoSurface->w; j++)
                            {
                                int offset = (i*infoSurface->w+j)*4;
                                for(int k=0;k<4;k++)
                                {
                                    pixels[offset+k] = (255-pixels[offset+k]);
                                }
                            }
                        }
                        saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                        SDL_Texture *t= SDL_CreateTextureFromSurface(renderer,saveSurface);

                        SDL_RenderCopy(renderer,t,NULL,NULL);
                        SDL_DestroyTexture(t);
                        SDL_FreeSurface(saveSurface);
                    }
                }
                delete[] pixels;
                SDL_FreeSurface(infoSurface);
            }
        }

        if(0)
        {
            SDL_Surface* infoSurface = SDL_GetWindowSurface(window);
            int pitch = infoSurface->format->BytesPerPixel;

            /// Criando textura do local de origem

            int altura = 200;
            int largura = 200;
            SDL_Rect origem = {500, 600, largura, altura};
            SDL_Rect destino = {1100, 175, largura, altura};

            unsigned char * pixels = new unsigned char[largura*altura*infoSurface->format->BytesPerPixel];

            SDL_RenderReadPixels(renderer,
                                 &origem,
                                 infoSurface->format->format,
                                 pixels,
                                 altura*pitch);


            SDL_Surface* saveSurface = SDL_CreateRGBSurfaceFrom(
                                       pixels,
                                       altura,
                                       largura,
                                       infoSurface->format->BitsPerPixel,
                                       altura*infoSurface->format->BytesPerPixel,
                                       infoSurface->format->Rmask,
                                       infoSurface->format->Gmask,
                                       infoSurface->format->Bmask,
                                       infoSurface->format->Amask);


            SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, saveSurface);

            SDL_RenderCopyEx(renderer, t, NULL, &destino, 0, NULL, SDL_FLIP_NONE);

            SDL_DestroyTexture(t);
            SDL_FreeSurface(saveSurface);
            SDL_FreeSurface(infoSurface);
            delete[] pixels;

//            register int i;
//            register int j;
//             saveSurface = NULL;
//
//
//
//            SDL_Rect dest = {500,500,200,200};
//            if (infoSurface != NULL)
//            {
//
//
//                int largura = 200;
//                int altura = 200;
//
//                if (pixels != 0)
//                {
//
//
//                    if( == 0)
//                    {
////                        for(i=0; i<infoSurface->h; i++)
////                        {
////                            for(j=0; j<infoSurface->w; j++)
////                            {
////                                int offset = (i*infoSurface->w+j)*4;
////                                for(int k=0;k<4;k++)
////                                {
////                                    pixels[offset+k] = (255-pixels[offset+k]);
////                                }
////                            }
////                        }
//                        printf("1\n");
//                        saveSurface = SDL_CreateRGBSurfaceFrom(pixels,
//                                                               200,
//                                                               200,
//                                                               infoSurface->format->BitsPerPixel,
//                                                               200*infoSurface->format->BytesPerPixel,
//                                                               infoSurface->format->Rmask,
//                                                               infoSurface->format->Gmask,
//                                                               infoSurface->format->Bmask,
//                                                               infoSurface->format->Amask);
//
//
//
//                        SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, saveSurface);
//
//                        SDL_RenderCopyEx(renderer, t, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
//
//                        //SDL_RenderCopy(renderer,t,NULL,NULL);
//                        SDL_DestroyTexture(t);
//                        SDL_FreeSurface(saveSurface);
//                    }
//                }
//                delete[] pixels;
//                SDL_FreeSurface(infoSurface);
//            }
        }

        SDL_RenderPresent(renderer);

        contFPS++;

        if (timerFPS->GetTempoDecorrido()>10.0){
            contFPS=0;
            timerFPS->Reinicia();
        }
    }

    int GetEstado(){
        return estado;
    }

    int GetRodando(){
        return rodando;
    }

    void SetEstado(int valor){
        estado = valor;
    }

    void DesenhaRetangulo(int x, int y, int altura, int largura, PIG_Cor cor){
        SDL_Rect rect;
        rect.x = x;
        rect.y = ALT_TELA-(y+altura);
        rect.h = altura;
        rect.w = largura;


        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
        SDL_RenderFillRect(renderer,&rect);
    }

    void DesenhaRetanguloVazado(int x, int y, int altura, int largura, PIG_Cor cor){
        SDL_Rect rect;
        rect.x = x;
        rect.y = ALT_TELA-(y+altura);
        rect.h = altura;
        rect.w = largura;


        SDL_SetRenderDrawColor(renderer, cor.r, cor.g, cor.b, cor.a);
        SDL_RenderDrawRect(renderer,&rect);
    }

    void DesenhaLinhaSimples(int x1,int y1,int x2,int y2,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b, cor.a);
        SDL_RenderDrawLine(renderer,x1,ALT_TELA-y1,x2,ALT_TELA-y2);
    }

    void DesenhaLinhas(SDL_Point* points, int n, PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r, cor.g, cor.b, cor.a);
        SDL_RenderDrawLines(renderer,points,n);
    }

    void DesenharPonto(int x, int y, PIG_Cor cor, int tam)
    {
        int i, j;

        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,cor.a);
        SDL_RenderDrawPoint(renderer,x,ALT_TELA-y);
        for(i=-tam/2; i<=tam/2; i++)
            for(j=-tam/2; j<=tam/2; j++)
                SDL_RenderDrawPoint(renderer,x+i,ALT_TELA-y+j);
    }

    void DesenharPontos(const SDL_Point *points, int count, PIG_Cor cor)
    {
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,cor.a);
        SDL_RenderDrawPoints(renderer,points,count);
    }

    void DesenhaLinhasDisjuntas(int *x,int *y,int qtd,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,cor.a);
        for (int k=0;k<qtd*2;k+=2){
            SDL_RenderDrawLine(renderer,x[k],ALT_TELA-y[k],x[k+1],ALT_TELA-y[k+1]);
        }
    }

    void DesenhaLinhasSequencia(int *x,int *y,int qtd,PIG_Cor cor){
        SDL_SetRenderDrawColor(renderer,cor.r,cor.g,cor.b,cor.a);
        for (int k=0;k<qtd-1;k++){
            SDL_RenderDrawLine(renderer,x[k],ALT_TELA-y[k],x[k+1],ALT_TELA-y[k+1]);
        }
    }

    int CriaFonteFundo(char *nome,int tamanho,int estilo,char *arquivoFundo,int contorno,PIG_Cor corContorno)
    {
        int indice = getProximoIndiceFonte();
        fontes[indice] = new CMapaCaracteres(nome,renderer,tamanho,estilo,arquivoFundo,contorno,corContorno);
        return indice;
    }

    int CriaFonteNormal(char *nome,int tamanho,int estilo,PIG_Cor corLetra,int contorno,PIG_Cor corContorno)
    {
        int indice = getProximoIndiceFonte();
        fontes[indice] = new CMapaCaracteres(nome,renderer,tamanho,estilo,corLetra,contorno,corContorno);
        return indice;
    }

    int DestroiFonte(int idFonte)
    {
        delete fontes[idFonte];
    }

    inline int GetLarguraPixels(char *str,int numFonte=0){
        fontes[numFonte]->GetLarguraPixelsString(str);
    }

    inline void EscreverCentralizada(char *str,int x,int y,int numFonte=0){
        fontes[numFonte]->EscreveStringCentralizado(str,x,y);
    }

    inline void EscreverDireita(char *str,int x,int y,int numFonte=0){
        fontes[numFonte]->EscreveStringDireita(str,x,y);
    }

    inline void EscreverEsquerda(char *str,int x,int y,int numFonte=0){
        fontes[numFonte]->EscreveStringEsquerda(str,x,y);
    }

    void GetPixel(int x,int y,int *r,int *g,int *b) {
        if (x<0 || x>=LARG_TELA  ||y<0 ||y>=ALT_TELA) return;

        SDL_Surface* saveSurface = NULL;
        SDL_Surface* infoSurface = NULL;
        infoSurface = SDL_GetWindowSurface(window);
        if (infoSurface != NULL) {
            unsigned char * pixels = new unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
            if (pixels != 0) {
                if (SDL_RenderReadPixels(renderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) == 0) {
                    Uint8 mr,mg,mb;
                    Uint32 *p = (Uint32*) pixels;
                    p += (x+(ALT_TELA-y)*LARG_TELA);
                    SDL_GetRGB(*p,(const SDL_PixelFormat*) &infoSurface->format->format,&mr,&mg,&mb);
                    *r = mr;
                    *g = mg;
                    *b = mb;
                }
                delete[] pixels;
            }
            SDL_FreeSurface(infoSurface);
        }
    }

    inline float GetFPS(){
        return contFPS/timerFPS->GetTempoDecorrido();
    }

    void PreparaOffScreenRenderer(int altura,int largura){
        if (offRenderer) delete offRenderer;
        offRenderer = new COffscreenRenderer(altura,largura);
    }

    inline void SalvaOffScreenBMP(char *nomeArquivo){
        if (offRenderer)
            offRenderer->SalvarImagem(nomeArquivo);
    }

    inline void PintaAreaOffScreen(int px,int py,PIG_Cor cor){
        if (offRenderer)
            offRenderer->PintarArea(px,py,cor);
    }

    inline void PintaFundoOffScreen(PIG_Cor cor){
        if (offRenderer)
            offRenderer->PintarFundo(cor);
    }

    inline void DesenhaRetanguloOffScreen(int x1,int y1,int altura,int largura,PIG_Cor cor){
        if (offRenderer)
            offRenderer->DesenharRetangulo(x1,y1,altura,largura,cor);
    }

    inline void DesenhaRetanguloVazadoOffScreen(int x1,int y1,int altura,int largura,PIG_Cor cor){
        if (offRenderer)
            offRenderer->DesenharRetanguloVazado(x1,y1,altura,largura,cor);
    }

    inline void DesenhaLinhaOffScreen(int x1,int y1,int x2,int y2,PIG_Cor cor){
        if (offRenderer)
            offRenderer->DesenharLinha(x1,y1,x2,y2,cor);
    }

    void SaveScreenshotBMP(char *nomeArquivo)
    {
        SDL_Surface* saveSurface = NULL;
        SDL_Surface* infoSurface = NULL;
        infoSurface = SDL_GetWindowSurface(window);
        if(infoSurface != NULL)
        {
            unsigned char * pixels = new unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
            if(pixels != 0)
            {
                if(SDL_RenderReadPixels(renderer,
                                        &infoSurface->clip_rect,
                                        infoSurface->format->format,
                                        pixels, infoSurface->w * infoSurface->format->BytesPerPixel) == 0)
                {
                    saveSurface = SDL_CreateRGBSurfaceFrom(pixels,
                                                           infoSurface->w,
                                                           infoSurface->h,
                                                           infoSurface->format->BitsPerPixel,
                                                           infoSurface->w*infoSurface->format->BytesPerPixel,
                                                           infoSurface->format->Rmask,
                                                           infoSurface->format->Gmask,
                                                           infoSurface->format->Bmask,
                                                           infoSurface->format->Amask);
                    if(saveSurface != NULL)
                    {
                        SDL_SaveBMP(saveSurface, nomeArquivo);
                        SDL_FreeSurface(saveSurface);
                    }
                }
                delete[] pixels;
            }
            SDL_FreeSurface(infoSurface);
        }
    }


};

typedef CJogo* Jogo;
