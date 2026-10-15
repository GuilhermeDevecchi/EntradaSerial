#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(char *texto);
int lerProximoNumero(char *&cursor);
void aplicaCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //? readStringUntil espera nommaximo esse tempo (ms) pelo '\n'
  //? o padrao é 1000ms
  
}

void loop()
{
  leituraNaoBloqueante();
}

void leituraNaoBloqueante()
{
  const int TAMANHO_MAX = 50; //*tamanho do vetor (inclui /0)
  static char frase[TAMANHO_MAX];
  static int position = 0;

  while (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c == '\r')
      continue;
    //* o continue pula o restante do while naquela volta e passa pra leitura do proximo caractere.
    //* o \r é lido e simplesmente descartado, sem entrar no vetor

    if (c == '\n')
    {
      frase[position] = '\0';

      if (position > 0)
        processarFrase(frase);

      position = 0;
    }

    else if (position < TAMANHO_MAX - 1)
    {
      frase[position] = c;
      position++;
    }
  }
}

void processarFrase(char *texto)
{
  Serial.printf("Recebido: [%s]\n\r", texto);

  char *comando = texto;
  char *parametro = NULL; //* -> \0

  char *espaco = strchr(texto, ' ');
  if (espaco != NULL) // se ele encontrou um espaco....
  {
    parametro = espaco + 1;
    *espaco = '\0';
  }

  if (strcasecmp(comando, "vermelho") == 0)
  {
    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }

  else if (strcasecmp(comando, "verde") == 0)
  {
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }

  else if (strcasecmp(comando, "azul") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 255;
    brilho = 100;
  }

  else if (strcasecmp(comando, "branco") == 0)
  {
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }

  else if (strcasecmp(comando, "desliga") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 0;
  }

  else if (strcasecmp(comando, "RGB") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("use o comando: RGB r g b(o a 255)");
      return;
    }

    char *cursor = parametro;

    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);
    

    vermelho = constrain(r, 0, 255); //* limita o valor ja convertido para o int de 0 a 255
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }

  else if (strcasecmp(comando, "brilho") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("use o comando: BRILHO n (o a 100)");
      return;
    }

    brilho = constrain(atoi(parametro), 0, 100);
  }

  else
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "comando: [%s] nao existe", comando);
    Serial.println(buffer);
    return;
  }
  aplicaCor();
}

void aplicaCor()
{
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;

 rgbLedWrite(PIN_RGB_LED, r, g, b);

  char bufferMensagem[80];
  snprintf(bufferMensagem, sizeof(bufferMensagem), "cor: r=%3d g=%3d b=%3d | brilho: %3d %% | HEX: #%02X%02X%02X",
           vermelho, verde, azul, brilho, r, b, g);
  Serial.println(bufferMensagem);
}
 
int lerProximoNumero(char* &cursor)
{
  if(cursor == NULL) return 0;
  int valor = atoi(cursor);
  char *espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL) ? espaco + 1 : NULL;
  return valor;
}

//* Vermelho
//* Verde
//* Azul
//* Branco
//* Desliga
//* RGB r g b(ex.: RGB 200 125 98)
//* Brilho n (0 a 100%)
void leituraBloqueante()
{ //? available() diz quantos bytes ja chegaram e estao esperando leitura
  if (Serial.available() > 0)
  {
    String texto = Serial.readStringUntil('\n');

    //*trim() remove espaços, /r , /t , /n que alguns terminais enviam
    texto.trim();

    Serial.printf("Seu nome é %s\n\r", texto.c_str());
    Serial.printf("Quantidade de caracteres: %u\n\r", texto.length());

    Serial.printf("Primeira letra: %c\n\r", texto.charAt(0));
    Serial.printf("Ultima letra: %c\n\r", texto[texto.length() - 1]);

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiusculas: %s\n\r", maiusculo.c_str());

    if (maiusculo == "THIAGO")
      Serial.println("Este é o seu nome correto");
  }
}
