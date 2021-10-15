# BCGE

A Basic Console Game Engine, ou Bixo Console Game Engine (para os íntimos) é um pequeno framework que busca facilitar o desenvolvimento de aplicações gráficas no console do Windows (infelizmente ela não tem nenhum tipo de suporte para usuários de Mac ou Linux)

Uma ideia no mínimo duvidosa, mas que pode ser bastante interessante para propósitos educacionais, visto que ela se trata apenas de um header que pode ser incluído e compilado diretamente na sua main com o apertar de um botão.

Mas não se engane, apesar de ser um framework simples, a BCGE é capaz de desenhar formas primitivas (como linhas, retângulos ou círculos) na tela do console, e usando alguns de seus módulos somos capazes de renderizar ate objetos tridimensionais (apesar de esta ser uma feature que ainda esta muito cedo no desenvolvimento).

## Se familiarizando com o framework
Seguindo o exemplo do arquivo blank_project.c você pode vai ter contato com a estrutura básica de funcionamento do framework. A função OnCreate é chamada apenas no primeiro frame de execução do programa nela você deve inicializar o console, especificando dimensoes e tamanho da fonte. Na função OnUpdate você deve colocar a logica que o programa vai executar a cada frame, bem como desenhar na tela todos os objetos desejados.

## Exemplos
Na pasta exemplos temos uma demonstração do que o framework é capaz

### Snake 
O primeiro programa, e o mais simples de se entender é uma recriação do jogo snake, ele não usa nada de especial, mas é um bom ponto para se familiarizar com as estruturas de dados e funções de desenho do framework


<center><img src="https://cdn.discordapp.com/attachments/898379907791482890/898381429006819348/2021-10-14-22-25-41.gif" ></center>
 <center>Snake em funcionamento</center>

### Flappy-Cube
Depois podemos passar para algo mais complicado com um clone de flappy bird. Esse ja mostra um pouco das capacidades do framework, pois além de simular gravidade e impulso, faz detecção de colisão

<center><img src="https://cdn.discordapp.com/attachments/898379907791482890/898382810061082644/2021-10-14-22-31-52.gif" ></center>
 <center>Flappy-Cube em funcionamento</center>

### Doom
O proximo se trata de uma RayCaster engine, que produz um mapa semelhante ao de jogos antigos como Doom ou Wolfemstein.

<center><img src="https://cdn.discordapp.com/attachments/898379907791482890/898383742870102056/2021-10-14-22-35-01.gif" ></center>
 <center>Doom em funcionamento</center>
 
### Mesh-Renderer
E por ultimo temos um exemplo que usa um dos módulos da BCGE, o BCGE3D (que ainda esta incompleto) para renderizar um objeto tridimensional no console (Loucura)

<center><img src="https://cdn.discordapp.com/attachments/898379907791482890/898385342682509312/2021-10-14-22-41-42.gif" ></center>
 <center>Mesh-Renderer em funcionamento</center>