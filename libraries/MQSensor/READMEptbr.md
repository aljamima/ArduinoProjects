# MQSensor | ([EN](./README.md))
Uma biblioteca para simplificar o uso de qualquer sensor MQ.  
Testada com os sensores MQ8 e MQ135.  

# Como baixar
## Usando Arduino IDE  
Abra o Arduino IDE, `Sketch > Incluir Biblioteca > Gerenciar Bibliotecas` e pesquise por "`mqsensor`" feita por Daguer.  

## Download Manual
Faça o download da biblioteca na aba [release](https://github.com/daguerpedro/MQSensor/releases).  
Abra o Arduino IDE, `Sketch > Incluir Biblioteca > Adicionar Biblioteca .ZIP` e selecione o .zip.  
Veja os exemplos em Arduino IDE, `Arquivo > Exemplos > Sensor MQ`.  

# Como usar  
**Para cada sensor físico MQ-X**, é necessário **criar uma nova instância** da classe 'MQ' no código e anexar uma porta analógica ligada ao sensor.  
O exemplo seguinte é para um sensor Mq135:  
- `AX` é a porta analógica.
- `RL` É o valor da resistência de carga em kOhm ligada ao sensor MQ (Não ao trimpot da placa).  
- É necessário passar os parâmetros 'resolution'(Em Volt) e 'bits'(Inteiro Positivo) de acordo com o conversor analógico digital utilizado. O conversor pode ser o do próprio Arduino, ou um externo como o ADS115. Para verificar o 'resolution' e os 'bits' do seu Arduino, acesse https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/. 
- **Se você estiver utilizando um Arduino Uno, não é necessário passar os parâmetros 'resolution' e 'bits', o código assumirá 5 e 10.**
```c++
MQSensor mq135(AX, RL, resolução, bits);
```
**É importante usar a função 'calculateR0' antes de tentar ler 'ppm'**, pois os sensores baseiam o valor de **ppm** em um **valor inicial 'R0'**, R0 é a resistência inicial do sensor.  
A função 'calculateR0' irá calcular R0 usando 'X' amostras, salvará o valor R0 na instância e retornará o valor de R0 se você quiser lê-lo.
```c++
float R0 = mq135.calculateR0(int samples);
```
Para ler RS, Tensão ou RS/R0, use:  
```c++
float v = mq135.readVoltage();
float rs = mq135.readRS();
float rsr0 = mq135.readRSR0();
float r0 = mq135.readSavedR0(); //Retorna o mesmo valor de R0 que é usado para dividir RS/R0.
```
**Para ler um ppm, você precisa plotar ppm(y) como uma função de rs/r0(x) e criar uma regressão exponencial. Onde y = a*x^b.**  
Se você não souber como fazer isso, leia o próximo capítulo e consulte este [diretório](./dados).
```c++
float ppm = mq135.readPPM(a, b);
```

### MQ8 
![mq8](./dados/mq8.png)

### MQ135
![mq135](./dados/mq135.png)