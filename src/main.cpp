#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(const char *texto);



void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //? readStringUntil espera nommaximo esse tempo (ms) pelo '\n'
  //? o padrao é 1000ms
  Serial.println("Digite o seu nome e pressione ENTER:");
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

    if (c == '\r') continue;
    //* o continue pula o restante do while naquela volta e passa pra leitura do proximo caractere.
    //* o \r é lido e simplesmente descartado, sem entrar no vetor

    if(c == 'n')
    {
      frase[position]= '\0';
      
      if(position > 0)  processarFrase(frase);
      
      position = 0;
     
    }

    else if (position < TAMANHO_MAX - 1)
    {
      frase[position] = c;
      position++;
    }
  }
}

void processarFrase(const char* texto)
{
Serial.printf("Recebido: [%s]\n\r", texto);

if (strcmp(texto, "ola") == 0)
Serial.println("ola para voce tambem");

}
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
