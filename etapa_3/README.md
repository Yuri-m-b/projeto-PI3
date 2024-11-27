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

## Tarefa 2:

A tarefa 2 segue o mesmo princípio da tarefa 1, de forma que se deseja realizar a comunicação entre as raspberrys em períodos diferentes.

## Tarefa 3:

A tarefa 3 foi listada para ser desenvolvida em conjunto com as tarefas 1 e 2, visando usar um sniffer (Wireshark) para analisar os pacotes enviados entre as raspberrys.
