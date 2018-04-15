//Tomas de Camino Beck
//www.funcostarica.org

#include "Chromosome.h"
#include <Adafruit_CircuitPlayground.h>

#define N 20

//crea la poblacion
population pop(N);


void setup() {
  randomSeed(analogRead(A9));
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(255);
  CircuitPlayground.clearPixels();
  Serial.begin(9600);
  //inicia con una población aleatoria
  pop.mutateChromosomes(0.5, 0);
}

void loop() {

  //aplica recombinación con cromosoma 0 a 2 y los copia en una población
  pop.copyCrossover(2);

  //genera mutaciones en cada gen con
  //probabilidad 0.05
  pop.mutateChromosomes(0.05, 1);

  //evalua
  evaluate();

  //Ordena los cromosomas de mayor a menor fitness
  pop.sort();

  //prende los leds que correspoden al mejor fitness
  setPixels(0);
  Serial.print(CircuitPlayground.lightSensor());
  Serial.print(",");
  Serial.println(pop.countBits(0));


  //espera segundo y medio
  delay(2000);
}

//prende los pixles dependiendo de los bits en el cromosoma
void setPixels(int a) {
  for (int i = 0; i < 8; i++) {
    if (bitRead(pop.chromosome[a], i)) {
      CircuitPlayground.setPixelColor(i + 2, 255, 255, 255);
    } else {
      CircuitPlayground.setPixelColor(i + 2, 0, 0, 0);
    }
  }
}

//Calcula el fitnes basado en la cantidad de luz (maximiza)
//y la cantidad de leds (minimiza)
void fitness(int a) {
  pop.fitness[a] = 0.5 * float(CircuitPlayground.lightSensor()) - 0.5 * float(pop.countBits(a));
}

//Evalua cada cromosoma
void evaluate() {
  for (int i = 0; i < pop.n; i++) {
    setPixels(i);
    //gives LED time to turn on
    delay(100);
    fitness(i);
  }
}

