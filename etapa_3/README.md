# Tarefas

## Tarefa 1:

A tarefa 1 é uma tarefa com o intuito de utilizar as tarefas desenvolvidas previamente para que duas tarefas, cada uma dentro de uma raspberry diferente, se comuniquem. Contudo, alguns detalhes, que antes não afetavam o desenvolvimento das outras tarefas, surgiram e defasaram o desenvolvimento da atividade.

Um deles sendo o fato de que ambas os dispositivos possuem o mesmo endereço MAC e estão configurados como clientes DHCP, de forma que quando os dois dispositivos são ligados, é necessário um servidor DHCP para fornecer um endereço de IP para os hosts, no entando, por conta do endereço MAC duplicado na rede, a tabela arp é mantida com apenas um endereço IP e um endereço MAC, de forma que se faz necessária a alteração tanto do endereço MAC quanto do endereço IP do dispositivo.

Usando alguns dos comandos do próprio sistema operacional, é possível realizar alterações de endereço IP, por exemplo:
```
ifconfig <NOME DA INTERFACE> inet <NOVO ENDEREÇO IP>
```

Também é possível fazer com que o equipamento não seja um cliente DHCP por meio da linha de comando:
```
ifconfig <NOME DA INTERFACE> -dhcp
```
No entanto, o fato de que o VxWorks realiza uma alteração no endereço MAC das raspberrys para o valor 00:11:22:33:44:55, um dos dispositivos pode ter o endereço MAC alterado por meio do seguinte comando:
```
ifconfig <NOME DA INTERFACE> lladdr <NOVO ENDEREÇO MAC>
```
Realizando ambas as alterações de IP e de endereço MAC a comunicação entre raspberrys deveria ser direta. Entretanto no decorrer do desenvolvimento diversos detalhes que antes não condiziam com as tarefas desenvolvidas nas etapas anteriores acabaram por afetar gravemente o desenvolvimento desta etapa.
O principal deles sendo o fato da fixação de endereço MAC pelo sistema operacional de forma que não é possível realizar uma conexão direta em um sistema onde esteja presente um servidor DHCP, será necessário configurar um dos dispositivos para depois conectar o próximo dispositivo para poder configura-lo e assim por diante.

Apesar das alterações serem simples, consistindo em alterações de IP, MAC e desativação de DHCP, realizar a alteração do endereço MAC do dispositivo torna-o inacessível para qualquer outro host conectado na rede, mesmo com a alteração do endereço MAC sendo visível por meio do comando ```arp -a```.



## Tarefa 2:

A tarefa 2 segue o mesmo princípio da tarefa 1, de forma que se deseja realizar a comunicação entre as raspberrys em períodos diferentes.

## Tarefa 3:

A tarefa 3 foi listada para ser desenvolvida em conjunto com as tarefas 1 e 2, visando usar um sniffer (Wireshark) para analisar os pacotes enviados entre as raspberrys.
O uso do wireshark foi validado, de forma que foi possível visualizar o envio e recebimento de pacotes entre host que não eram de origem ou destinados à máquina que estava com a captura do wireshark rodando.


Por meio do wireshark também é possível realizar o monitoramento dos comandos realizados via telnet, como por exemplo:
![Screenshot from 2024-12-01 12-54-57](https://github.com/user-attachments/assets/4e5ef8bd-f36f-4460-848c-8fe1e87daa65)
