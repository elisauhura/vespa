Elemento sala   = {"sala", "um pequeno escritório de trabalho", "sala pequena", NULL, LUGAR, .Det.lug.Saidas =  {NULL, NULL, NULL, NULL, NULL,NULL}, .visto = 0};
Elemento quarto = {"quarto", "um grande quarto bagunçado", "quarto enorme", NULL, LUGAR, .Det.lug.Saidas =  {NULL, NULL, &sala, NULL, &sala, NULL}, .visto = 0};


Elemento sotao = {
  .nome = "sotao",
  .genero = 'M',
  .longa = "Parece um lugar bem vazio, mas deve ter algo de útil",
  .curta = "Quanta caixa?! tudo vazia...",
  .cont = NULL,
  .tipo = LUGAR,
  .Det.lug = {
      .Saidas = {
          /*N*/NULL,
          /*S*/NULL,
          /*L*/NULL,
          /*O*/NULL,
          /*C*/NULL,
          /*B*/NULL}
  },
  .animacao = NULL
};

Elemento porao = {
  .nome = "porao",
  .genero = 'M',
  .longa = "um arrepio sobe pela espinha a cada segundo",
  .curta = "aqui da medo",
  .cont = NULL,
  .tipo = LUGAR,
  .Det.lug = {
      .Saidas = {
          /*N*/NULL,
          /*S*/NULL,
          /*L*/NULL,
          /*O*/NULL,
          /*C*/NULL,
          /*B*/NULL}
  },
  /*add trigger to spawn the zombie*/
  .animacao = NULL
};

Elemento telhado = {
  .nome = "telhado",
  .genero = 'M',
  .longa = "Parece um lugar bem vazio, mas deve ter algo de útil",
  .curta = "Quanta caixa?! tudo vazia...",
  .cont = NULL,
  .tipo = LUGAR,
  .Det.lug = {
      .Saidas = {
          /*N*/NULL,
          /*S*/NULL,
          /*L*/NULL,
          /*O*/NULL,
          /*C*/NULL,
          /*B*/NULL}
  },
  .animacao = NULL
};

Elemento cozinha = {
  .nome = "sotao",
  .genero = 'M',
  .longa = "Parece um lugar bem vazio, mas deve ter algo de útil",
  .curta = "Quanta caixa?! tudo vazia...",
  .cont = NULL,
  .tipo = LUGAR,
  .Det.lug = {
      .Saidas = {
          /*N*/NULL,
          /*S*/NULL,
          /*L*/NULL,
          /*O*/NULL,
          /*C*/NULL,
          /*B*/NULL}
  },
  .animacao = NULL
};