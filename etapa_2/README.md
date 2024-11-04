# Tarefas

Nessa etapa uma informação importante foi descoberta ao longo da criação das tarefas. Para o VxWorks por padrão, um processo é encerrado quando a função main() retorna, porque o compilador C insere automaticamente uma chamada a exit() no final de main(). Isso é um comportamento indesejável se main() criar outras tarefas, pois encerrar o processo exclui todas as tarefas que estavam sendo executadas nele. Para evitar que isso aconteça, qualquer aplicativo que use main() para criar tarefas pode chamar taskExit() em vez de return() como a última instrução na rotina main(). Quando main() inclui taskExit() como sua última chamada, a tarefa inicial do processo pode sair sem que o kernel encerre automaticamente o processo.

## Tarefa 1:

Criar uma tarefa que lê um pacote de rede

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

Limitar pacotes enviados e adequar envios e recebimento para usar o protocolo UDP

## Tarefa 4:

Sincronizar tarefas períodicas
