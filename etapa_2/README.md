# Tarefas

Nessa etapa uma informação importante foi descoberta ao longo da criação das tarefas. Para o VxWorks por padrão, um processo é encerrado quando a função main() retorna, porque o compilador C insere automaticamente uma chamada a exit() no final de main(). Isso é um comportamento indesejável se main() criar outras tarefas, pois encerrar o processo exclui todas as tarefas que estavam sendo executadas nele. Para evitar que isso aconteça, qualquer aplicativo que use main() para criar tarefas pode chamar taskExit() em vez de return() como a última instrução na rotina main(). Quando main() inclui taskExit() como sua última chamada, a tarefa inicial do processo pode sair sem que o kernel encerre automaticamente o processo.

## Tarefa 1:

O objetivo nessa tarefa é criar um source code que receba algum tipo de dado enviado por UDP. Sendo assim o source code [receiver_data](/etapa_2/tarefa_1/receiver_data.c) foi criado.

`receiver_data.c` cria uma tarefa para receber pacotes de rede utilizando sockets UDP. Definindo uma task chamada `receivePackets` que é responsável por configurar um socket de rede, associá-lo a uma porta específica, no caso 21, e escutar pacotes que chegam. A função main segue o padrão que a gente vem usando até então, sendo o local onde a chamada da task é localizada.

Como para essa tarefa é preciso enviar algum tipo de dado para o endereço IP do ambiente VxWorks que está esperando a chegada de dados, foi criado um script em python que envia uma string na mesma quantidade de vezes que é esperado de pacotes no source code utilizado no VxWorks.

O script em python utilizado pode ser encontrado aqui: [udp_sender](/etapa_2/extra/udp_sender.py)

Porém também é possível enviar um dado utilizando o comando no terminal a seguir:
```
echo "Hello from netcat" | nc -u *Raspberry IP* 21

```

Finalmente temos o resultado esperado, como pode ser visto na imagem a seguir:

![tarefa_1](/etapa_2/image_files/tarefa_1.png)

## Tarefa 2:

Para a tarefa 2 o objetivo é criar um `log` para as tasks subsequentes do projeto, com a intenção de facilitar a visualização de mensagens de informações,erros e debugging. A ferramenta VxWorks possui uma `lib` focada especialmente para isso chamada: [logLib](https://www.ecb.torontomu.ca/~courses/ee8205/Data-Sheets/Tornado-VxWorks/vxworks/ref/logLib.html).
<br>Em teoria à adição dessa `lib` deveria ser simples e facilmente utilizada como mostra o exemplo abaixo:

```
#include <logLib.h>
...
    doLog (which)
        {
        char string [100];

        strcpy (string, which ? "hello" : "goodbye");
        ...
        logMsg (string);
        }
...
```
Porém um problema foi encontrado ao compilar os source codes que utilizavam a biblioteca logLib, o linker utilizado na compilação acaba não conseguindo encontrar a função `logMsg` como pode ser visto no erro mostrado no console.
![tarefa_2](/etapa_2/image_files/log_error_1.png)

Possíveis motivos que estão causando esse problema:

 - A lib `logLib.h` não está tendo um link correto durante a compilação.
 - É possível que a função `logMsg` seja parte apenas das bibliotecas para kernel do VxWorks e não esteja disponível para aplicações RTP.

Até o momento foram tentados as seguintes soluções sem resultados positivos:

- Adicionar o comando -l , -L e -I a linha de comando de compilação, tentando linkar o compilador diretamente ao arquivo header da lib `logLib.h`
- Alterar o `#include`do source code para utilizar o Path até o arquivo header.
- Alterar o comando de compilação tirando o comando `-rtp` do prompt.
- Foi verificado  que a função `logMsg()` pode ser utilizada no console conectado ao VxWorks e está funcionando.
- Foi encontrado uma pagína com discurssão sobre esse mesmo problema, porém de acordo com os comentarios não foi encontrado uma solução para o problema, o link pode ser visto [aqui](https://comp.os.vxworks.narkive.com/9vfIPMRg/undefined-symbol-logmsg-help).

Sendo assim, como solução provisória foi criado uma função de log para ser utilizada como desejavamos:
```
void logMsg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
```
Onde:

- `va_list args`: Declara uma variável para armazenar a lista de argumentos adicionais.
- `va_start(args, fmt)`: Inicializa a variável 'args' para obter os argumentos adicionais.
- `vprintf(fmt, args)`: Usa 'vprintf' para imprimir a saída formatada. `vprintf` é semelhante a `printf`, mas aceita `va_list` como argumento
em vez de variáveis individuais, permitindo lidar com um número variável de argumentos
- `va_end(args)`: Finaliza o uso de 'va_list' ao concluir para evitar problemas de memória.

Função criada com base nessa referência: https://cplusplus.com/reference/cstdio/vprintf/


## Tarefa 3:

Para a tarefa 3, o objetivo é criar um código que implementa uma aplicação de envio de pacotes UDP no VxWorks com limitação.

Sendo assim o código [send_limited_data](/etapa_2/tarefa_3/send_limited_data.c) cria uma tarefa separada para enviar uma série de pacotes para um IP e porta de servidor predefinidos, utilizando um loop. A função `sendPackage`, que é executada como uma tarefa, cria um socket UDP, configura o endereço do servidor e entra em um loop para enviar várias mensagens, cada uma com um número de pacote incrementado de acordo com o número máximo desejado. Após o envio de cada mensagem, a função registra o status e fecha o socket ao completar todas as transmissões.

Para acompanhar o término da tarefa, um semáforo binário `taskDoneSem` é usado para sinalizar a função principal assim que todos os pacotes foram enviados. A função principal espera por esse semáforo e, ao recebê-lo, fecha a tarefa usando taskClose para garantir uma gestão limpa de recursos. Em seguida, o semáforo é deletado para evitar vazamento de recursos.

Permitindo então uma gestão assíncrona da tarefa, garantindo sincronização adequada e controle do ciclo de vida da tarefa no VxWorks.

O resultado obtido então é:
![tarefa_3](/etapa_2/image_files/tarefa_3.png)

Como o código para essa tarefa utiliza envio de dados, é preciso abrir um terminal extra que será o receptor das mensagens/pacotes enviados.

## Tarefa 4:

Essa tarefa tem como objetivo criar um caso onde é propositalmente feito uma sincronização entre duas tarefas períodicas sendo acionadas no VxWorks. Dessa forma foi criado o source code [synced_tasks.c](/etapa_2/tarefa_4/synced_tasks.c).

O código `synced-tasks.c` cria duas tarefas periódicas `periodicTask1` e `periodicTask2` que imprimem as letras do alfabeto de a-z e A-Z de forma sincronizada e sequencial, usando um semáforo para sincronizar o acesso à saída de cada tarefa. A utilização de delay para as tasks é necessario para que ambas as tarefas tenham tempo de executar em seus devidos momentos. Finalmente tendo o resulto a seguir:

![tarefa_4](/etapa_2/image_files/tarefa_4.png)

## Análise:

