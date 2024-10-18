

/********************************
A função criarSprite() é responsável por alocar uma imagem na memória
********************************/

int PIG_criarSprite(char* nomeArquivo, int retiraFundo = 1)
{
    return CGerenciadorObjetos::CriaObjeto(0,0,nomeArquivo,retiraFundo,255);
}


PIG_Cor PIG_getCorPixelSprite(int indiceSprite, int coluna, int linha)
{
    return CGerenciadorObjetos::getCorPixel(indiceSprite, coluna, linha);
}

int PIG_getLarguraSprite(int idSprite)
{
    int altura;
    int largura;
    CGerenciadorObjetos::GetDimensoesOriginais(idSprite, &altura, &largura);
    return largura;
}

int PIG_getAlturaSprite(int idSprite)
{
    int altura;
    int largura;
    CGerenciadorObjetos::GetDimensoesOriginais(idSprite, &altura, &largura);
    return altura;
}



/********************************
A função destruirSprite() é responsável por desalocar uma imagem da memória
********************************/

void PIG_destruirSprite(int id_objeto)
{
    CGerenciadorObjetos::DestroiObjeto(id_objeto);
}
