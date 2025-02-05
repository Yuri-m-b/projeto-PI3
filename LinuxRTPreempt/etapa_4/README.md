## Etapa 4: Comunicação e monitoramento de pacotes de rede

Nesta etapa final faremos uma análise mais aprofundada dos códigos criados nesta etapas. Queremos saber não só resultados temporais dos códigos como também se houve perdas de pacotes, e como é o comportamente de cada script criado. Todos os códigos criados nesta etapa estarão na pasta [codes](/etapa_4/codes/) e os arquivos com os dados temporais podem ser encontrados [aqui](/etapa_4/dados/).


### Script 1: Transferência de string.

Pensando em um nível gradativo de complexidade para as tarefas, o primeiro código criado sendo ele o mais "simples", tem como objetivo o envio de uma string estatica 100 vezes para o receptor. O receptor então recebe a string e o valor do tempo de origem dela, fazendo assim uma comparação com o valor do tempo recebido. Após receber a mensagem 100 vezes o receptor então cria um arquivo output no formato `csv`que será lido e usado para retirar as informações do tempo e a plotagem do gráfico.

Sendo assim os 2 primeiros códigos criados foram [string_receiver.c](/etapa_4/codes/string_receiver.c) e [string_sender.c](/etapa_4/codes/string_sender.c), seguindo a logica que vemos fazendo até o momento, durante a execução dos códigos podemos ver a mensagem e o tempo ser "printados" na tela:

![string_sender1](/etapa_4/image_files/string_sender1.png)

Como explicado anteriormente, ao finalizar os envios o `string_receiver`cria um arquivo output [string_output1.csv](/etapa_4/dados/string_output1.csv) que contém as informações de análise que queremos. Para facilitar a extração das informações do arquivo `csv`foi criado um script em python que extrai as informações relevantes e gera o gráfico dos dados obtidos [data_analyzer](/etapa_4/codes/data_analyzer.py)

Resultados para [string_output_1](/etapa_4/dados/string_output1.csv):

- Valor mínimo do tempo: 0.000255146 s

- Valor máximo do tempo: 0.001617226 s

- Valor médio do tempo: 0.00039557613999999984 s

![grafico_string_sender1](/etapa_4/image_files/string_sender1_graph.png)

Não houve perca de pacotes para esse código. Com o intuito de verificar se existe alguma diferença nos resultados caso a string enviada for menor, foi modificado o codigo `string_sender` para que a string enviasse fosse apenas 2 letras e os resultados foram o seguinte.

![string_sender2](/etapa_4/image_files/string_sender2_.png)

Resultados para [string_output_2](/etapa_4/dados/string_output2.csv):

Valor mínimo do tempo: 0.000255146 s

Valor máximo do tempo: 0.001617226 s

Valor médio do tempo: 0.00039557613999999984 s

Como os resultados foram virtualmente o mesmo, o gráfico também será.

![grafico_string_sender2](/etapa_4/image_files/string_sender2_graph.png)

Neste primeiro script podemos concluir então que a alteração no tamanho da string enviada não altera os valores de tempo em relação ao envio e recebimento da tarefa, neste caso também não houveram percas de pacotes durante a transferência, novamente pelo fato que o envio de string é um processo leve para o sistema em questão.

### Script 2: Transferência de imagem.

Para aumentar a complexidade do código e testar o quanto isso irá influenciar no tempo e transferência de pacotes, para o segundo código foi pensando em envio de imagem, porém diferente do código anterior, o envio de imagem é realizado em formato de `bits` e em `chuncks`, sendo a imagem enviada apenas uma vez e a analise de tempo feita por cima do tempo de envio e recebimento desses `chuncks`

Conforme o esperado foi criado dois novos códigos para o envio[image_receiver.c](/etapa_4/codes/image_receiver.c) e [image_sender.c](/etapa_4/codes/image_sender.c), seguindo a logica que vemos fazendo até o momento, durante a execução dos códigos podemos ver a de quantos `chuncks` serão enviados, qual `chuncks` está sendo enviado e o seu tempo de origem. A imagem enviada é [essa](/etapa_4/image_files/bmw_1080.jpg), uma imagem de 1080p no formato `jpg`.
Tendo assim os resultados:

![image_sender1](/etapa_4/image_files/image_sender1.png)

Resultados para [image_output1](/etapa_4/dados/image_output1.csv):

- Valor mínimo do tempo: 0.055742357 s

- Valor máximo do tempo: 0.057853962 s

- Valor médio do tempo: 0.055813494990291306 s

![grafico_image_sender1](/etapa_4/image_files/image_sender1_graph.png)


A análise que pode ser feita pelo gráfico obitidos através dos dados, é que o comportamento do tempo de envio e entrega se manteve parecido com ao código do `string_sender`, tendo os maiores atrasos no ínicio do envio, e um tempo de envio "constante" ao decorrer do envio. Porém os valores de tempo para esse código se mostraram maiores. 

Podemos confirmar que a imagem foi corretamente enviada pelo fato que a imagem recebida na raspberry receptora não está corrompida como pode ser visto [aqui](/etapa_4/image_files/received_image.jpg)

Para fazer uma segunda comparação ainda utilizando os mesmos dois códigos foi utilizado uma segunda imagem [bmw_4k.jph](/etapa_4/image_files/bmw_4k.jpg) porém no tamanho 4k, sendo assim mais pesada.

![image_sender2](/etapa_4/image_files/image_sender2.png)

Resultados para [image_output2](/etapa_4/dados/image_output2.csv):

- Valor mínimo do tempo: 0.05387325 s

- Valor máximo do tempo: 0.056532928 s

- Valor médio do tempo: 0.05392091775120153 s

![grafico_image_sender1](/etapa_4/image_files/image_sender2_graph.png)

Neste segundo caso era esperado que os valores de tempo tivessem uma diferença significativa, porém resultados se mostraram parecidos, possívelmente pelo fato que as imagens mesmo com tamanhos diferentes, têm tamanho parecidos. Sendo [bmw_1080](/etapa_4/image_files/bmw_1080.jpg) `1.2 Mb` e [bmw_4k](/etapa_4/image_files/bmw_4k.jpg) `2.8 Mb`.
Assim como na primeira imagem não houveram perca de pacotes e imagem recebida não aparenta ter nenhuma alteração ou estar corrompida como pode ser visto [aqui](/etapa_4/image_files/received2_image.jpg)


### Script 3: Transferência de imagem durante filtragem.

Chegando ao código final, nossa ideia para esse último codigo seria adicionar uma nova função ao [image_receiver](/etapa_4/codes/image_receiver.c) para que ele além de receber os pacotes de imagem, fizesse uma filtragem desses pacotes com o intuito de aumentar o tempo de processamento de uma das raspberry para verificar se esse aumento de processamento causaria um maior número de perca de pacotes e alterações no tempo de envio e recebimento desses pacotes. Sendo assim criamos o código [image_filter_receiver.c](/etapa_4/codes/image_filter_receiver.c), e executando ele juntamente com o já criado [image_sender](/etapa_4/codes/image_sender.c), fizemos testes enviando a mesma [imagem](/etapa_4/image_files/bmw_4k.jpg), e os resultados foram:

![image_filter1](/etapa_4/image_files/image_filter1.png)

Resultados para [image_output3](/etapa_4/dados/image_output3.csv):

Valor mínimo do tempo: 0.000781094 s

Valor máximo do tempo: 0.003215529 s

Valor médio do tempo: 0.0008334668935305009 s

Pacotes perdidos: 0

![grafico_image_filter1](/etapa_4/image_files/image_filter1_graph.png)

Como pode ser visto pelos resultados, não houve perca de pacotes mesmo com a adição de um filtro para aumentar o processamento do código. Para verificar o comportamento do sistemas em casos onde realmente acontece falhas ao enviar o pacotes decidimos testar o mesmo código [image_filter_receiver](/etapa_4/codes/image_filter_receiver.c) porém com um delay de 50 us durante o processamento dos pacotes simulando uma demanda alta no processamento, tendo assim os resultados:

![image_filter2](/etapa_4/image_files/image_filter2.png)

Resultados para [image_output3](/etapa_4/dados/image_output3_2.csv):

Valor mínimo do tempo: 0.000371448 s

Valor máximo do tempo: 5.75733146 s

Valor médio do tempo: 3.5777404062643683 s

Pacotes perdidos: 2531

![grafico_image_filter2](/etapa_4/image_files/image_filter2_graph.png)

Neste caso podemos concluir que o sistema está corretamente detectando os casos onde houveram pacotes perdidos no envio da imagem. Neste caso também embora o delay seja apenas de 50 us pode ser visto como isso afetou o valor máximo do tempo de envio e o valor médio de envios. Concluindo assim que em casos onde existe um processamento muito alto sendo realizado em uma das raspberry durante o processo de envio de dados, há uma grande chance que esse processamento cause atrasos e percas de pacotes significativos podendo causa com que o "objeto" sendo enviado seja corrompido.

## Conclusão:

### Tabela de Resultados

| Code            | Tempo mínimo (s) | Tempo máximo (s) | Tempo médio (s) | Percas de pacote |
| --------------- | -----------------| ---------------- | --------------- | ---------------- |
| String sender 1 |      0.000255146 |     0.001617226  |  0.00039557614  |        0         |
| String sender 2 |      0.000255146 |     0.001617226  |  0.00039557614  |        0         |
| Image sender 1  |      0.055742357 |     0.057853962  |  0.05581349500  |        0         |
| Image sender 2  |      0.053873250 |     0.056532928  |  0.05392091775  |        0         |
| Image filter 1  |      0.000781094 |     0.003215529  |  0.00083346689  |        0         |
| Image filter 2  |      0.000371448 |     5.757331460  |  3.57774040626  |        2531      |

Finalmente considerando os resultados da Tabela de Resultados, podemos notar como o aumento de complexidade e processamento dos códigos afetaram os valores de tempo obitidos durante os testes.

Para os códigos enviando apenas uma string estatica, a alteração do tamanho da string não afetou nenhum dos tempos analisados e também não houveram percas de pacotes.

Agora nos códigos que enviam uma imagem, pode ser notado uma diferença grande no tempo com relação aos `string senders`, tendo um aumento no tempo entre envio e recebimento porém sem percas de pacotes. As duas imagens usadas para os testes, embora de tamanho diferentes não mostraram ter afetado os resultados obitidos.

Por fim os `image filters`foram criados com intuito de adicionar um processamento maior aos já existente `image receiver`, porém os resultados mostraram que apenas quando colocamos um delay durante o processamento dos pacotes foi quando os resultados esperados começaram a ser evidenciandos, como o tempo médio e tempo máximo de envio e recebimento serem significativamente maiores dos que os outros códigos, além de percas de pacote acontecerem durante o processo.

Por fim mais testes e correções nos códigos criados nesse projetos poderiam ser feitos como melhoria para uma maior obentação de dados e análise de dados. No geral o projeto atendeu as expectativas e todos objetivos criados para esse projeto foram obitidos.

## Referências:

https://stackoverflow.com/questions/5888737/c-socket-send-commands-in-string

https://stackoverflow.com/questions/72243701/writing-an-image-to-a-socket-in-c

https://gitee.com/openharmony/docs/blob/c940e87462148931d2f51ac65636749941d43648/en/application-dev/media/image/image-receiver-native.md

https://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf

https://www.youtube.com/watch?v=WbLOh6NQzyE

https://stackoverflow.com/questions/29979826/blur-image-filter-c

https://www.quora.com/How-do-you-turn-an-image-into-grayscale-in-C

https://abhijitnathwani.github.io/blog/2018/01/08/rgbtogray-Image-using-C

