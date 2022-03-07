O Problema que esse código resolve é o seguinte: 

Implementação do problema do estacionamento, usando um semáforo de contagem para controlar o acesso ao interior do estacionamento (recurso compartilhado).
O semáforo foi configurado para permitir que, no máximo, 100 carros estejam dentro do estacionamento (esse valor pode ser alterado no código, conforme a vontade de quem for utilizar)
Estão simulados, no código, a chegada de carros, e os valores de permanencia dos carros dentro do estacionamento pode ser alterado de acordo com a vontade do programador.

A Seguir, estão demonstrados os dados obtidos com os testes realizados no código
desenvolvido:

![image](https://user-images.githubusercontent.com/39598441/157034191-5b4fa729-eb3f-433d-b6ca-cd6b708b6fb9.png)

sendo sleep o tempo de permanencia dos carros. 

É perceptível que quanto maior a quantidade de carros, maior o tempo de
processamento para valores de sleep (tempo em que um carro fica estacionado) iguais,
conforme observado na primeira linha de dados da tabela. Além disso, quando alterado o
valor de sleep para maior, o tempo de processamento também aumenta, conforme
demonstrado na terceira linha de dados na tabela, onde para um valor alto de sleep e um
valor alto de carros, demorou mais de 2h para a execução do processo. Então, o tempo de
processamento é determinado pelos valores de quantidade de carros e sleep,
proporcionalmente.
