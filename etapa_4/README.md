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
