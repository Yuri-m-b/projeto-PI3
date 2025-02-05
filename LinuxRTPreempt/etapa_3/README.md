# Tarefas

Para essa etapa do projeto a instalação de uma versão linux do Raspberry que tivesse suporte a RT preempt (Real Time Computing) foi necessaria. Após diversas tentativas falhas de conseguir instalar uma OS que funcionasse nas Raspberry 3B+, foi encontrado uma versão que tinha todos os suportes desejados, sendo ela a versão `6.6.51+rpt-rpi-v8`.
Os comandos a seguir comprovam que o software da Raspberry está com suporte ao RT Preempt:
![RTPREEMPT](/LinuxRTPreempt/etapa_3/image_files/rtpreempt.png)

A configuração do kernel indica que o Raspberry Pi está executando um kernel totalmente preemptivo (CONFIG_PREEMPT = y), projetado para baixa latência e capacidade de resposta em tempo real.

### Comandos utilizados para passar os scripts para as raspberry e compilação:

Passando arquivos via ssh:
```
scp time_sender.c pi@192.168.0.95:/home/pi/
scp time_receiver.c pi@192.168.0.35:/home/pi/
```
Nas raspberry:
```
gcc -o time_sender time_sender.c -lpthread
gcc -o time_receiver time_receiver.c
```

Para rodar scripts compilados:
```
sudo ./time_receiver
sudo ./time_sender
```

## Tarefa 1
Seguindo o que já teria sido feito até o momento, foi criado dois códigos que realizam a comunicação entre as duas raspberry. Sendo um código feito para enviar pacotes com a informação de tempo de uma raspberry, enquanto que outro código criado para esperar e receber os pacotes enviados da outra raspberry. Tendo assim como resultado as informações no console de ambos os raspberry.
![tarefa1](/LinuxRTPreempt/etapa_3/image_files/etapa3_tarefa1.png)

Ambos códigos recebem e enviam de forma automatica, e precisam ser encerrados manualmente.

Continuando com a mesma ideia, foi feito uma segunda versão do `time_receiver`, onde um atraso proposital foi adicionado ao codigo, podendo assim verificar se o RT preempt através da diferença de tempo entre o pacote enviado, e o recebimento do pacote após o atraso, como pode ser visto na imagem a seguir:
![tarefa1pt2](/LinuxRTPreempt/etapa_3/image_files/etapa3_tarefa1_pt2.png)

## Tarefa 2
Para essa tarefa foi pensando em modificar o script [time_sender](/LinuxRTPreempt/etapa_3/tarefa_1/time_sender.c) para que seja utilizado dois threads trabalhando em paralelo para enviar dados.
Sendo assim o primeiro script idealizado foi  o [multi_thread_sender](/LinuxRTPreempt/etapa_3/tarefa_2/multi_thread_sender.c) que utilizando dois threads que chamam a mesma função, envia as informações de data e hora para o receptor. Tendo cada thread com um offset diferentes, enviando assim tempos diferentes em paralelo.
![tarefa2](/LinuxRTPreempt/etapa_3/image_files/etapa3_tarefa2.png)

Para a continuidade dessa tarefa, foi criado outro script que utiliza 2 threads porém dessa vez com cada thread utilizando uma função diferente. Sendo assim o script [thread_message](/LinuxRTPreempt/etapa_3/tarefa_2/thread_message.c) tem um thread enviando as informações de tempo enquanto que o outro thread envia uma mensagem estatica para o receptor.
![tarefa2pt2](/LinuxRTPreempt/etapa_3/image_files/etapa3_tarefa2_pt2.png)

Nota-se que a execução da thread 1 acaba antes da thread 2 porque durante a execução da função para a thread 2, existe um delay de 2 segundos enquanto que para a execução da função da thread 1 é utilizado um delay de 1 segundo, fazendo assim também com que a thread 1 execute 2 vezes no período de intervalo da thread 2.

## Tarefa 3

Durante a realização das Tarefa 1 e Tarefa 2 foi possível acompanhar e rastrear o envio dos pacotes utilizando o software `Wireshark`, poden ser notado a comunicação e as informações dos pacotes.

![tarefa3](/LinuxRTPreempt/etapa_3/image_files/wireshark_etapa3.png)

## Extra

Aproveitando uma das fontes de refêrencia para essa etapa (a vídeo aula do professor Rômulo Silva de Oliveira), foi adicionado um script extra mostrado em sua aula que demonstra o comportamento de um sistema em tempo real.

![extra](/LinuxRTPreempt/etapa_3/image_files/periodica.png)

Comparando os resultados com os do professor, podemos notar uma diferença grande nos resultados obitidos, porém no caso do professor, ele executou o codigo em uma instalação do linux sem RT Preempt, enquanto que nesse projeto estamos utilizando o linux com suporte a RT Preempt, podendo assim ter afetado a resposta em nano segundos do codigo, causando com que a variavel tivesse um valor negativo, por ser um número muito pequeno, quebrando assim o código.

### Referências:
https://stackoverflow.com/questions/16502054/why-sched-setscheduler-does-not-work-if-not-wrapped-within-an-if-block

https://stackoverflow.com/questions/24666186/why-memset-sockaddr-in-to-0

https://stackoverflow.com/questions/26908439/c-program-to-receive-and-send-the-same-packets-out-over-another-interface

https://forums.raspberrypi.com/viewtopic.php?t=334889

https://www.youtube.com/watch?v=rndtCLAqI-A

https://www.youtube.com/watch?v=N_98eU_kfcE
