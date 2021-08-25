// Controle de Motor de Passo com Modulo driver A4988
// Driver A4988 / Motor de Passo Bipolar Nema 17/ Arduino Uno / IDE 1.8.13
 
// Definiçoes das Portas Digitais do Arduino - Driver A4988

int DIR = 5;                                      // Porta digital D05 - Direção (Direction)
int STP = 6;                                      // Porta digital D06 - Passo (Step)
int SLP = 7;                                      // Porta digital D07 - Pausa (Sleep)
int RST = 8;                                      // Porta digital D08 - Redefinir (Reset)
int MS3 = 9;                                      // Porta digital D09 - MS3
int MS2 = 10;                                     // Porta digital D10 - MS2
int MS1 = 11;                                     // Porta digital D11 - MS1
int ENA = 12;                                     // Porta digital D12 - Ativa (Enable)
int MeioPeriodo;                                  // MeioPeriodo do pulso STEP em microsegundos
float PPS = 0;                                    // Pulsos por segundo
boolean sentido = true;                           // Variavel de sentido
long PPR = 200;                                   // Número de passos por volta
long Pulsos;                                      // Pulsos para o driver do motor
int Voltas;                                       // Voltas do motor
float RPM;                                        // Rotações por minuto
 
void setup()
{
  Serial.begin(9600);
 
  DDRD = DDRD | B11111100;                        // Configura Portas D02 até D07 como saída
  disa_A4988();                                   // Desativa o chip A4988
 
  DDRB = 0x0F;                                    // Configura Portas  D08 até D13 como saída
  digitalWrite(SLP, HIGH);                        // Desativa modo sleep do A4988
  rst_A4988();                                    // Reseta o chip A4988
  ena_A4988();                                    // Ativa o chip A4988
}
 
void rst_A4988()
{
  digitalWrite(RST, LOW);                         // Realiza o reset do A4988
  delay (10);                                     // Atraso de 10 milisegundos
  digitalWrite(RST, HIGH);                        // Libera o reset do A4988
  delay (10);                                     // Atraso de 10 milisegundos
}
 
void disa_A4988()
{
  digitalWrite(ENA, HIGH);                        // Desativa o chip A4988
  delay (10);                                     // Atraso de 10 milisegundos
}
 
void ena_A4988()
{
  digitalWrite(ENA, LOW);                         // Ativa o chip A4988
  delay (10);                                     // Atraso de 10 milisegundos
}
 
void HOR()                                        // Configura o sentido de rotação do Motor
{
  Serial.println(" Sentido Horario ");
  digitalWrite(DIR, HIGH);                        // Configura o sentido HORÁRIO
}
 
void AHR()                                        // Configura o sentido de rotação do Motor
{
  Serial.println(" Sentido anti-Horario ");
  digitalWrite(DIR, LOW);                         // Configura o sentido ANTI-HORÁRIO
}
 
void PASSO()                                      // Pulso do passo do Motor
{
  digitalWrite(STP, LOW);                         // Pulso nível baixo
  delayMicroseconds (MeioPeriodo);                // MeioPeriodo de X microsegundos
  digitalWrite(STP, HIGH);                        // Pulso nível alto
  delayMicroseconds (MeioPeriodo);                // MeioPeriodo de X microsegundos
}
 
void FREQUENCIA()                                 // calcula Pulsos, PPS e RPM
{
  Pulsos = PPR * Voltas;                          // Quantidade total de Pulsos (PPR = pulsos por volta)
  PPS = (MeioPeriodo)/ (2);                       // Frequencia Pulsos por segundo
  RPM = (PPS * 60) / PPR;                         // Calculo do RPM
}
 
void FULL()
{
  Serial.println(" Passo Completo  PPR = 200 ");
  PPR = 200;                                      // PPR = pulsos por volta
  digitalWrite(MS1, LOW);                         // Configura modo Passo completo (Full step)
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}
 
void HALF()
{
  Serial.println(" Meio Passo  PPR = 400 ");
  PPR = 400;                                      // PPR = pulsos por volta
  digitalWrite(MS1, HIGH);                        // Configura modo Meio Passo (Half step)
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}
 
void P1_4()
{
  Serial.println(" Micro-passo 1/4  PPR = 800 ");
  PPR = 800;                                      // PPR = pulsos por volta
  digitalWrite(MS1, LOW);                         // Configura modo Micro Passo 1/4
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}
 
void P1_8()
{
  Serial.println(" Micro-passo 1/8  PPR = 1600 ");
  PPR = 1600;                                     // PPR = pulsos por volta
  digitalWrite(MS1, HIGH);                        // Configura modo Micro Passo 1/8
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, LOW);
}
 
void P1_16()
{
  Serial.println(" Micro-passo 1/16  PPR = 3200 ");
  PPR = 3200;                                     // PPR = pulsos por volta
  digitalWrite(MS1, HIGH);                        // Configura modo Micro Passo 1/16
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
}
 
void TesteMotor()
{
  Print_RPM ();                                   // Print Voltas, PPS e  RPM
 
  HOR();                                          // Gira sentido Horario
    for (int i = 0; i <= Pulsos; i++)             // Incrementa o Contador
    {
      PASSO();                                    // Avança um passo no Motor
    }
    disa_A4988();                                 // Desativa o chip A4988
    delayMicroseconds (MeioPeriodo);              // MeioPeriodo de X microsegundos
    ena_A4988();                                  // Ativa o chip A4988
 
  AHR();                                          // Gira sentido anti-Horario
    for (int i = 0; i <= Pulsos; i++)             // Incrementa o Contador
  {
    PASSO();                                      // Avança um passo no Motor
  }
  disa_A4988();                                  // Desativa o chip A4988
  delayMicroseconds (MeioPeriodo);               // MeioPeriodo de X microsegundos
  ena_A4988();                                   // Ativa o chip A4988
}
 
void Print_RPM ()
{
  FREQUENCIA();                                  // calcula Pulsos, PPS e RPM
  Serial.print(" Voltas= ");
  Serial.print(Voltas);
  Serial.print(" Pulsos= ");
  Serial.print(Pulsos);
  Serial.print(" PPS= ");
  Serial.print(PPS, 2);
  Serial.print(" RPM= ");
  Serial.println(RPM, 2);
}
 
void loop()
{
  Serial.println();                              // Mostra a configuração no Monitor Serial
  FULL();                                        // Selecione o modo de passo
  //HALF();                                      // desmarque só o comentario acorde à opção do modo desejada
  //P1_4();
  //P1_8();
  //P1_16();
  Voltas = 1;                                    // Selecione o numero de Voltas
  MeioPeriodo = 1000;                            // Configure os atrasos desejado
  TesteMotor();                                  // Inicia teste do motor
}
